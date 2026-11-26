# Dados dos ensaios

Cada arquivo `.tsv` é um log do `scripts/monitor_serial.py`. Primeira coluna: horário do PC. O resto é a linha que o Arduino mandou; comandos enviados aparecem como `# comando (...)`.

**Atenção com o `firmware/teste_nos`:** os nomes das colunas (`A1_T`, `A2_F`, ...) são fixos no firmware, mas os jumpers mudaram de lugar entre os ensaios. **Use a tabela abaixo para saber o que cada canal media em cada log.** A coluna `a_b_mV` é sempre A3 − A4 e só tem significado quando A3 e A4 estavam nos nós a e b.

Resumo rápido de qualquer log (mediana, mínimo e máximo de cada coluna):

```sh
python3 scripts/resumo_log.py dados/<arquivo>.tsv
```

## Índice

Ensaio 01, sensores (2026-11-26). Relatório: [`docs/ensaios/01-sensores-2026-11-26.md`](../docs/ensaios/01-sensores-2026-11-26.md). "solto" = canal sem jumper (valor flutuando, ignorar).

### Firmware `teste_sensores` (A0 = saída do filtro, D2 = DS18B20)

| Log | Situação | Resultado (mediana) |
|---|---|---|
| `001655_sensores` | primeira montagem | DS18B20 25,81 °C ✅; A0 0,024 V ❌ |
| `004334_sensores-2` | após revisar o Sense (pino 11) | igual: A0 0,024 V |
| `004916_checagem`, `004934_checagem-2` | cabo do DS18B20 religado no pino errado | DS18B20 encontrados: 0 |
| `005120_checagem-3` | DS18B20 de volta no D2 | DS18B20 25,87 °C ✅ |

### Firmware `teste_nos` (Arduino como voltímetro)

| Log | A1 | A2 | A3 | A4 | A5 | Resultado |
|---|---|---|---|---|---|---|
| `005618_nos-1` | solto | solto | solto | solto | INA 11 (Sense) | VO 0,024 V |
| `005825_nos-2` | solto | solto | solto | solto | INA 10 (VO) | VO 0,024 V: filtro ok, problema antes do VO |
| `010209_nos-3` | solto | solto | INA 6 | INA 7 | INA 10 | entradas em 0 V: ponte sem alimentação |
| `010714_nos-4` | nó T | solto | nó a | nó b | INA 10 | componente recolocado: T 2,096; Vab −62 mV |
| `011210_nos-5` | nó T | INA 1 | nó a | nó b | INA 10 | V+ 4,995 V; Vab −62 mV (Pt100 A em curto) |
| `012328_nos-6-2fios` | nó T | INA 1 | nó a | nó b | INA 10 | 2 fios usando os dois fios do par: Vab −63 mV |
| `012613_nos-7` | nó T | INA 1 | ponto do VIN+ | nó b | INA 10 | curto resolvido, VIN+ no F: Vab −97 mV |
| `012835_nos-8`, `013243_nos-9` | nó T | INA 1 | ponto do VIN+ | nó b | INA 10 | igual: Vab −97 mV |
| `013530_nos-10` | nó T | INA 1 | ponto do VIN+ | nó b | INA 10 | montagem sendo mexida (instável) |
| `013545_nos-11` | nó T | INA 1 | ponto do VIN+ | nó b | INA 10 | Vab +33 a +49 mV subindo; VO 0,059 V |
| `013955_nos-12-2fios` | nó T | INA 1 | nó a | nó b | INA 10 | **ponte certa a 2 fios: Vab +10,6 mV** |
| `014440_nos-13-3fios` | nó T | INA 1 | nó a | nó b | INA 10 | **ponte certa a 3 fios: Vab +10 mV**; VO 0,06 V |
| `015153_ina-pinos-1` | nó T | INA 6 | INA 7 | INA 8 | INA 9 | jumpers deslocados: pino 6 = 2,046, pino 7 = 2,034, pinos 8 e 9 = 2,033 |
| `015942_ina-pinos-2` | INA 6 | INA 7 | INA 8 | INA 9 | INA 2 | entradas ok, SLEEP 5 V; pino 8 = 2,033 (devia seguir o pino 6, 2,044) |
| `021847_ina-pinos-3` | INA 1 | INA 3 | INA 13 | INA 10 | INA 8 | V+ 5 V, V− 0 V; VREFBG 0,332 V (esperado cerca de 1,24); VO 0,058 |
