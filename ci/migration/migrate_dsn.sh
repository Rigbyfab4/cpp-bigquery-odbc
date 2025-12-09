# Migration script for Linux/MacOS ODBC DSNs
# - Migrates from "Simba Google BigQuery ODBC Connector"
# - to       "Google ODBC Driver for Google BigQuery"

# Usage:
#     migrate.sh copy   <installer.zip>
#     migrate.sh replace <installer.zip>

#!/bin/bash
set -euo pipefail

usage() {
  cat <<EOF
Usage: $0 <mode> <installer-path>
 mode:    copy    - create new DSN(s) named <orig>_google (originals untouched)
          replace - update existing Simba DSN(s) in-place (name preserved)
 installer-path: directory OR .zip OR .tar.gz/.tgz
Environment:
 ODBCINI - optional path to odbc.ini. If not set, /etc/odbc.ini is used.
EOF
  exit 1
}

if [ $# -ne 2 ]; then
  usage
fi

MODE="$1"
INSTALLER="$2"

if [ "$MODE" != "copy" ] && [ "$MODE" != "replace" ]; then
  echo "ERROR: mode must be 'copy' or 'replace'"
  exit 1
fi

# Resolve odbc.ini location
if [ -n "${ODBCINI-}" ]; then
  ODBCINI_PATH="$ODBCINI"
elif [ -f "/etc/odbc.ini" ]; then
  ODBCINI_PATH="/etc/odbc.ini"
else
  echo "ERROR: No odbc.ini found. Set ODBCINI env var or ensure /etc/odbc.ini exists."
  exit 1
fi

if [ ! -f "$ODBCINI_PATH" ]; then
  echo "ERROR: odbc.ini not found at resolved path: $ODBCINI_PATH"
  exit 1
fi

echo "Using odbc.ini: $ODBCINI_PATH"
echo "Mode: $MODE"
echo "Installer input: $INSTALLER"

WORKDIR=""
CLEANUP=0

# Extract installer
if [ -d "$INSTALLER" ]; then
  WORKDIR="$(realpath "$INSTALLER")"
else
  WORKDIR="$(mktemp -d)"
  CLEANUP=1
  case "$INSTALLER" in
    *.zip) unzip -q "$INSTALLER" -d "$WORKDIR" ;;
    *.tar.gz|*.tgz) tar -xzf "$INSTALLER" -C "$WORKDIR" ;;
    *) echo "ERROR: Installer must be directory, .zip or .tar.gz/tgz"; exit 1 ;;
  esac
fi

# Locate lib
LIBDIR=""
if [ -d "$WORKDIR/lib" ]; then
  LIBDIR="$WORKDIR/lib"
else
  LIBDIR="$(find "$WORKDIR" -type d -name lib | head -n 1 || true)"
fi

if [ -z "$LIBDIR" ] || [ ! -d "$LIBDIR" ]; then
  echo "ERROR: Could not find lib/ folder inside installer content."
  [ "$CLEANUP" -eq 1 ] && rm -rf "$WORKDIR"
  exit 1
fi

# Driver file
NEW_DRIVER_SRC="$(find "$LIBDIR" -maxdepth 1 -type f -name "*.so" -print -quit || true)"
if [ -z "$NEW_DRIVER_SRC" ]; then
  NEW_DRIVER_SRC="$(find "$LIBDIR" -maxdepth 1 -type f -name "*.dylib" -print -quit || true)"
fi

if [ -z "$NEW_DRIVER_SRC" ]; then
  echo "ERROR: No .so or .dylib found in $LIBDIR"
  [ "$CLEANUP" -eq 1 ] && rm -rf "$WORKDIR"
  exit 1
fi

echo "Found driver in installer: $NEW_DRIVER_SRC"

DEST_DIR="/usr/local/lib/google_odbc"
mkdir -p "$DEST_DIR"
DRIVER_BASENAME="$(basename "$NEW_DRIVER_SRC")"
DEST_DRIVER_PATH="$DEST_DIR/$DRIVER_BASENAME"

if [ ! -w "$DEST_DIR" ]; then
  sudo cp -f "$NEW_DRIVER_SRC" "$DEST_DRIVER_PATH"
  sudo chmod 644 "$DEST_DRIVER_PATH"
else
  cp -f "$NEW_DRIVER_SRC" "$DEST_DRIVER_PATH"
  chmod 644 "$DEST_DRIVER_PATH"
fi

echo "Driver copied to: $DEST_DRIVER_PATH"

TS="$(date +%Y%m%d%H%M%S)"
ODBC_BAK="${ODBCINI_PATH}.bak.${TS}"
cp -a "$ODBCINI_PATH" "$ODBC_BAK"
echo "Backup created: $ODBC_BAK"

# ----------------------------------------------------------------------------------------
#  MINIMAL CHANGE: Detect Simba DSNs and prompt user if >1
# ----------------------------------------------------------------------------------------
SIMBA_DSNS=()
while IFS= read -r line; do
  trimmed="$(echo "$line" | sed -e 's/^[ \t]*//' -e 's/[ \t]*$//')"
  if [[ "$trimmed" == *=*Simba\ Google\ BigQuery\ ODBC\ Connector* ]]; then
    name="${trimmed%%=*}"
    SIMBA_DSNS+=("$name")
  fi
done < <(
  awk '/^\[ODBC Data Sources\]/{flag=1; next} /^\[/{flag=0} flag{print}' "$ODBCINI_PATH"
)

if [ ${#SIMBA_DSNS[@]} -eq 0 ]; then
  echo "No Simba DSNs found. Exiting."
  exit 0
elif [ ${#SIMBA_DSNS[@]} -eq 1 ]; then
  SELECTED_DSN="${SIMBA_DSNS[0]}"
else
  echo "Multiple Simba DSNs detected:"
  i=1
  for d in "${SIMBA_DSNS[@]}"; do
    echo " $i) $d"
    i=$((i+1))
  done

  while true; do
    read -rp "Enter the number of the DSN to migrate: " choice
    if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le ${#SIMBA_DSNS[@]} ]; then
      SELECTED_DSN="${SIMBA_DSNS[$((choice-1))]}"
      break
    fi
    echo "Invalid selection. Try again."
  done
fi

echo "Selected DSN: $SELECTED_DSN"
SIMBA_DSNS=("$SELECTED_DSN")   # override with single selection
# ----------------------------------------------------------------------------------------

TMP_ODBC="$(mktemp)"
cp "$ODBCINI_PATH" "$TMP_ODBC"

# Functions (unchanged)
update_dsn_inplace() { ... }   # <-- UNCHANGED (your original code)
create_dsn_copy() { ... }      # <-- UNCHANGED (your original code)

NEW_DESC="Google ODBC Driver for Google BigQuery DSN"
NEW_DRV="$DEST_DRIVER_PATH"
NEW_DRIVER_NAME="Google ODBC Driver for Google BigQuery"

# Process selected DSN (unchanged)
for dsn in "${SIMBA_DSNS[@]}"; do
  if [ "$MODE" = "replace" ]; then
    ... # unchanged code
  else
    ... # unchanged code
  fi
done

# Write final file (unchanged)
if [ -w "$ODBCINI_PATH" ]; then
  mv "$TMP_ODBC" "$ODBCINI_PATH"
else
  sudo mv "$TMP_ODBC" "$ODBCINI_PATH"
  sudo chmod 644 "$ODBCINI_PATH"
fi

echo "Updated odbc.ini successfully."
echo "Backup: $ODBC_BAK"
echo "Done."

[ "$CLEANUP" -eq 1 ] && rm -rf "$WORKDIR"

exit 0
