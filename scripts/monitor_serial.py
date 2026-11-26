#!/usr/bin/env python3
"""Monitor serial da câmara térmica.

Mostra cada linha do Arduino na tela, grava em dados/<data>_<nome>.tsv com o
horário do PC e aceita comandos (ex.: PWM em %) digitados no terminal ou
escritos no FIFO build/serial_cmd (echo 30 > build/serial_cmd).
Ao sair (Ctrl+C), manda 0 para desligar o aquecedor.
"""
import argparse
import datetime
import os
import pathlib
import signal
import sys
import threading

import serial
from serial.tools import list_ports

ROOT = pathlib.Path(__file__).resolve().parent.parent
FIFO = ROOT / "build" / "serial_cmd"


def find_port() -> str:
    for p in sorted(list_ports.comports(), key=lambda p: p.device):
        if p.device.startswith(("/dev/ttyACM", "/dev/ttyUSB")):
            return p.device
    sys.exit("Nenhum Arduino encontrado em /dev/ttyACM* ou /dev/ttyUSB*.")


def now() -> str:
    return datetime.datetime.now().isoformat(timespec="milliseconds")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("-p", "--port", help="porta serial (padrão: detecta)")
    ap.add_argument("-b", "--baud", type=int, default=9600)
    ap.add_argument("-n", "--nome", default="ensaio", help="sufixo do arquivo de log")
    args = ap.parse_args()

    # kill / parada em segundo plano também passa pelo finally e desliga o PWM
    signal.signal(signal.SIGTERM, lambda *_: (_ for _ in ()).throw(KeyboardInterrupt))

    port = args.port or find_port()
    stamp = datetime.datetime.now().strftime("%Y-%m-%d_%H%M%S")
    log_path = ROOT / "dados" / f"{stamp}_{args.nome}.tsv"
    log_path.parent.mkdir(exist_ok=True)
    FIFO.parent.mkdir(exist_ok=True)
    if FIFO.exists():
        FIFO.unlink()
    os.mkfifo(FIFO)

    # Abrir a porta reinicia o Uno (DTR); o cabeçalho do setup() entra no log.
    ser = serial.Serial(port, args.baud, timeout=1)
    log = open(log_path, "w", buffering=1, encoding="utf-8")
    lock = threading.Lock()

    def send(cmd: str, origem: str) -> None:
        cmd = cmd.strip()
        if not cmd:
            return
        with lock:
            ser.write((cmd + "\n").encode())
            log.write(f"{now()}\t# comando ({origem}): {cmd}\n")
        print(f"<< {cmd} ({origem})", flush=True)

    def fifo_loop() -> None:
        while True:
            with open(FIFO, encoding="utf-8") as f:
                for line in f:
                    send(line, "fifo")

    def stdin_loop() -> None:
        for line in sys.stdin:
            send(line, "teclado")

    threading.Thread(target=fifo_loop, daemon=True).start()
    if sys.stdin.isatty():
        threading.Thread(target=stdin_loop, daemon=True).start()

    print(f"Porta {port} a {args.baud} baud. Log: {log_path}", flush=True)
    print(f"Comandos: digite aqui + Enter, ou: echo 30 > {FIFO}. Ctrl+C sai e desliga o PWM.", flush=True)
    try:
        while True:
            raw = ser.readline()
            if not raw:
                continue
            line = raw.decode("utf-8", errors="replace").rstrip("\r\n")
            with lock:
                log.write(f"{now()}\t{line}\n")
            print(line, flush=True)
    except KeyboardInterrupt:
        pass
    finally:
        try:
            send("0", "saida")
        except serial.SerialException:
            pass
        ser.close()
        log.close()
        FIFO.unlink(missing_ok=True)
        print(f"\nEncerrado. Dados em {log_path}", flush=True)


if __name__ == "__main__":
    main()
