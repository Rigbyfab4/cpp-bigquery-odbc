// Copyright 2024 Google LLC
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

#include "google/cloud/odbc/bq_driver/odbc_windows.h"
#include "google/cloud/odbc/bq_client_interface/odbc_authentication.h"
#include "google/cloud/odbc/bq_driver/internal/driver_adv_opt_form.h"
#include "google/cloud/odbc/bq_driver/internal/driver_form.h"
#include "google/cloud/odbc/bq_driver/internal/odbc_internal_commons.h"
#include "google/cloud/odbc/bq_driver/internal/trace_utils.h"
#include "google/cloud/odbc/internal/status_record_or.h"

namespace google::cloud::odbc_bq_driver {
using ::google::cloud::odbc_bigquery_client_interface::OauthMechanism;
using google::cloud::odbc_bq_driver_internal::AddDSNToRegistry;
using google::cloud::odbc_bq_driver_internal::AddLogTraceToRegistry;
using google::cloud::odbc_bq_driver_internal::AdvanceOptions;
using google::cloud::odbc_bq_driver_internal::ConvertLPCSTRToString;
using google::cloud::odbc_bq_driver_internal::DriverForm;
using google::cloud::odbc_bq_driver_internal::EditDSNInRegistry;
using google::cloud::odbc_bq_driver_internal::GetPathToOdbcIni;
using google::cloud::odbc_bq_driver_internal::GetSectionWin;
using google::cloud::odbc_bq_driver_internal::GetTraceLogRegistryPath;
using google::cloud::odbc_bq_driver_internal::GetUpperStr;
using google::cloud::odbc_bq_driver_internal::GetValueOrDefault;
using ::google::cloud::odbc_bq_driver_internal::LanguageDialect;
using google::cloud::odbc_bq_driver_internal::LogLevel;
using google::cloud::odbc_bq_driver_internal::LogTraceDialog;
using google::cloud::odbc_bq_driver_internal::ParseConnectionString;
using google::cloud::odbc_bq_driver_internal::RemoveDSNFromRegistry;
using google::cloud::odbc_bq_driver_internal::Section;
using google::cloud::odbc_bq_driver_internal::Trim;
using google::cloud::odbc_internal::StatusRecordOr;

std::string ConvertOAuthMechanism(std::string o_auth_mechanism) {
  std::string o_auth_value;
  if (o_auth_mechanism == "Service Authentication") {
    o_auth_value = std::to_string(
        static_cast<int>(OauthMechanism::kServiceAndUserAccount));
  } else if (o_auth_mechanism == "Application Default Credentials") {
    o_auth_value =
        std::to_string(static_cast<int>(OauthMechanism::kApplicationDefault));
  } else
    o_auth_value = "";
  return o_auth_value;
}

std::string ConvertLanguageDialect(std::string language_dialect) {
  std::string language_dialect_value;
  if (language_dialect == "Standard SQL") {
    language_dialect_value =
        std::to_string(static_cast<int>(LanguageDialect::kStandardSQL));
  } else if (language_dialect == "Legacy SQL") {
    language_dialect_value =
        std::to_string(static_cast<int>(LanguageDialect::kLegacySQL));
  } else
    language_dialect_value = "";
  return language_dialect_value;
}

// TODO(b/b/391859145): Customization and Support For Logging and Driver
// Parameters
std::string ConvertLogLevel(std::string log_level) {
  std::string log_level_val;

  if (log_level == "LOG_TRACE") {
    log_level_val = std::to_string(static_cast<int>(LogLevel::kLogTrace));
  } else if (log_level == "LOG_OFF") {
    log_level_val = std::to_string(static_cast<int>(LogLevel::kLogOff));
  } else {
    log_level_val = "";
  }
  return log_level_val;
}

bool ConfigDSNInternal(HWND hwnd_parent, WORD f_request, LPCSTR lpsz_driver,
                       LPCSTR lpsz_attributes) {
  if (!lpsz_driver) {
    return FALSE;
  }
  std::string attribute = ConvertLPCSTRToString(lpsz_attributes);
  StatusRecordOr<Section> status_or_section = ParseConnectionString(attribute);
  Section section = *status_or_section;
  // TODO(@khushikathuria008): Make "DSN" and other key as constants in single
  // file.
  std::string dsn_value = GetValueOrDefault(section, "DSN");
  if (dsn_value.empty()) {
    dsn_value = "Default DSN";
  }

  std::string dsn_name;
  std::string email = GetValueOrDefault(section, "Email");
  std::string key_file_path = GetValueOrDefault(section, "KeyFilePath");
  std::string o_auth_mechanism =
      ConvertOAuthMechanism(GetValueOrDefault(section, "OAuthMechanism"));
  std::string catalog = GetValueOrDefault(section, "Catalog");
  std::string dataset_name = GetValueOrDefault(section, "Dataset");
  std::string encrypt_data = GetValueOrDefault(section, "EncryptData");
  std::string trusted_certs = GetValueOrDefault(section, "TrustedCerts");
  std::string min_tls_version = GetValueOrDefault(section, "Min_TLS");
  std::string description = GetValueOrDefault(section, "Description");
  std::string log_level =
      ConvertLogLevel(GetValueOrDefault(section, "LogLevel").empty()
                          ? "0"
                          : GetValueOrDefault(section, "LogLevel"));
  std::string log_file = GetValueOrDefault(section, "LogFile");
  std::string language_dialect =
      ConvertLanguageDialect(GetValueOrDefault(section, "SQLDialect"));
  std::string large_dataset_name =
      GetValueOrDefault(section, "LargeResultsDatasetId");
  std::string encryption_key = GetValueOrDefault(section, "KMSKeyName");
  std::string rows_per_block =
      GetValueOrDefault(section, "RowsFetchedPerBlock");
  std::string default_string_length =
      GetValueOrDefault(section, "DefaultStringColumnLength");
  std::string temp_expiration =
      GetValueOrDefault(section, "LargeResultsTempTableExpirationTime");
  std::string session_location = GetValueOrDefault(section, "SessionLocation");
  std::string additional_projects =
      GetValueOrDefault(section, "AdditionalProjects");
  std::string query_properties = GetValueOrDefault(section, "QueryProperties");
  std::string activation_threshold =
      GetValueOrDefault(section, "HTAPI_ActivationThreshold");
  std::string use_wchar = GetValueOrDefault(section, "UseWVarChar");
  std::string enable_session = GetValueOrDefault(section, "EnableSession");
  std::string htapi_activation_threshold_check =
      GetValueOrDefault(section, "AllowHtapiForLargeResults");
  std::string allow_large_results =
      GetValueOrDefault(section, "AllowLargeResults");
  std::string use_default_large_results_dataset =
      GetValueOrDefault(section, "UseDefaultLargeResultsDataset");

  DriverForm form;
  AdvanceOptions advance_form;

  auto CreateSectionFromForm = [&]() -> Section {
    return {
        {"Email", email},
        {"KeyFilePath", key_file_path},
        {"OAuthMechanism", o_auth_mechanism},
        {"Catalog", catalog},
        {"Dataset", dataset_name},
        {"EncryptData", encrypt_data},
        {"TrustedCerts", trusted_certs},
        {"Min_TLS", min_tls_version},
        {"Description", description},
        {"SQLDialect", language_dialect},
        {"LargeResultsDatasetId", large_dataset_name},
        {"KMSKeyName", encryption_key},
        {"RowsFetchedPerBlock", rows_per_block},
        {"DefaultStringColumnLength", default_string_length},
        {"LargeResultsTempTableExpirationTime", temp_expiration},
        {"SessionLocation", session_location},
        {"AdditionalProjects", additional_projects},
        {"QueryProperties", query_properties},
        {"HTAPI_ActivationThreshold", activation_threshold},
        {"UseWVarChar", use_wchar},
        {"EnableSession", enable_session},
        {"AllowHtapiForLargeResults", htapi_activation_threshold_check},
        {"AllowLargeResults", allow_large_results},
        {"UseDefaultLargeResultsDataset", use_default_large_results_dataset}};
  };

  auto CreateSectionFromLogForm = [&]() -> Section {
    return {{"LogLevel", log_level}, {"LogFile", log_file}};
  };

  auto ShowFormAndReturnValues = [&]() -> std::string {
    form.Show();
    form.GetHwnd();
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    dsn_name = form.GetDSN();
    email = form.GetEmail();
    key_file_path = form.GetKeyFilePath();
    o_auth_mechanism = ConvertOAuthMechanism(form.GetOAuthMechanism());
    catalog = form.GetCatalogName();
    dataset_name = form.GetDatasetName();
    encrypt_data = form.GetEncryptData();
    trusted_certs = form.GetTrustedCerts();
    min_tls_version = form.GetMinTls();
    description = form.GetDescription();
    log_level = ConvertLogLevel(form.GetLogLevel());
    log_file = form.GetLogFilePath();
    language_dialect =
        ConvertLanguageDialect(advance_form.GetLanguageDialect());
    large_dataset_name = advance_form.GetDatasetName();
    encryption_key = advance_form.GetEncryptionKey();
    rows_per_block = advance_form.GetRowsPerBlock();
    default_string_length = advance_form.GetDefaultStringLength();
    temp_expiration = advance_form.GetTempTableExpiration();
    session_location = advance_form.GetSessionLocation();
    additional_projects = advance_form.GetAdditionalProjects();
    query_properties = advance_form.GetQueryProperties();
    activation_threshold = advance_form.GetActivationThreshold();
    use_wchar = advance_form.GetUseWchar();
    enable_session = advance_form.GetEnableSession();
    htapi_activation_threshold_check =
        advance_form.GetActivationThresholdCheckbox();
    allow_large_results = advance_form.GetAllowLargeResults();
    use_default_large_results_dataset =
        advance_form.GetUseDefaultLargeResults();

    return dsn_name;
  };

  switch (f_request) {
    case ODBC_ADD_DSN: {
      if (hwnd_parent == NULL) {
        Section section = CreateSectionFromForm();
        Section trace_section = CreateSectionFromLogForm();
        AddDSNToRegistry(dsn_value, lpsz_driver, section);
        AddLogTraceToRegistry(trace_section);
        return true;
      }

      dsn_name = ShowFormAndReturnValues();
      Section section = CreateSectionFromForm();
      Section trace_section = CreateSectionFromLogForm();
      AddDSNToRegistry(dsn_name, lpsz_driver, section);
      AddLogTraceToRegistry(trace_section);
      return TRUE;
    }

    case ODBC_CONFIG_DSN: {
      if (hwnd_parent == NULL) {
        Section section_config = CreateSectionFromForm();
        EditDSNInRegistry(dsn_value, section_config);
        return true;
      }

      std::string registry_key = GetPathToOdbcIni() + "\\" + dsn_value;
      std::string driver_registry_key = GetTraceLogRegistryPath() + "\\Driver";

      auto res = GetSectionWin(registry_key);
      auto trace_result = GetSectionWin(driver_registry_key);
      auto section = res.GetValue();
      auto trace_section = trace_result.GetValue();

      (*section)["DSN"] = dsn_value;

      form.SetValues(*section);
      advance_form.SetValues(*section);
      form.SetLogTraceValues(*trace_section);
      dsn_name = ShowFormAndReturnValues();

      Section section_config = CreateSectionFromForm();
      Section trace_config_section = CreateSectionFromLogForm();
      EditDSNInRegistry(dsn_value, section_config);
      AddLogTraceToRegistry(trace_config_section);
      return TRUE;
    }
    case ODBC_REMOVE_DSN:
      RemoveDSNFromRegistry(dsn_value);
      return TRUE;

    default:
      return FALSE;
  }
}

}  // namespace google::cloud::odbc_bq_driver
