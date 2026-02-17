#!/usr/bin/env bash
set -euo pipefail

LANG_CODE="${1:-EN}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IRONOS="$ROOT/IronOS"
OUTDIR="$ROOT/out/ironos"

if [[ ! -d "$IRONOS" ]]; then
  echo "ERROR: IronOS not found at $IRONOS"
  exit 1
fi

mkdir -p "$OUTDIR"

# 1) Start/ensure docker dev environment (as per IronOS docs)
# (deploy.sh may build the docker image & start a container)
"$IRONOS/scripts/deploy.sh"

# 2) Build Pinecilv2 firmware (as per IronOS docs)
pushd "$IRONOS/source" >/dev/null
./build.sh -l "$LANG_CODE" -m Pinecilv2
popd >/dev/null

# 3) Copy the produced BIN from IronOS output directory into out/ironos/
# IronOS typically writes under /IronOS/source/Hexfile/
HEXDIR="$IRONOS/source/Hexfile"

# Find a Pinecilv2 bin matching language; fall back to any Pinecilv2 bin
BIN_CANDIDATE="$(ls -1 "$HEXDIR" 2>/dev/null | grep -E "^Pinecilv2_.*${LANG_CODE}\.bin$" | head -n 1 || true)"
if [[ -z "$BIN_CANDIDATE" ]]; then
  BIN_CANDIDATE="$(ls -1 "$HEXDIR" 2>/dev/null | grep -E "^Pinecilv2_.*\.bin$" | head -n 1 || true)"
fi

if [[ -z "$BIN_CANDIDATE" || ! -f "$HEXDIR/$BIN_CANDIDATE" ]]; then
  echo "ERROR: Could not find Pinecilv2 .bin in $HEXDIR"
  echo "Contents:"
  ls -la "$HEXDIR" || true
  exit 1
fi

cp -f "$HEXDIR/$BIN_CANDIDATE" "$OUTDIR/$BIN_CANDIDATE"
echo "Built IronOS -> $OUTDIR/$BIN_CANDIDATE"
