#!/usr/bin/env bash
# Gera build/relatorio-overleaf.zip com o conteúdo de relatorio/ na raiz do zip,
# pronto para "New Project > Upload Project" no Overleaf.
set -euo pipefail

repo="$(cd "$(dirname "$0")/.." && pwd)"
out="$repo/build/relatorio-overleaf.zip"

mkdir -p "$repo/build"
rm -f "$out"

cd "$repo/relatorio"
git -C "$repo" ls-files -z -- relatorio \
  | sed -z 's|^relatorio/||' \
  | xargs -0 zip -q "$out"

echo "$out"
