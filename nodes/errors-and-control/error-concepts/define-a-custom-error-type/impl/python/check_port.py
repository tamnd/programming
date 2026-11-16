# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-custom-error-type/.


class PortError(Exception):
    """The text is not a valid TCP port number."""

    def __init__(self, text: str, reason: str) -> None:
        super().__init__(f"invalid port '{text}': {reason}")
        self.text = text
        self.reason = reason


def parse_port(text: str) -> int:
    """Return the port number in text, or raise PortError."""
    if text == "":
        raise PortError(text, "empty")
    if any(c not in "0123456789" for c in text):
        raise PortError(text, "not a number")
    port = int(text)
    if not 1 <= port <= 65535:
        raise PortError(text, "out of range")
    return port


def check_port(text: str) -> str:
    """Return "port N", or the message of the PortError."""
    try:
        return f"port {parse_port(text)}"
    except PortError as e:
        # Only a PortError comes here. A bug, such as a TypeError, goes on up.
        return str(e)
