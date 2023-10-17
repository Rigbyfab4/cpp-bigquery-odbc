# ~~~
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
# ~~~

cmake_minimum_required(VERSION 3.14)
project(
  cpp-bigquery-odbc-integration-testing
  LANGUAGES CXX
)

include(CppBigqueryOdbcCommon)
include(FetchContent)

# We need googletest for all our tests target.
find_package(GTest CONFIG QUIET)
if (NOT GTest_FOUND)
  FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/v1.13.0.tar.gz
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
else()
  find_package(GTest CONFIG REQUIRED)
endif()

# We need google-cloud-cpp for all our tests target.
find_package(google_cloud_cpp_bigquery_rest CONFIG QUIET)
if (NOT google_cloud_cpp_bigquery_rest_FOUND)
  # These flags need to be set to build google_cloud_cpp_bigquery_rest without examples
  set(GOOGLE_CLOUD_CPP_ENABLE_CTYPE_CORD_WORKAROUND ON)
  set(GOOGLE_CLOUD_CPP_ENABLE_EXAMPLES OFF)
  set(BUILD_TESTING OFF)
  set(GOOGLE_CLOUD_CPP_ENABLE experimental-bigquery_rest)

  FetchContent_Declare(
      google-cloud-cpp
      URL https://github.com/googleapis/google-cloud-cpp/archive/refs/heads/main.zip
  )
  FetchContent_MakeAvailable(google-cloud-cpp)
else()
  find_package(google_cloud_cpp_bigquery_rest CONFIG REQUIRED)
endif()

function (define_integration_tests)
  if (NOT INTEGRATION_TESTING)
    return()
  endif ()
  enable_testing()

  add_executable(
      scheduled_integration_tests
      apis/list_dataset_integration_test.cc
  )

  target_link_libraries(
      scheduled_integration_tests
      GTest::gtest_main
      google_cloud_cpp_bigquery_rest
  )

  target_include_directories(scheduled_integration_tests PUBLIC ./)

  target_compile_features(scheduled_integration_tests PUBLIC cxx_std_17)

  include(GoogleTest)
  gtest_discover_tests(scheduled_integration_tests)
endfunction ()

define_integration_tests()
