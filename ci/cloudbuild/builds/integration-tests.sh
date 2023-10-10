#!/bin/bash
#
# Copyright 2023 Google LLC
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
source module ci/cloudbuild/builds/lib/bazel.sh
source module ci/lib/io.sh

io::run export CPP_BIGQUERY_ODBC_TEST_GOOGLE_CLOUD_PROJECT=bigquery-devtools-drivers
io::run export CPP_BIGQUERY_ODBC_TEST_BIGQUERY_DATASET=ODBC_TEST_DATASET

mapfile -t args < <(bazel::common_args)
io::run bazel run //google/cloud/odbc/integration_tests:apis_list_dataset_test explicit-adcs
io::run bazel run //google/cloud/odbc/integration_tests:apis_list_dataset_test explicit-adcs
