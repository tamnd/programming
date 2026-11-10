# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-command-line-arguments/.

import argparse
import sys
from typing import NamedTuple


class Args(NamedTuple):
    verbose: bool
    limit: int
    files: list[str]


class UsageError(Exception):
    """A usage error of the command line."""


class _Parser(argparse.ArgumentParser):
    def exit(self, status: int = 0, message: str | None = None) -> None:
        # argparse calls exit after it prints help (status 0) or an error
        # (status 2). An error becomes an exception, so the caller decides.
        if status:
            raise UsageError(message or "usage error")
        super().exit(status, message)

    def print_usage(self, file=None) -> None:
        # error() prints the usage before it calls exit. The caller prints
        # the message of UsageError instead.
        pass


def _parser() -> _Parser:
    p = _Parser(prog="count", allow_abbrev=False)
    p.add_argument("-v", "--verbose", action="store_true")
    p.add_argument("-n", "--limit", type=_count, default=10, metavar="N")
    p.add_argument("files", nargs="*", metavar="FILE")
    return p


def _count(text: str) -> int:
    # int() also accepts "+3", " 3" and "1_000"; the tool takes digits only.
    if not (text.isascii() and text.isdecimal()):
        raise ValueError(text)
    return int(text)


def parse_args(args: list[str]) -> Args:
    ns = _parser().parse_args(args)
    return Args(ns.verbose, ns.limit, ns.files)


if __name__ == "__main__":
    try:
        print(parse_args(sys.argv[1:]))
    except UsageError as e:
        print(e, file=sys.stderr, end="")
        sys.exit(2)
