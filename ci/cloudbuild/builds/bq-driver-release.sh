#!/bin/bash
#
# Copyright 2025 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -euo pipefail

source "$(dirname "$0")/../../lib/init.sh"
source module ci/install-dependencies.sh

source module ci/cloudbuild/builds/lib/cmake.sh
source module ci/cloudbuild/builds/lib/bazel.sh
source module ci/cloudbuild/builds/lib/secrets.sh
source module ci/cloudbuild/builds/lib/unit-tests.sh
source module ci/lib/io.sh

# Save current workspace path
WORKSPACE_DIR=$(pwd)

# Read and export VCPKG version from file
VCPKG_VERSION=$(cat /tmp/vcpkg-version.txt)
export VCPKG_VERSION
echo "Using VCPKG_VERSION=$VCPKG_VERSION"

# Vcpkg install and configure
export VCPKG_ROOT=/vcpkg
git clone --branch "$VCPKG_VERSION" https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
cd "$VCPKG_ROOT"
git checkout "$VCPKG_VERSION"

./bootstrap-vcpkg.sh -disableMetrics

cd "$WORKSPACE_DIR"
# This runs all the unit tests
mapfile -t args < <(bazel::common_args)
mapfile -t unit_tests_args < <(unit_tests::bazel_args)
mapfile -t secrets_bazel < <(secrets::bazel_args)

io::run bazel test "${args[@]}" "${secrets_bazel[@]}" "${unit_tests_args[@]}" --test_tag_filters=unit-tests ...

if [[ -n "${TAG_NAME:-}" ]]; then
  VERSION="${TAG_NAME#v}"
  echo "Version from Git tag: $VERSION"
else
  VERSION="1.0.0"
  echo "Warning: TAG_NAME and SHORT_SHA not found. Using default version: ${VERSION}"
fi

# Run the integration tests
mapfile -t cmake_args < <(cmake::common_args)

BUILD_DIR="/opt/odbc-driver"
# This is the name of DSN set in odbc.ini
export ODBC_TESTS_DSN="SampleDSNGoogleDriver"
export CPP_BIGQUERY_ODBC_TEST_TABLE_PREFIX=${TRIGGER_NAME//[-:;.,?]/_}_${BRANCH_NAME//[-:;.,?]/_}
export ODBCINSTINI=/opt/odbc-driver/odbcinst.ini

io::run cmake -B "$BUILD_DIR" \
  "${cmake_args[@]}" \
  -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_CXX_STANDARD=17 \
  -DODBC_INTEGRATION_TESTING=ON \
  -DBQ_DRIVER_INTEGRATION_TESTS=ON \
  -DODBC_DEMO_TESTING=OFF \
  -DODBC_EXAMPLES=ON \
  -DODBC_UNIT_TESTING=OFF \
  -DCLIENT_LIBRARY_INTEGRATION_TESTING=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  -DPROJECT_VERSION="${VERSION}"

io::run cmake --build cmake-out
# Copy the roots.pem file to the .so directory to run test cases.
cp /opt/odbc-driver/roots.pem "cmake-out/google/cloud/odbc/roots.pem"
mapfile -t ctest_args < <(ctest::common_args)
# Run integration tests, but do not fail the release if they fail
io::run env -C cmake-out ctest "${ctest_args[@]}" || io::log "Warning: Integration tests failed. Proceeding to package and release the driver anyway."

io::log_h1 "Packaging and Uploading Driver"

RELEASE_DIR="release_package"
mkdir -p "${RELEASE_DIR}/lib"

# Copy driver files
io::run cp -v "/workspace/cmake-out/google/cloud/odbc/libgoogle_cloud_odbc_bq_driver.so" "${RELEASE_DIR}/lib/libgoogle_cloud_odbc_bq_driver.so"

# Copy ODBC config file templates
io::run cp -v "/opt/odbc-driver/odbc_template.ini" "${RELEASE_DIR}/odbc.ini"
io::run cp -v "/opt/odbc-driver/odbcinst_template.ini" "${RELEASE_DIR}/odbcinst.ini"
io::run cp -v "/opt/odbc-driver/googlebigqueryodbc.ini" "${RELEASE_DIR}/googlebigqueryodbc.ini"

# Copy root certificates
io::run cp -v "/opt/odbc-driver/roots.pem" "${RELEASE_DIR}/roots.pem"

io::log_h1 "Generating SBOM (Microsoft SBOM Tool)"

# Generate the SBOM for the C++ dependencies
SBOM_NAME="odbc-driver.${VERSION}.spdx.json"
io::run sbom-tool generate \
  -b "${RELEASE_DIR}" \
  -bc . \
  -pn "ODBC Driver for BigQuery" \
  -pv "${VERSION}" \
  -ps "Google LLC" \
  -nsb "https://github.com/googleapis/cpp-bigquery-odbc" \
  -nsu "linux-${VERSION}" \
  -cd "--DetectorsDisabled Pip,Poetry,Conda,Pipfile" \
  -V Verbose

# Copy and rename the SBOM manifest to the release package directory so it is included in the ZIP
io::run cp -v "${RELEASE_DIR}/_manifest/spdx_2.2/manifest.spdx.json" "${RELEASE_DIR}/${SBOM_NAME}"
io::run cp -v "${RELEASE_DIR}/_manifest/spdx_2.2/manifest.spdx.json" "${SBOM_NAME}"

# Create ZIP file
ZIP_NAME="odbc-driver.${VERSION}.zip"
cd "${RELEASE_DIR}"
io::run zip -r "../${ZIP_NAME}" .
cd ..
io::log "ZIP package created: ${ZIP_NAME}"

# Upload to GCS
export GCS_BUCKET=bq_devtools_release_private
io::log "Uploading ${ZIP_NAME} and ${SBOM_NAME} to gs://${GCS_BUCKET}/drivers/odbc/linux/"
io::run gsutil -m cp "${ZIP_NAME}" "gs://${GCS_BUCKET}/drivers/odbc/linux/"
io::run gsutil -m cp "${SBOM_NAME}" "gs://${GCS_BUCKET}/drivers/odbc/linux/"
