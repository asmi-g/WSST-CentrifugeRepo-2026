#!/usr/bin/env bash
set -euo pipefail

FW_BIN="${1:?Usage: $0 path/to/Pinecilv2_*.bin}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BLISP="$ROOT/tools/bin/blisp"

if [[ ! -f "$FW_BIN" ]]; then
  echo "ERROR: firmware bin not found: $FW_BIN"
  exit 1
fi

# Ensure blisp exists
if [[ ! -x "$BLISP" ]]; then
  echo "blisp not found"
fi

BLISP_DIR="$(dirname "$BLISP")"
FW_BASE="$(basename "$FW_BIN")"

# Pine64 flow: bin in same folder as blisp
cp -f "$FW_BIN" "$BLISP_DIR/$FW_BASE"

pushd "$BLISP_DIR" >/dev/null
echo "Flashing: $FW_BASE"
sudo ./blisp write -c bl70x --reset "$FW_BASE"
popd >/dev/null
