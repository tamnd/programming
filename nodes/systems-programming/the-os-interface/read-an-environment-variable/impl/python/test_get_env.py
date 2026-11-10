# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-an-environment-variable/.

import os
import sys

import pytest

from get_env import get_env

# name: (variables to set, name to read, default, expected value)
CASES = {
    "present": ({"APP_PORT": "8080"}, "APP_PORT", "80", "8080"),
    "path-value": ({"APP_HOME": "/srv/app"}, "APP_HOME", "/opt/app", "/srv/app"),
    "missing": ({}, "APP_PORT", "80", "80"),
    "empty-value": ({"APP_MODE": ""}, "APP_MODE", "production", ""),
    "empty-default": ({}, "APP_MODE", "", ""),
    "spaces": ({"APP_TITLE": "  two  spaces  "}, "APP_TITLE", "none", "  two  spaces  "),
    "equals": ({"APP_QUERY": "key=value=more"}, "APP_QUERY", "none", "key=value=more"),
    "newline": ({"APP_TEXT": "first\nsecond"}, "APP_TEXT", "none", "first\nsecond"),
    "non-ascii": ({"APP_GREETING": "xin chào, 日本語, 🙂"}, "APP_GREETING", "none", "xin chào, 日本語, 🙂"),
    "case": ({"APP_PORT": "8080"}, "app_port", "80", "80"),
    "inherited-name": ({}, "toString", "none", "none"),
}


@pytest.mark.parametrize("case", CASES)
def test_cases(monkeypatch: pytest.MonkeyPatch, case: str) -> None:
    env, name, default, want = CASES[case]
    monkeypatch.delenv(name, raising=False)
    for key, value in env.items():
        monkeypatch.setenv(key, value)
    assert get_env(name, default) == want


@pytest.mark.skipif(sys.platform == "win32", reason="os.environb exists only on Unix")
def test_value_not_utf8(monkeypatch: pytest.MonkeyPatch) -> None:
    # On Unix, a byte that is not UTF-8 becomes a lone surrogate (surrogateescape).
    monkeypatch.setitem(os.environb, b"APP_NAME", b"caf\xe9")
    assert get_env("APP_NAME", "none") == "caf\udce9"
