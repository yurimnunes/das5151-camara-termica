#!/usr/bin/env python3
"""Resumo de um ou mais logs de dados/: mediana, mínimo e máximo de cada coluna.

Uso: python3 scripts/resumo_log.py dados/2026-11-26_*_nos-5.tsv [...]
"""
import statistics
import sys


def resumo(path: str) -> None:
    header, rows = None, []
    # Logs do monitor_serial.py têm o horário do PC na 1ª coluna; logs copiados
    # do Serial Monitor da IDE não. O cabeçalho diz qual é o caso.
    skip = 1
    with open(path, encoding="utf-8") as f:
        for line in f:
            cols = line.rstrip("\n").split("\t")
            if "tempo_s" in cols[:2]:
                skip = cols.index("tempo_s")
                header = cols[skip:]
                continue
            parts = cols[skip:]
            if not parts or parts[0].startswith("#"):
                continue
            if header and len(parts) == len(header) and parts[0].isdigit():
                rows.append(parts)
    print(f"== {path}  ({len(rows)} leituras)")
    if not rows:
        return
    for i, name in enumerate(header[1:], start=1):
        vals = []
        for r in rows:
            try:
                vals.append(float(r[i]))
            except ValueError:
                pass
        if vals:
            print(f"  {name:12s} mediana={statistics.median(vals):8.3f}  min={min(vals):8.3f}  max={max(vals):8.3f}")
        else:
            print(f"  {name:12s} sem valores numéricos ({rows[0][i]})")


if __name__ == "__main__":
    for p in sys.argv[1:]:
        resumo(p)
