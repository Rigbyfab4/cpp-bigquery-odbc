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
# Usage: trigger.sh [options]
#
#   Options:
#     --create=name            Create a new job with the specified name
#     -t|--trigger=id          Uses specific trigger while creating a scheduler job
#     -s|--schedule=frequency  The format is "* * * * *"
#
# Example:
#
#    $ schedule.sh --create <name> -t <trigger_id> -s "0 0 * * *"

set -euo pipefail

source "$(dirname "$0")/../lib/init.sh"
source module ci/lib/io.sh

function print_usage() {
  # Extracts the usage from the file comment starting at line 17.
  sed -n '17,/^$/s/^# \?//p' "${PROGRAM_PATH}"
}

readonly CLOUD_PROJECT="bigquery-devtools-drivers"
readonly CREATE="create"

function create_job() {
  local name="$1"
  local schedule="$2"
  local trigger_uri="$3"
  io::run gcloud beta scheduler jobs create http "${name}" \
      --project "${CLOUD_PROJECT}" \
      --location=us-central1 \
      --schedule "${schedule}" \
      --oauth-service-account-email=cloud-build-trigger-scheduler@bigquery-devtools-drivers.iam.gserviceaccount.com \
      --oauth-token-scope=https://www.googleapis.com/auth/cloud-platform \
      --uri "https://cloudbuild.googleapis.com/v1/projects/${CLOUD_PROJECT}/locations/us-east1/triggers/${trigger_uri}:run"
}

# Use getopt to parse and normalize all the args.
PARSED="$(getopt -a \
  --options="t:s:" \
  --longoptions="create:,trigger:,schedule:,help" \
  --name="${PROGRAM_NAME}" \
  -- "$@")"
eval set -- "${PARSED}"

VERB=""
NAME=""
TRIGGER=""
SCHEDULE=""
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
    -s | --schedule)
      SCHEDULE="$2"
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
    create_job "${NAME}" "${SCHEDULE}" "${TRIGGER}"
    ;;
  -h | --help)
    print_usage
    ;;
  *)
    print_usage
    ;;
esac
