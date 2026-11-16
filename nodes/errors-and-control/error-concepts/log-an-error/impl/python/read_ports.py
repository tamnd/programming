# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/log-an-error/.

import logging
from typing import TextIO


def parse_port(text: str) -> int:
    """Return the port number in text, or raise ValueError."""
    if text == "" or any(c not in "0123456789" for c in text):
        raise ValueError(f"not a number: '{text}'")
    port = int(text)
    if not 1 <= port <= 65535:
        raise ValueError(f"out of range: {text}")
    return port


def quote(value: str) -> str:
    """Put value in double quotes and escape the characters that could end the line or the value."""
    for c, e in (("\\", "\\\\"), ('"', '\\"'), ("\n", "\\n"), ("\r", "\\r"), ("\t", "\\t")):
        value = value.replace(c, e)
    return f'"{value}"'


def make_logger(name: str, stream: TextIO) -> logging.Logger:
    """Return the logger name, which writes INFO and higher to stream in the form level=LEVEL message."""
    logger = logging.getLogger(name)
    logger.handlers.clear()
    handler = logging.StreamHandler(stream)
    handler.setFormatter(logging.Formatter("level=%(levelname)s %(message)s"))
    logger.addHandler(handler)
    logger.setLevel(logging.INFO)
    logger.propagate = False
    return logger


def read_ports(texts: list[str], log: logging.Logger) -> list[int]:
    """Return the good ports. Log each bad text at the ERROR level, then a summary."""
    ports = []
    for line, text in enumerate(texts, start=1):
        try:
            ports.append(parse_port(text))
        except ValueError as e:
            # The error is handled here: it is logged once and not raised again.
            log.error('msg="read port failed" line=%d err=%s', line, quote(str(e)))
    log.info('msg="ports read" ok=%d total=%d', len(ports), len(texts))
    return ports
