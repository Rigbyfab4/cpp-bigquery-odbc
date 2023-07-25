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

# This bash library has various helper functions to run integration tests

# Downloads the iODBC repo and sets the env IODBC_INCLUDE_PATH pointing to the headers
function integration::odbc_headers() {
  local workspace_path
  workspace_path="$(pwd 2>&1)"
  
  printf "Cloning iODBC github repo...\n"
  git clone https://github.com/openlink/iODBC.git
  
  export IODBC_INCLUDE_PATH="${workspace_path}/iODBC/include"

  # This is the name of DSN set in odnc.ini from simba.zip
  export ODBC_TESTS_DSN="SampleDSN"
}
