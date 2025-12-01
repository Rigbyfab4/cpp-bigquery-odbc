# Migration script for Linux/MacOS ODBC DSNs
# - Migrates from "Simba Google BigQuery ODBC Connector"
#- to       "Google ODBC Driver for Google BigQuery"

# Usage:
#     migrate.ps1 -Mode copy -DriverInstaller "GoogleBigQueryODBC.msi"
#     migrate.ps1 -Mode replace -DriverInstaller "GoogleBigQueryODBC.msi"
#

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

# Resolve odbc.ini location (env first, then /etc/odbc.ini)
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

# Prepare installer extraction if necessary
if [ -d "$INSTALLER" ]; then
  echo "Installer is a directory: $INSTALLER"
  WORKDIR="$(realpath "$INSTALLER")"
else
  WORKDIR="$(mktemp -d)"
  CLEANUP=1
  case "$INSTALLER" in
    *.zip)
      if ! command -v unzip >/dev/null 2>&1; then
        echo "ERROR: unzip is required to extract zip archives."
        exit 1
      fi
      unzip -q "$INSTALLER" -d "$WORKDIR"
      ;;
    *.tar.gz|*.tgz)
      tar -xzf "$INSTALLER" -C "$WORKDIR"
      ;;
    *)
      echo "ERROR: Installer must be a directory, .zip, or .tar.gz/.tgz"
      exit 1
      ;;
  esac
fi

# Find lib/ folder in extracted tree
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

# Find driver file (prefer .so, else .dylib)
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
  echo "Copying driver to $DEST_DIR with sudo..."
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
echo "Backup of odbc.ini saved to: $ODBC_BAK"

# Detect Simba DSNs in [ODBC Data Sources]
SIMBA_DSNS=()
while IFS= read -r line; do
  trimmed="$(echo "$line" | sed -e 's/^[ \t]*//' -e 's/[ \t]*$//')"
  if [[ "$trimmed" == *=*Simba\ Google\ BigQuery\ ODBC\ Connector* ]]; then
    name="${trimmed%%=*}"
    SIMBA_DSNS+=("$name")
  fi
done < <(awk '/^\[ODBC Data Sources\]/{flag=1; next} /^\[/{flag=0} flag{print}' "$ODBCINI_PATH" || true)

