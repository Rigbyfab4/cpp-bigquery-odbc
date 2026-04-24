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

#include "google/cloud/odbc/bq_driver/internal/trace_utils.h"
#include "google/cloud/odbc/bq_client_interface/setenv.h"
#include "google/cloud/odbc/testing/utils/status_matchers.h"
#include "google/cloud/internal/getenv.h"
#include <absl/log/log.h>
#include <absl/strings/str_format.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

namespace google::cloud::odbc_bq_driver_internal {

using ::google::cloud::odbc_bigquery_client_interface::SetEnv;
using ::google::cloud::odbc_internal::SQLStates;
using ::google::cloud::odbc_internal::StatusRecordOr;
using google::cloud::odbc_testing_utils::StatusRecordIs;

// Common Test Values.
Section const kDriverSection2{{"LogLevel", "0"}, {"LogPath", "/tmp"}};
Section const kDriverSection4{{"LogLevel", "4"}, {"LogPath", "/tmp"}};
Section const kDriverSection5{{"LogLevel", "1"}};
Section const kDriverSection6{{"LogLevel", "3"},
                              {"LogPath", "/tmp"},
                              {"LogFileCount", "2"},
                              {"LogFileSize", "6"}};

Sections const kConfigSections2{{"Driver", kDriverSection2}};
Sections const kConfigSections4{{"Driver", kDriverSection4}};
Sections const kConfigSections5{{"Driver", kDriverSection5}};
Sections const kConfigSections6{{"Driver", kDriverSection6}};

#ifdef _WIN32
Section const kWINDriverSection1{{"LogLevel", "1"}, {"LogPath", "C:\\b"}};
Sections const kWINConfigSections1{{"Driver", kWINDriverSection1}};
#endif  // _WIN32

static void CreateDummyFile(std::string const& path) {
  std::ofstream ofs(path);
  ofs << "test";
}

void CreateTestFile(std::string const& filename, std::size_t size) {
  std::ofstream file(filename, std::ios::binary);
  file.seekp(size - 1);
  file.write("", 1);
}

void DeleteTestFile(std::string const& filename) {
  std::remove(filename.c_str());
}

void UpdateTraceConfig(std::string const& odbc_trace_config,
                       std::string const& log_path,
                       std::string const& log_level,
                       std::string const& log_file_count,
                       std::string const& log_file_size) {
  std::unordered_map<std::string, std::string> kv;
  std::ifstream input(odbc_trace_config);
  if (input.is_open()) {
    std::string line;

    while (std::getline(input, line)) {
      if (line == "[Driver]" || line.empty()) {
        continue;
      }
      auto pos = line.find('=');
      if (pos != std::string::npos) {
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        kv[key] = val;
      }
    }
    input.close();
  }

  // Update trace config
  kv["LogPath"] = log_path;
  kv["LogLevel"] = log_level;
  kv["LogFileCount"] = log_file_count;
  kv["LogFileSize"] = log_file_size;

  std::ofstream output(odbc_trace_config, std::ios::trunc);
  ASSERT_TRUE(output.is_open());

  output << "[Driver]\n";
  for (auto const& [k, v] : kv) {
    output << k << "=" << v << "\n";
  }
  output.close();
}

TEST(TraceLoggingFile, TraceOptionsEmptyConfigs) {
  std::shared_ptr<Sections> config_sections = nullptr;
  auto opts = TraceOptions::CreateTraceOptionsFile(config_sections);
  EXPECT_THAT(
      opts, StatusRecordIs(SQLStates::k_HY000(), "Invalid ODBC Driver Config"));
}

TEST(GetAbslSeverity, MapLogLevelWithAbslSeverity) {
  EXPECT_EQ(GetAbslSeverity(LogLevel::kLogInfo), absl::LogSeverity::kInfo);
  EXPECT_EQ(GetAbslSeverity(LogLevel::kLogWarning),
            absl::LogSeverity::kWarning);
  EXPECT_EQ(GetAbslSeverity(LogLevel::kLogError), absl::LogSeverity::kError);
}

TEST(GetAbslSeverity, InvalidAbslSeverity) {
  auto invalid = static_cast<LogLevel>(-1);

  EXPECT_EQ(GetAbslSeverity(invalid), static_cast<absl::LogSeverity>(100));
}

TEST(GetLogFileWithIndex, CustomLogPath) {
  std::string log_path = "/custom/path/to/log/file";
  std::string expected = "/custom/path/to/log/file/" + kLogTraceFileName + "_";

  auto actual = GetLogFileWithIndex(log_path);
  EXPECT_TRUE(absl::StartsWith(actual, expected));
  EXPECT_TRUE(absl::EndsWith(actual, ".log"));
}

TEST(ClearOldLogFiles, WhenMaxFileCountIsOne) {
  std::string dir = std::filesystem::temp_directory_path().string();
  std::string file = dir + "/Tracetestingcountone_0.log";

  CreateDummyFile(file);

  ClearOldLogFiles(dir, 5, 1);

  EXPECT_TRUE(std::filesystem::exists(file));

  std::filesystem::remove(file);
}

TEST(ClearOldLogFiles, WhenLessThanMaxCount) {
  std::string dir = std::filesystem::temp_directory_path().string();
  std::string file = dir + "/Tracetestingmaxcount_0.log";

  CreateDummyFile(file);
  ClearOldLogFiles(dir, 2, 5);
  EXPECT_TRUE(std::filesystem::exists(file));

  std::filesystem::remove(file);
}

TEST(ClearOldLogFiles, RemoveOldestLogFile) {
  std::string dir = std::filesystem::temp_directory_path().string();
  std::string separator =
      (!dir.empty() && dir.back() != '/' && dir.back() != '\\') ? "/" : "";

  std::string file_to_delete =
      absl::StrFormat("%s%s%s_%d.log", dir, separator, kLogTraceFileName, 2);

  CreateDummyFile(file_to_delete);
  ClearOldLogFiles(dir, 6, 5);

  EXPECT_FALSE(std::filesystem::exists(file_to_delete));
}

TEST(ClearOldLogFiles, IgnoresMissingOldFile) {
  std::string dir = std::filesystem::temp_directory_path().string();

  // Ensure file does NOT exist
  std::string missing_file = dir + "/trace_10.log";
  std::filesystem::remove(missing_file);

  // Should not throw or crash
  EXPECT_NO_THROW(ClearOldLogFiles(dir, 12, 5));
}

TEST(TraceLoggingFile, TraceOptionsCheckLogFileCountAndLogFileSize) {
  auto config_sections = std::make_shared<Sections>(kConfigSections6);
  StatusRecordOr<std::shared_ptr<TraceOptions>> test_opts_file =
      TraceOptions::CreateTraceOptionsFile(config_sections);
  ASSERT_STATUS_RECORD_OK(test_opts_file);

  EXPECT_EQ(3, (*test_opts_file)->log_level);
  EXPECT_EQ(2, (*test_opts_file)->max_file_count);
  EXPECT_EQ(6, (*test_opts_file)->max_file_size);
}

#if defined(__linux__)
TEST(TraceLoggingFile, VerifyTraceLogFileAndLogFileCount) {
  // original config
  auto original_ini_path = GetOdbcTraceConfigPath();

  int const log_file_count = 2;
  int const log_file_size = 1024;

  // Unique test dir
  auto test_prefix =
      google::cloud::internal::GetEnv("CPP_BIGQUERY_ODBC_TEST_TABLE_PREFIX")
          .value_or("default");
  std::string log_path = "/tmp/" + test_prefix;
  mkdir(log_path.c_str(), 0777);

  // create ini file
  std::string ini_path = log_path + "/googlebigqueryodbc.ini";
  std::ofstream file(ini_path);
  ASSERT_TRUE(file.is_open());
  file.close();

  // override env var to use test config
  SetEnv("GOOGLEBIGQUERYODBCINI", ini_path);

  auto curr_ini_path = GetOdbcTraceConfigPath();
  UpdateTraceConfig(curr_ini_path, log_path, "3",
                    std::to_string(log_file_count),
                    std::to_string(log_file_size));

  auto trace_opts_file = TraceOptions::CreateTraceOptionsFile(curr_ini_path);
  EXPECT_TRUE(trace_opts_file);

  auto logging_init = TraceOptions::InitializeLogging();
  EXPECT_TRUE(logging_init);

  // generate logs
  std::string chunk(500000, 'A');
  for (int i = 0; i < 100; ++i) {
    LOG(INFO) << "This is log INFO testing message. ";
    LOG(INFO) << chunk;
    LOG(WARNING) << "This is log WARNING testing message. ";
    LOG(ERROR) << "This is log ERROR testing message. ";
  }

  // validate multiple log files existed
  std::vector<std::string> log_files;
  for (int i = 0; i < log_file_count; ++i) {
    std::string file =
        absl::StrFormat("%s/%s_%d.log", log_path, kLogTraceFileName, i);
    if (std::filesystem::exists(file)) {
      log_files.push_back(file);
    }
  }

  // Should have atleast 2 files if rotation worked
  EXPECT_EQ(log_files.size(), log_file_count);
  // First file must be max_file_size (1MB)
  EXPECT_LE(std::filesystem::file_size(log_files[0]), log_file_size * 1024);
  // Second file must exist and be smaller
  if (log_files.size() > 1) {
    EXPECT_GT(std::filesystem::file_size(log_files[1]), 0);
  }

  // Delete the test log dir
  if (absl::StartsWith(log_path, "/tmp/")) {
    std::filesystem::remove_all(log_path);
  }
  // Restore original env var
  SetEnv("GOOGLEBIGQUERYODBCINI", original_ini_path);
}
#endif /* defined(__linux__) */

#ifdef WIN32
// TODO(b/375112496) enable this function after trace registry work is done
// TEST(TraceLoggingFile, WINTraceOptionsFromConfigTraceEnabled) {
// #ifdef _WIN64
//   auto sections_status =
//       ParseConfig("SOFTWARE\\Google\\ODBC Driver for BigQuery");
// #else
//   auto sections_status = ParseConfig(
//       "SOFTWARE\\WOW6432Node\\Google\\ODBC Driver for BigQuery");
// #endif  // _WIN64
//   ASSERT_STATUS_RECORD_OK(sections_status);
//   auto sections = *sections_status;

//   for (auto const& it_outer : kWINConfigSections1) {
//     std::string section_name = it_outer.first;
//     Section sample_ini_section = it_outer.second;
//     for (auto& it_inner : sample_ini_section) {
//       std::string property = it_inner.first;
//       EXPECT_EQ(sample_ini_section[property],
//                 (*(sections))[section_name][property]);
//     }
//   }
// }
#endif  // _WIN32

}  // namespace google::cloud::odbc_bq_driver_internal
