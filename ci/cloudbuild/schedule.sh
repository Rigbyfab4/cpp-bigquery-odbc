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
#
# This script manages Google Cloud Scheduler jobs. It uses 'cloud-build-scheduler'
# service account which can interact with Cloud Build.
#
# Usage: schedule.sh [options]
#
#   Options:
#     --create=name               Create a new job with the specified name
#     -t|--trigger=id             Uses specific trigger while creating a scheduler job
#     -f|--frequency=frequency    The format is "* * * * *"
#     -p|--project=name           The name of the GCP project
#     -s|--service_account=email  The full email of service account, which will be used
#                                 to run Cloud Build builds
#
# Example:
#
#    $ schedule.sh --create integration-tests-scheduler -t c00caade-cf62-4f42-9e1e-b0c12edd516d -f "0 0 * * *" -p bigquery-devtools-drivers -s cloud-build-trigger-scheduler@bigquery-devtools-drivers.iam.gserviceaccount.com

set -euo pipefail

source "$(dirname "$0")/../lib/init.sh"
source module ci/lib/io.sh

function print_usage() {
  # Extracts the usage from the file comment starting at line 17.
  sed -n '17,/^$/s/^# \?//p' "${PROGRAM_PATH}"
}

readonly CREATE="create"

# Use getopt to parse and normalize all the args.
PARSED="$(getopt -a \
  --options="t:f:p:s:" \
  --longoptions="create:,trigger:,frequency:,project:,service_account:,help" \
  --name="${PROGRAM_NAME}" \
  -- "$@")"
eval set -- "${PARSED}"

VERB=""
NAME=""
TRIGGER=""
FREQUENCY=""
PROJECT=""
SERVICE_ACCOUNT=""
while true; do
  case "$1" in
    --create)
      VERB="${CREATE}"
      NAME="$2"
      shift 2
      ;;
    -t | --trigger)
      TRIGGER="$2"
      shift 2
      ;;
    -f | --frequency)
      FREQUENCY="$2"
      shift 2
      ;;
    -p | --project)
      PROJECT="$2"
      shift 2
      ;;
    -s | --service_account)
      SERVICE_ACCOUNT="$2"
      shift 2
      ;;
    -h | --help)
      print_usage
      exit 0
      ;;
    --)
      shift
      break
      ;;
  esac
done

case "${VERB}" in
  "${CREATE}")
    io::run gcloud beta scheduler jobs create http "${NAME}" \
          --project "${PROJECT}" \
          --location=us-east1 \
          --schedule "${FREQUENCY}" \
          --oauth-service-account-email="${SERVICE_ACCOUNT}" \
          --oauth-token-scope=https://www.googleapis.com/auth/cloud-platform \
          --uri "https://cloudbuild.googleapis.com/v1/projects/${PROJECT}/locations/us-east1/triggers/${TRIGGER}:run"
    ;;
  -h | --help)
    print_usage
    ;;
  *)
    print_usage
    ;;
esac
