# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-an-error-with-context/.


class ConfigError(Exception):
    """A setting in a configuration file is bad."""


def parse_port(text: str) -> int:
    """Return the port number in text, or raise ValueError."""
    if text == "" or any(c not in "0123456789" for c in text):
        raise ValueError(f"not a number: '{text}'")
    port = int(text)
    if not 1 <= port <= 65535:
        raise ValueError(f"out of range: {text}")
    return port


def read_port(where: str, text: str) -> int:
    """Return the port number, or raise ConfigError with the ValueError as its cause."""
    try:
        return parse_port(text)
    except ValueError as e:
        raise ConfigError(f"{where}: read port") from e


def chain(e: BaseException) -> str:
    """Join the message of e and the messages of its causes."""
    parts = [str(e)]
    while e.__cause__ is not None:
        e = e.__cause__
        parts.append(str(e))
    return ": ".join(parts)


def check_setting(where: str, text: str) -> str:
    """Return "port N", or "error: " and the chain of messages."""
    try:
        return f"port {read_port(where, text)}"
    except ConfigError as e:
        return f"error: {chain(e)}"
