#!/bin/sh
# Gera build/relatorio-overleaf.zip com o relatorio/ do último commit, na raiz do zip,
# pronto para "New Project > Upload Project" no Overleaf.
# Usa git archive (só precisa do git) para o zip ser sempre um estado que compila.
set -eu

repo="$(cd "$(dirname "$0")/.." && pwd)"
out="$repo/build/relatorio-overleaf.zip"

if [ -n "$(git -C "$repo" status --porcelain -- relatorio)" ]; then
  echo "Aviso: mudanças não commitadas em relatorio/ ficam fora do zip:" >&2
  git -C "$repo" status --short -- relatorio >&2
fi

mkdir -p "$repo/build"
git -C "$repo" archive --format=zip -o "$out" HEAD:relatorio
echo "$out"
