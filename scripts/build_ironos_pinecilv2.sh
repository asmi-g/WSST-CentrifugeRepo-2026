#!/usr/bin/env bash
set -euo pipefail

LANG_CODE="${1:-EN}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IRONOS="$ROOT/IronOS"
OUTDIR="$ROOT/out/ironos"
HEXDIR="$IRONOS/source/Hexfile"

mkdir -p "$OUTDIR"

# Build inside docker container
docker compose -f "$IRONOS/Env.yml" run --rm builder sh -lc \
  "cd source && ./build.sh -l '$LANG_CODE' -m Pinecilv2" \

# Copy the resulting bin out
BIN="$(ls -1 "$HEXDIR" 2>/dev/null | grep -E "^Pinecilv2_.*${LANG_CODE}\.bin$" | head -n 1 || true)"
if [[ -z "$BIN" ]]; then
  BIN="$(ls -1 "$HEXDIR" 2>/dev/null | grep -E "^Pinecilv2_.*\.bin$" | head -n 1 || true)"
fi

[[ -n "$BIN" ]] || { echo "ERROR: No Pinecilv2 .bin found in $HEXDIR"; ls -la "$HEXDIR" || true; exit 1; }

cp -f "$HEXDIR/$BIN" "$OUTDIR/$BIN"
echo "Built IronOS -> $OUTDIR/$BIN"
