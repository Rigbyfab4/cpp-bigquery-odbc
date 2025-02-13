// Copyright 2025 Google LLC
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

#include "google/cloud/odbc/bq_driver/internal/driver_adv_opt_form.h"
#include "google/cloud/odbc/bq_driver/internal/odbc_internal_commons.h"

namespace google::cloud::odbc_bq_driver_internal {
using google::cloud::odbc_bq_driver_internal::LanguageDialect;

char const AdvanceOptions::CLASS_NAME[] = "AdvanceOptClass";

std::string AdvanceOptions::activation_threshold_;

// Specifies the default SQL dialect used for queries.
// The default is "Standard SQL", but it may be overridden by the user.
std::string AdvanceOptions::language_dialect_ = "Standard SQL";
std::string AdvanceOptions::adv_dataset_name_;

// Default expiration time for temporary objects in milliseconds.
// This value (3600000 ms) corresponds to 1 hour.
// It is used as a default configuration for temporary resource cleanup
// in the existing driver, ensuring unused resources do not persist
// indefinitely.
std::string AdvanceOptions::temp_expiration_ = "3600000";
std::string AdvanceOptions::encryption_key_;

// Defines the number of rows per data block when fetching results.
// Default is 100,000 rows per block as per existing driver.
std::string AdvanceOptions::rows_per_block_ = "100000";

// Defines the default length of string columns in characters.
// The default is 16,384 characters as per existing driver.
std::string AdvanceOptions::default_string_length_ = "16384";
std::string AdvanceOptions::session_location_;
std::string AdvanceOptions::additional_projects_;
std::string AdvanceOptions::query_properties_;
std::string AdvanceOptions::use_wchar_;
std::string AdvanceOptions::enable_session_;
std::string AdvanceOptions::activation_threshold_checkbox_;
std::string AdvanceOptions::allow_large_results_;
std::string AdvanceOptions::use_default_large_results_;

std::string const kLanguageDialect = "SQLDialect";
std::string const kLargeResultsDatasetId = "LargeResultsDatasetId";
std::string const kEncryptionKey = "KMSKeyName";
std::string const kRowsFetchedPerBlock = "RowsFetchedPerBlock";
std::string const kDefaultStringColumnLength = "DefaultStringColumnLength";
std::string const kLargeResultsTempTableExpirationTime =
    "LargeResultsTempTableExpirationTime";
std::string const kSessionLocation = "SessionLocation";
std::string const kAdditionalProjects = "AdditionalProjects";
std::string const kQueryProperties = "QueryProperties";
std::string const kHTAPI_ActivationThreshold = "HTAPI_ActivationThreshold";
std::string const kUseWChar = "UseWVarChar";
std::string const kEnableSession = "EnableSession";
std::string const kHTAPIActivationThresholdCheck = "AllowHtapiForLargeResults";
std::string const kAllowLargeResults = "AllowLargeResults";
std::string const kUseDefaultLargeResultsDataset =
    "UseDefaultLargeResultsDataset";

// Control dimensions and positions
int const kHeight = 20;
int const kWidth = 50;
int const kButtonHeight = 30;
int const kButtonWidth = 80;
int const kXAxis = 20;
int const kOkButtonX = 220;
int const kCancelButtonX = 310;
int const kButtonY = 580;
int const kYAxis = 10;

HWND AdvanceOptions::GetHwnd() const { return adv_hwnd; }
AdvanceOptions::AdvanceOptions() : adv_hwnd(NULL) {}
AdvanceOptions::~AdvanceOptions() {
  if (adv_hwnd) {
    DestroyWindow(adv_hwnd);
  }
  UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
}

void AdvanceOptions::CreateLanguageControls(HFONT h_font) {
  HWND h_language_header =
      CreateLabel(adv_hwnd, "Language Dialect", kXAxis, kYAxis, kWidth * 3,
                  kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_language_combo_box =
      CreateComboBox(adv_hwnd, kXAxis * 10, kYAxis, kWidth * 3, kHeight * 6,
                     kIdcLanguageDialectComboBox);
  SendMessage(h_language_combo_box, WM_SETFONT, (WPARAM)h_font, TRUE);
  SendMessage(h_language_combo_box, CB_ADDSTRING, 0, (LPARAM) "Standard SQL");
  SendMessage(h_language_combo_box, CB_ADDSTRING, 0, (LPARAM) "Legacy SQL");
  SendMessage(h_language_combo_box, CB_SETCURSEL, 0, 0);
  SetWindowText(h_language_combo_box, language_dialect_.c_str());
}

void AdvanceOptions::CreateLargeResultsControls(HFONT h_font) {
  HWND h_large_results_header =
      CreateLabel(adv_hwnd, "Large Results Options", kXAxis, kYAxis * 5,
                  kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT | SS_NOPREFIX);
  HWND h_allow_large_results_checkbox =
      CreateCheckBox(adv_hwnd, "Allow Large Result Sets", kXAxis, kYAxis * 8,
                     kWidth * 6, kHeight, kIdcAllowLargeResultsCheckbox);
  EnableWindow(h_allow_large_results_checkbox, FALSE);
  CheckDlgButton(adv_hwnd, kIdcAllowLargeResultsCheckbox,
                 (allow_large_results_ == "1") ? BST_CHECKED : BST_UNCHECKED);

  HWND h_use_default_checkbox = CreateCheckBox(
      adv_hwnd, "Use Default \"_bqodbc_temp_tables\" Dataset", kXAxis * 2,
      kYAxis * 11, kWidth * 6 + 20, kHeight, kIdcUseDefaultCheckbox);
  CheckDlgButton(
      adv_hwnd, kIdcUseDefaultCheckbox,
      (use_default_large_results_ == "1") ? BST_CHECKED : BST_UNCHECKED);

  HWND h_dataset_name_label =
      CreateLabel(adv_hwnd, "Dataset Name for Large Result Sets:", kXAxis * 2,
                  kYAxis * 14, kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_dataset_name_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 14, kWidth * 2, kHeight,
                    kIdcDatasetNameEdit);
  SetWindowText(h_dataset_name_edit, adv_dataset_name_.c_str());

  HWND h_temp_expiration_label = CreateLabel(
      adv_hwnd, "Default temp table expiration time(ms):", kXAxis * 2,
      kYAxis * 17, kWidth * 5 + 20, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_temp_expiration_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 17, kWidth * 2, kHeight,
                    kIdcTempExpirationEdit);
  SetWindowText(h_temp_expiration_edit, temp_expiration_.c_str());
}

void AdvanceOptions::CreateHighThroughputControls(HFONT h_font) {
  HWND h_allow_high_throughput_checkbox = CreateCheckBox(
      adv_hwnd, "Allow High-Throughput API for Large Results queries:", kXAxis,
      kYAxis * 20, kWidth * 7 + 20, kHeight, kIdcAllowHighThroughputCheckbox);
  CheckDlgButton(
      adv_hwnd, kIdcAllowHighThroughputCheckbox,
      (activation_threshold_checkbox_ == "1") ? BST_CHECKED : BST_UNCHECKED);

  HWND h_high_throughput_header =
      CreateLabel(adv_hwnd, "High-Throughput API Options", kXAxis, kYAxis * 23,
                  kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT);

  HWND h_activation_threshold_label =
      CreateLabel(adv_hwnd, "Activation Threshold for High-Throughput:", kXAxis,
                  kYAxis * 26, kWidth * 6, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_activation_threshold_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 26, kWidth * 2, kHeight,
                    kIdcActivationThresholdEdit);
}

void AdvanceOptions::CreateEncryptionControls(HFONT h_font) {
  HWND h_encryption_key_header =
      CreateLabel(adv_hwnd, "Customer-Managed Encryption Key:", kXAxis,
                  kYAxis * 29, kWidth * 6, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_encryption_key_edit =
      CreateEditBox(adv_hwnd, kXAxis, kYAxis * 32, kWidth * 6 + 30, kHeight,
                    kIdcEncryptionKeyEdit);
  SetWindowText(h_encryption_key_edit, encryption_key_.c_str());
}

void AdvanceOptions::CreateSessionControls(HFONT h_font) {
  HWND h_rows_per_block_label =
      CreateLabel(adv_hwnd, "Rows Per Block:", kXAxis, kYAxis * 35, kWidth * 3,
                  kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_rows_per_block_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 35, kWidth * 2, kHeight,
                    kIdcRowsPerBlockEdit);
  SetWindowText(h_rows_per_block_edit, rows_per_block_.c_str());

  HWND h_default_string_label =
      CreateLabel(adv_hwnd, "Default String Column Length:", kXAxis,
                  kYAxis * 38, kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_default_string_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 38, kWidth * 2, kHeight,
                    kIdcDefaultStringEdit);
  SetWindowText(h_default_string_edit, default_string_length_.c_str());

  HWND h_enable_session_checkbox =
      CreateCheckBox(adv_hwnd, "Enable Session", kXAxis, kYAxis * 41,
                     kWidth * 2 + 30, kHeight, kIdcEnableSessionCheckbox);
  CheckDlgButton(adv_hwnd, kIdcEnableSessionCheckbox,
                 (enable_session_ == "1") ? BST_CHECKED : BST_UNCHECKED);

  HWND h_session_location_label =
      CreateLabel(adv_hwnd, "Session Location:", kXAxis * 9, kYAxis * 41,
                  kWidth * 2 + 30, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_session_location_edit =
      CreateEditBox(adv_hwnd, kXAxis * 15, kYAxis * 41, kWidth * 2, kHeight,
                    kIdcSessionLocationEdit);
  EnableWindow(h_session_location_edit, FALSE);
  SetWindowText(h_session_location_edit, session_location_.c_str());
}

void AdvanceOptions::CreateAdditionalControls(HFONT h_font) {
  HWND h_variables_checkbox = CreateCheckBox(
      adv_hwnd, "Use SQL_WVARCHAR instead of SQL_VARCHAR", kXAxis, kYAxis * 44,
      kWidth * 7, kHeight, kIdcVariableCheckbox);
  CheckDlgButton(adv_hwnd, kIdcVariableCheckbox,
                 (use_wchar_ == "1") ? BST_CHECKED : BST_UNCHECKED);

  HWND h_additional_projects_label =
      CreateLabel(adv_hwnd, "Additional Projects:", kXAxis, kYAxis * 47,
                  kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_additional_projects_edit =
      CreateScrollableEditBox(adv_hwnd, kXAxis, kYAxis * 49, kWidth * 7 + 30,
                              kHeight, kIdcAdditionalProjectsEdit);
  SetWindowText(h_additional_projects_edit, additional_projects_.c_str());

  HWND h_query_properties_label =
      CreateLabel(adv_hwnd, "Query Properties:", kXAxis, kYAxis * 52,
                  kWidth * 5, kHeight, WS_VISIBLE | SS_LEFT);
  HWND h_query_properties_edit =
      CreateScrollableEditBox(adv_hwnd, kXAxis, kYAxis * 55, kWidth * 7 + 30,
                              kHeight, kIdcQueryPropertiesEdit);
  SetWindowText(h_query_properties_edit, query_properties_.c_str());
}

void AdvanceOptions::CreateButtons(HFONT h_font) {
  HWND h_ok_button = CreateButton(adv_hwnd, "OK", kOkButtonX, kButtonY,
                                  kButtonWidth, kButtonHeight, kIdcOKButton);
  SendMessage(h_ok_button, WM_SETFONT, (WPARAM)h_font, TRUE);

  HWND h_cancel_button =
      CreateButton(adv_hwnd, "Cancel", kCancelButtonX, kButtonY, kButtonWidth,
                   kButtonHeight, kIdcCancelButton);
  SendMessage(h_cancel_button, WM_SETFONT, (WPARAM)h_font, TRUE);
}

LRESULT CALLBACK AdvanceOptions::AdvanceOptProc(HWND hwnd, UINT u_msg,
                                                WPARAM w_param,
                                                LPARAM l_param) {
  AdvanceOptions* p_current_window = NULL;
  if (u_msg == WM_NCCREATE) {
    CREATESTRUCT* pCreate = (CREATESTRUCT*)l_param;
    p_current_window = (AdvanceOptions*)pCreate->lpCreateParams;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)p_current_window);
  } else {
    p_current_window = (AdvanceOptions*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  }
  switch (u_msg) {
    case WM_COMMAND:
      switch (LOWORD(w_param)) {
        case kIdcOKButton: {
          HWND h_language_box = GetDlgItem(hwnd, kIdcLanguageDialectComboBox);
          char language_buffer[256] = {0};
          GetWindowText(h_language_box, language_buffer,
                        sizeof(language_buffer));
          language_dialect_ = language_buffer;

          HWND h_dataset_name_edit = GetDlgItem(hwnd, kIdcDatasetNameEdit);
          char dataset_name_buffer[256] = {0};
          GetWindowText(h_dataset_name_edit, dataset_name_buffer,
                        sizeof(dataset_name_buffer));
          if (!IsDlgButtonChecked(hwnd, kIdcUseDefaultCheckbox)) {
            adv_dataset_name_ = dataset_name_buffer;
          } else {
            adv_dataset_name_ = "";
          }

          HWND h_temp_expiration_edit =
              GetDlgItem(hwnd, kIdcTempExpirationEdit);
          char temp_expiration_buffer[256] = {0};
          GetWindowText(h_temp_expiration_edit, temp_expiration_buffer,
                        sizeof(temp_expiration_buffer));
          temp_expiration_ = temp_expiration_buffer;

          HWND h_encryption_key_edit = GetDlgItem(hwnd, kIdcEncryptionKeyEdit);
          char encryption_key_buffer[256] = {0};
          GetWindowText(h_encryption_key_edit, encryption_key_buffer,
                        sizeof(encryption_key_buffer));
          encryption_key_ = encryption_key_buffer;

          HWND h_rows_per_block_edit = GetDlgItem(hwnd, kIdcRowsPerBlockEdit);
          char rows_per_block_buffer[256] = {0};
          GetWindowText(h_rows_per_block_edit, rows_per_block_buffer,
                        sizeof(rows_per_block_buffer));
          rows_per_block_ = rows_per_block_buffer;

          HWND h_default_string_edit = GetDlgItem(hwnd, kIdcDefaultStringEdit);
          char default_string_buffer[256] = {0};
          GetWindowText(h_default_string_edit, default_string_buffer,
                        sizeof(default_string_buffer));
          default_string_length_ = default_string_buffer;

          HWND h_session_location_edit =
              GetDlgItem(hwnd, kIdcSessionLocationEdit);
          char session_location_buffer[256] = {0};
          GetWindowText(h_session_location_edit, session_location_buffer,
                        sizeof(session_location_buffer));
          if (IsDlgButtonChecked(hwnd, kIdcEnableSessionCheckbox)) {
            session_location_ = session_location_buffer;
          } else {
            session_location_ = "";
          }

          HWND h_additional_projects_edit =
              GetDlgItem(hwnd, kIdcAdditionalProjectsEdit);
          char additional_projects_buffer[1024] = {0};
          GetWindowText(h_additional_projects_edit, additional_projects_buffer,
                        sizeof(additional_projects_buffer));
          additional_projects_ = additional_projects_buffer;

          HWND h_query_properties_edit =
              GetDlgItem(hwnd, kIdcQueryPropertiesEdit);
          char query_properties_buffer[1024] = {0};
          GetWindowText(h_query_properties_edit, query_properties_buffer,
                        sizeof(query_properties_buffer));
          query_properties_ = query_properties_buffer;

          HWND h_activation_threshold =
              GetDlgItem(hwnd, kIdcActivationThresholdEdit);
          char activation_threshold_buffer[1024] = {0};
          GetWindowText(h_activation_threshold, activation_threshold_buffer,
                        sizeof(activation_threshold_buffer));
          activation_threshold_ = activation_threshold_buffer;

          use_wchar_ =
              (IsDlgButtonChecked(hwnd, kIdcVariableCheckbox) == BST_CHECKED)
                  ? "1"
                  : "0";

          enable_session_ =
              (IsDlgButtonChecked(hwnd, kIdcEnableSessionCheckbox) ==
               BST_CHECKED)
                  ? "1"
                  : "0";

          activation_threshold_checkbox_ =
              (IsDlgButtonChecked(hwnd, kIdcAllowHighThroughputCheckbox) ==
               BST_CHECKED)
                  ? "1"
                  : "0";

          allow_large_results_ =
              (IsDlgButtonChecked(hwnd, kIdcAllowLargeResultsCheckbox) ==
               BST_CHECKED)
                  ? "1"
                  : "0";

          use_default_large_results_ =
              (IsDlgButtonChecked(hwnd, kIdcUseDefaultCheckbox) == BST_CHECKED)
                  ? "1"
                  : "0";
          DestroyWindow(hwnd);
          break;
        }
        case kIdcUseDefaultCheckbox: {
          if (HIWORD(w_param) == BN_CLICKED) {
            BOOL is_checked =
                (IsDlgButtonChecked(hwnd, kIdcUseDefaultCheckbox) ==
                 BST_CHECKED);
            EnableWindow(GetDlgItem(hwnd, kIdcDatasetNameEdit), !is_checked);
          }
          break;
        }
        case kIdcEnableSessionCheckbox: {
          if (HIWORD(w_param) == BN_CLICKED) {
            BOOL is_checked =
                (IsDlgButtonChecked(hwnd, kIdcEnableSessionCheckbox) ==
                 BST_CHECKED);
            EnableWindow(GetDlgItem(hwnd, kIdcSessionLocationEdit), is_checked);
          }
          break;
        }
        case kIdcLanguageDialectComboBox: {
          if (HIWORD(w_param) == CBN_SELCHANGE) {
            HWND h_language_box = GetDlgItem(hwnd, kIdcLanguageDialectComboBox);
            char language_buffer[256] = {0};

            int index = SendMessage(h_language_box, CB_GETCURSEL, 0, 0);
            if (index != CB_ERR) {
              SendMessage(h_language_box, CB_GETLBTEXT, index,
                          (LPARAM)language_buffer);

              HWND h_checkbox = GetDlgItem(hwnd, kIdcAllowLargeResultsCheckbox);
              if (strcmp(language_buffer, "Legacy SQL") == 0) {
                EnableWindow(h_checkbox, TRUE);
              } else {
                EnableWindow(h_checkbox, FALSE);
                CheckDlgButton(hwnd, kIdcAllowLargeResultsCheckbox,
                               BST_UNCHECKED);
              }
            }
          }
          break;
        }
        case kIdcCancelButton:
          DestroyWindow(hwnd);  // Close the window
          break;
      }
      break;

    case WM_CLOSE:
      DestroyWindow(hwnd);  // Close the window
      return 0;

    case WM_DESTROY:
      if (p_current_window) {
        p_current_window->adv_hwnd = NULL;
      }
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, u_msg, w_param, l_param);
}

void AdvanceOptions::SetValues(Section const& attribute_map) {
  language_dialect_ = GetValueOrDefault(attribute_map, kLanguageDialect);
  std::string lang_dialect_value =
      GetValueOrDefault(attribute_map, kLanguageDialect);
  if (lang_dialect_value ==
      std::to_string(static_cast<int>(LanguageDialect::kStandardSQL))) {
    language_dialect_ = "Standard SQL";
  } else if (lang_dialect_value ==
             std::to_string(static_cast<int>(LanguageDialect::kLegacySQL))) {
    language_dialect_ = "Legacy SQL";
  } else {
    language_dialect_ = "";
  }
  adv_dataset_name_ = GetValueOrDefault(attribute_map, kLargeResultsDatasetId);
  encryption_key_ = GetValueOrDefault(attribute_map, kEncryptionKey);
  rows_per_block_ = GetValueOrDefault(attribute_map, kRowsFetchedPerBlock);
  default_string_length_ =
      GetValueOrDefault(attribute_map, kDefaultStringColumnLength);
  temp_expiration_ =
      GetValueOrDefault(attribute_map, kLargeResultsTempTableExpirationTime);
  session_location_ = GetValueOrDefault(attribute_map, kSessionLocation);
  additional_projects_ = GetValueOrDefault(attribute_map, kAdditionalProjects);
  query_properties_ = GetValueOrDefault(attribute_map, kQueryProperties);
  activation_threshold_ =
      GetValueOrDefault(attribute_map, kHTAPI_ActivationThreshold);
  use_wchar_ = GetValueOrDefault(attribute_map, kUseWChar);
  enable_session_ = GetValueOrDefault(attribute_map, kSessionLocation);
  activation_threshold_checkbox_ =
      GetValueOrDefault(attribute_map, kHTAPIActivationThresholdCheck);
  allow_large_results_ = GetValueOrDefault(attribute_map, kAllowLargeResults);
  use_default_large_results_ =
      GetValueOrDefault(attribute_map, kUseDefaultLargeResultsDataset);
}

void AdvanceOptions::Show(HWND hwnd) {
  if (adv_hwnd) {
    ShowWindow(adv_hwnd, SW_SHOW);
    SetForegroundWindow(adv_hwnd);
    return;
  }

  WNDCLASS wc_adv = {};
  wc_adv.lpfnWndProc = AdvanceOptions::AdvanceOptProc;
  wc_adv.hInstance = GetModuleHandle(NULL);
  wc_adv.lpszClassName = CLASS_NAME;
  wc_adv.hbrBackground =
      (HBRUSH)(COLOR_WINDOW + 1);  // Sets background to white

  RegisterClass(&wc_adv);

  int window_width = 420;
  int window_height = 650;
  int screen_width = GetSystemMetrics(SM_CXSCREEN);
  int screen_height = GetSystemMetrics(SM_CYSCREEN);
  int x_pos = (screen_width - window_width) / 2;
  int y_pos = (screen_height - window_height) / 2;

  adv_hwnd = CreateWindowEx(
      0, CLASS_NAME, "Advanced Options", WS_OVERLAPPEDWINDOW, x_pos, y_pos,
      window_width, window_height, hwnd, NULL, GetModuleHandle(NULL), this);

  if (adv_hwnd) {
    HFONT h_font =
        CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_SWISS, "Segoe UI");

    CreateLanguageControls(h_font);
    CreateLargeResultsControls(h_font);
    CreateHighThroughputControls(h_font);
    CreateEncryptionControls(h_font);
    CreateSessionControls(h_font);
    CreateAdditionalControls(h_font);
    CreateButtons(h_font);
  }
  ShowWindow(adv_hwnd, SW_SHOW);
  UpdateWindow(adv_hwnd);
}
}  // namespace google::cloud::odbc_bq_driver_internal