if [ ${#SIMBA_DSNS[@]} -eq 0 ]; then
  echo "No DSN entries found in [ODBC Data Sources] that point to 'Simba Google BigQuery ODBC Connector'. Exiting."
  [ "$CLEANUP" -eq 1 ] && rm -rf "$WORKDIR"
  exit 0
fi

echo "Detected Simba DSN(s): ${SIMBA_DSNS[*]}"

TMP_ODBC="$(mktemp)"
cp "$ODBCINI_PATH" "$TMP_ODBC"

# Replace driver & description inside a DSN section
update_dsn_inplace() {
  target="$1"
  newdrv="$2"
  newdesc="$3"
  awk -v section="$target" -v drv="$newdrv" -v desc="$newdesc" '
  BEGIN { in_section=0 }
  {
    if ($0 ~ ("^\\[" section "\\]$")) { print; in_section=1; next }
    if ($0 ~ /^\[/ && in_section==1) { in_section=0 }
    if (in_section==1) {
      if ($0 ~ /^Description=/) { print "Description=" desc; next }
      if ($0 ~ /^Driver=/) { print "Driver=" drv; next }
    }
    print
  }
  ' "$TMP_ODBC" > "${TMP_ODBC}.new" && mv "${TMP_ODBC}.new" "$TMP_ODBC"
}

create_dsn_copy() {
  orig="$1"
  copyname="${orig}_google"
  newdrv="$2"
  newdesc="$3"

  if awk -v s="[$copyname]" 'BEGIN{found=0} $0==s{found=1} END{exit !found}' "$TMP_ODBC"; then
    echo "Note: $copyname already exists; skipping creation."
    return
  fi

  awk -v s="[$orig]" 'BEGIN{in=0} {if($0==s){in=1; print; next} if(in==1 && $0~/^\[/){exit} if(in==1) print}' "$TMP_ODBC" > "${TMP_ODBC}.${orig}.section" || true

  if [ ! -s "${TMP_ODBC}.${orig}.section" ]; then
    echo "Warning: Original section [$orig] not found; creating minimal section for $copyname"
    {
      echo "[$copyname]"
      echo "Description=$newdesc"
      echo "Driver=$newdrv"
    } >> "$TMP_ODBC"
    return
  fi

  awk -v orig="$orig" -v copy="$copyname" -v drv="$newdrv" -v desc="$newdesc" '
  NR==1{if($0 ~ ("^\\[" orig "\\]$")){print "[" copy "]"} else {print}; next}
  {if($0 ~ /^Description=/){print "Description=" desc; next} if($0 ~ /^Driver=/){print "Driver=" drv; next} print}' "${TMP_ODBC}.${orig}.section" >> "$TMP_ODBC"

  rm -f "${TMP_ODBC}.${orig}.section"
  echo "Created DSN copy: [$copyname]"
}

NEW_DESC="Google ODBC Driver for Google BigQuery DSN"
NEW_DRV="$DEST_DRIVER_PATH"
NEW_DRIVER_NAME="Google ODBC Driver for Google BigQuery"

# Update DSNs
for dsn in "${SIMBA_DSNS[@]}"; do
  if [ "$MODE" = "replace" ]; then
    awk -v dsn="$dsn" -v newname="$NEW_DRIVER_NAME" '
    BEGIN { in_ods=0 }
    {
      if ($0 ~ /^\[ODBC Data Sources\]/){ print; in_ods=1; next }
      if ($0 ~ /^\[/ && $0 !~ /^\[ODBC Data Sources\]/){ in_ods=0 }
      if(in_ods==1){ if($0 ~ ("^"dsn"[ \t]*=")){ print dsn "=" newname; next } }
      print
    }' "$TMP_ODBC" > "${TMP_ODBC}.tmp" && mv "${TMP_ODBC}.tmp" "$TMP_ODBC"

    update_dsn_inplace "$dsn" "$NEW_DRV" "$NEW_DESC"
    echo "Replaced DSN: [$dsn] -> updated Description & Driver"
  else
    create_dsn_copy "$dsn" "$NEW_DRV" "$NEW_DESC"
    newdsn="${dsn}_google"
    awk -v original="$dsn" -v addeddsn="$newdsn" -v newname="$NEW_DRIVER_NAME" '
    BEGIN{ in_ods=0; printed=0 }
    { if($0 ~ /^\[ODBC Data Sources\]/){ print; in_ods=1; next }
      if($0 ~ /^\[/ && $0 !~ /^\[ODBC Data Sources\]/){ if(in_ods==1 && printed==0){ print addeddsn "=" newname; printed=1 } in_ods=0 }
      if(in_ods==1){ print; next }
      print
    }
    END{ if(in_ods==1 && printed==0) print addeddsn "=" newname }' "$TMP_ODBC" > "${TMP_ODBC}.tmp" && mv "${TMP_ODBC}.tmp" "$TMP_ODBC"
    echo "Created copy DSN: [$newdsn]"
  fi
done

if [ -w "$ODBCINI_PATH" ]; then
  mv "$TMP_ODBC" "$ODBCINI_PATH"
else
  echo "Updating $ODBCINI_PATH with sudo..."
  sudo mv "$TMP_ODBC" "$ODBCINI_PATH"
  sudo chown root:root "$ODBCINI_PATH" || true
  sudo chmod 644 "$ODBCINI_PATH" || true
fi

echo "Updated odbc.ini successfully: $ODBCINI_PATH"
echo "Backup retained at: $ODBC_BAK"
echo "Driver in use: $NEW_DRV"
echo "Mode finished: $MODE"

[ "$CLEANUP" -eq 1 ] && rm -rf "$WORKDIR"

exit 0
