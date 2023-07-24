// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <iostream>
#include "google/cloud/bigquery/v2/minimal/internal/dataset_request.h"
#include "google/cloud/bigquery/v2/minimal/internal/dataset_client.h"
#include "google/cloud/bigquery/v2/minimal/internal/dataset_options.h"
#include "google/cloud/bigquery/v2/minimal/internal/dataset_connection.h"
#include "google/cloud/bigquery/v2/minimal/internal/dataset_rest_stub.h"

#include "google/cloud/common_options.h"
#include "google/cloud/internal/absl_str_cat_quiet.h"
#include "google/cloud/internal/format_time_point.h"
#include "google/cloud/options.h"
#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/internal/getenv.h"
#include "google/cloud/testing_util/example_driver.h"

#include "testing_util/status_matchers.h"
#include <gmock/gmock.h>

#include <fstream>
#include <iostream>

namespace google {
namespace cloud {
namespace bigquery_v2_minimal_internal {

void SetClientEndpoint(std::vector<std::string> const& argv) {
  if (!argv.empty()) {
    throw google::cloud::testing_util::Usage{"set-client-endpoint"};
  }
  //! [set-client-endpoint]
  // This configuration is common with Private Google Access:
  //     https://cloud.google.com/vpc/docs/private-google-access
  auto options = google::cloud::Options{}.set<google::cloud::EndpointOption>(
      "private.googleapis.com");
  auto client = DatasetClient(MakeDatasetConnection(options));
  //! [set-client-endpoint]
}

//! [custom-idempotency-policy]
class CustomIdempotencyPolicy
    : public DatasetIdempotencyPolicy {
 public:
  ~CustomIdempotencyPolicy() override = default;
  std::unique_ptr<DatasetIdempotencyPolicy>
  clone() const override {
    return std::make_unique<CustomIdempotencyPolicy>(*this);
  }
  // Override inherited functions to define as needed.
};
//! [custom-idempotency-policy]

void SetRetryPolicy(std::vector<std::string> const& argv) {
  if (!argv.empty()) {
    throw google::cloud::testing_util::Usage{"set-client-retry-policy"};
  }
  //! [set-retry-policy]
  auto options =
      google::cloud::Options{}
          .set<DatasetIdempotencyPolicyOption>(
              CustomIdempotencyPolicy().clone())
          .set<DatasetRetryPolicyOption>(DatasetLimitedErrorCountRetryPolicy(3)
                      .clone())
          .set<DatasetBackoffPolicyOption>(
              google::cloud::ExponentialBackoffPolicy(
                  /*initial_delay=*/std::chrono::milliseconds(200),
                  /*maximum_delay=*/std::chrono::seconds(45),
                  /*scaling=*/2.0)
                  .clone());
  auto connection = MakeDatasetConnection(options);

  // c1 and c2 share the same retry policies
  auto c1 = DatasetClient(connection);
  auto c2 = DatasetClient(connection);

  // You can override any of the policies in a new client. This new client
  // will share the policies from c1 (or c2) *except* for the retry policy.
  auto c3 = DatasetClient(
      connection, google::cloud::Options{}
                      .set<DatasetRetryPolicyOption>(DatasetLimitedTimeRetryPolicy(
                                  std::chrono::minutes(5))
                                  .clone()));

  // You can also override the policies in a single call:
  // c3.SomeRpc(..., google::cloud::Options{}
  //     .set<google::cloud::bigquery_biglake_v1::MetastoreServiceRetryPolicyOption>(
  //       google::cloud::bigquery_biglake_v1::MetastoreServiceLimitedErrorCountRetryPolicy(10).clone()));
  //! [set-retry-policy]
}

void ExplicitADCs(std::vector<std::string> const& argv) {
  if (argv.size() == 1 && argv[0] == "--help") {
    throw google::cloud::testing_util::Usage{
        "explicit-adcs"};
  }
  //! [explicit-adcs]
  auto options =
      google::cloud::Options{}.set<google::cloud::UnifiedCredentialsOption>(
          google::cloud::MakeGoogleDefaultCredentials());
  auto dataset_client = DatasetClient(MakeDatasetConnection(options));
  
  ListDatasetsRequest request;
  request.set_project_id("google.com:bq-devtools-test");
  auto range = dataset_client.ListDatasets(request);
  auto begin = range.begin();
  ASSERT_NE(begin, range.end());
  std::vector<std::string> actual_dataset_ids;
  for (auto const& dataset : range) {
    ASSERT_STATUS_OK(dataset);
    actual_dataset_ids.push_back(dataset->id);
  }
  for (auto dataset_id: actual_dataset_ids) {
    std::cout << "Dataset: " << dataset_id << std::endl;
  }
}

void WithServiceAccount(std::vector<std::string> const& argv) {
  if (argv.size() != 1 || argv[0] == "--help") {
    throw google::cloud::testing_util::Usage{"with-service-account <keyfile>"};
  }
  //! [with-service-account]
  [](std::string const& keyfile) {
    std::cout << "WithServiceAccount::: " << keyfile << std::endl;
    auto is = std::ifstream(keyfile);
    is.exceptions(std::ios::badbit);  // Minimal error handling in examples
    auto contents = std::string(std::istreambuf_iterator<char>(is.rdbuf()), {});
    auto options =
        google::cloud::Options{}.set<google::cloud::UnifiedCredentialsOption>(
            google::cloud::MakeServiceAccountCredentials(contents));
    auto dataset_client = DatasetClient(MakeDatasetConnection(options));
    GetDatasetRequest request;
    request.set_project_id("moonlit-byway-383018");
    request.set_dataset_id("ODBCTESTDATASET");
    auto dataset = dataset_client.GetDataset(request);
    ASSERT_STATUS_OK(dataset);
  }
  //! [with-service-account]
  (argv.at(0));
}

void AutoRun(std::vector<std::string> const& argv) {
  namespace examples = ::google::cloud::testing_util;
  using ::google::cloud::internal::GetEnv;
  if (!argv.empty()) throw examples::Usage{"auto"};
  examples::CheckEnvironmentVariablesAreSet(
      {"GOOGLE_CLOUD_CPP_TEST_SERVICE_ACCOUNT_KEYFILE"});
  auto const keyfile =
      GetEnv("GOOGLE_CLOUD_CPP_TEST_SERVICE_ACCOUNT_KEYFILE").value();

  std::cout << "\nRunning SetClientEndpoint() example" << std::endl;
  SetClientEndpoint({});

  std::cout << "\nRunning SetRetryPolicy() example" << std::endl;
  SetRetryPolicy({});

  std::cout << "\nRunning WithServiceAccount() example" << std::endl;
  WithServiceAccount({keyfile});
}

}  // namespace bigquery_v2_minimal_internal
}  // namespace cloud
}  // namespace google

int main(int argc, char* argv[]) {  // NOLINT(bugprone-exception-escape)
  google::cloud::testing_util::Example example({
      {"set-client-endpoint", google::cloud::bigquery_v2_minimal_internal::SetClientEndpoint},
      {"set-retry-policy", google::cloud::bigquery_v2_minimal_internal::SetRetryPolicy},
      {"explicit-adcs", google::cloud::bigquery_v2_minimal_internal::ExplicitADCs},
      {"with-service-account", google::cloud::bigquery_v2_minimal_internal::WithServiceAccount},
      {"auto", google::cloud::bigquery_v2_minimal_internal::AutoRun},
  });
  return example.Run(argc, argv);
}
