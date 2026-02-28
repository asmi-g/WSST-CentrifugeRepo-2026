#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./scripts/flash_pinecilv2.sh
#     - flashes firmware/current/Pinecilv2_EN.bin if present, otherwise fallback
#
#   ./scripts/flash_pinecilv2.sh path/to/Pinecilv2_EN.bin
#     - flashes the given file directly
#
# Notes:
# - Requires blisp already built at: tools/bin/blisp (see docs/ironOSBuildFlash.md)
# - Copies the .bin next to blisp before flashing (as per Pine64 instructions)

FW_BIN="${1:-}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BLISP="$ROOT/tools/bin/blisp"

CURRENT="$ROOT/firmware/ironos/current/Pinecilv2_EN.bin"
FALLBACK="$ROOT/firmware/ironos/fallback/Pinecilv2_EN.bin"

die() { echo "ERROR: $*" >&2; exit 1; }

# Check blisp exists
if [[ ! -x "$BLISP" ]]; then
  echo "ERROR: blisp not found or not executable at: $BLISP"
  echo "Build blisp once on this machine (see docs/ironOSBuildFlash.md)."
  exit 1
fi

choose_firmware() {
  # provided path to bin, use it directly
  if [[ -n "$FW_BIN" ]]; then
    [[ -f "$FW_BIN" ]] || die "firmware bin not found: $FW_BIN"
    echo "$FW_BIN"
    return 0
  fi

  # flash current tracked firmware, else fallback
  if [[ -f "$CURRENT" ]]; then
    echo "$CURRENT"
    return 0
  fi

  echo "Current firmware not found; using fallback."
  [[ -f "$FALLBACK" ]] || die "fallback firmware not found: $FALLBACK"
  echo "$FALLBACK"
}

USE_BIN="$(choose_firmware)"
echo "Using firmware: $USE_BIN"

BLISP_DIR="$(dirname "$BLISP")"
FW_BASE="$(basename "$USE_BIN")"

# copies our .bin to be in the same folder as blisp
cp -f "$USE_BIN" "$BLISP_DIR/$FW_BASE"

cd "$BLISP_DIR"
echo "Flashing $FW_BASE ..."
sudo ./blisp write -c bl70x --reset "$FW_BASE"
