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

# Custom vcpkg triplet for x64 Linux builds with AddressSanitizer enabled.
# This ensures that vcpkg-installed dependencies (e.g. abseil) are compiled
# with the same -fsanitize=address flags as the rest of the project, preventing
# the abseil raw_hash_set assertion:
#   "Try enabling sanitizers."
# which fires when non-ASAN abseil code is called from ASAN-instrumented code.

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# Add ASAN flags to all vcpkg dependency builds
set(VCPKG_C_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
set(VCPKG_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
set(VCPKG_LINKER_FLAGS "-fsanitize=address")
