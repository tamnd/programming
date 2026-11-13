# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-csv-to-json/.

import json
import pathlib

import pytest

from csv_to_json import CsvError, csv_to_json

CASES = pathlib.Path(__file__).resolve().parents[2] / "cases"


def load(name):
    with open(CASES / name, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


@pytest.mark.parametrize("case", load("basic.jsonl") + load("edge.jsonl"), ids=lambda c: c["name"])
def test_cases(case):
    assert csv_to_json(case["args"]["text"]) == case["expect"]


@pytest.mark.parametrize("case", load("errors.jsonl"), ids=lambda c: c["name"])
def test_errors(case):
    with pytest.raises(CsvError) as e:
        csv_to_json(case["args"]["text"])
    assert e.value.kind == case["error"]


def test_output_is_json():
    text = 'id,note\n1,"She said ""hi""\nand left"\n'
    assert json.loads(csv_to_json(text)) == [{"id": "1", "note": 'She said "hi"\nand left'}]
