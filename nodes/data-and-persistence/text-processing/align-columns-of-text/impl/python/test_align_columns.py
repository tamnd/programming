# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/align-columns-of-text/.

import json
import pathlib
import unittest

from align_columns import align_columns

CASES = pathlib.Path(__file__).resolve().parent.parent.parent / "cases"


def load(name):
    with open(CASES / name, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


class AlignColumnsTest(unittest.TestCase):
    def test_cases(self):
        for case in load("basic.jsonl") + load("edge.jsonl"):
            with self.subTest(case["name"]):
                args = case["args"]
                self.assertEqual(align_columns(args["text"], args["align"]), case["expect"])

    def test_many_lines(self):
        text = "".join(f"{i}$x\n" for i in range(100000))
        lines = align_columns(text, "right").split("\n")
        self.assertEqual(len(lines), 100001)
        self.assertEqual(lines[0], "    0 x")
        self.assertEqual(lines[99999], "99999 x")

    def test_bad_align(self):
        for case in load("errors.jsonl"):
            with self.subTest(case["name"]):
                with self.assertRaises(ValueError):
                    align_columns(case["args"]["text"], case["args"]["align"])


if __name__ == "__main__":
    unittest.main()
