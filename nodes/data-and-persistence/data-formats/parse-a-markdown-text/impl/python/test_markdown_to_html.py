# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-a-markdown-text/.

import json
import pathlib

import pytest

from markdown_to_html import markdown_to_html

CASES = pathlib.Path(__file__).resolve().parents[2] / "cases"


def load(name):
    with open(CASES / name, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


@pytest.mark.parametrize("case", load("basic.jsonl") + load("edge.jsonl"), ids=lambda c: c["name"])
def test_cases(case):
    assert markdown_to_html(case["args"]["text"]) == case["expect"]

