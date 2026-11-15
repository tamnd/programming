# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/tail-a-growing-file/.

import json
import os
import tempfile
import time
import unittest
from pathlib import Path

from follow_file import Follower, follow, follow_file

CASES = Path(__file__).resolve().parents[2] / "cases"


def load(name: str) -> list[dict]:
    with open(CASES / name, encoding="utf-8") as f:
        return [json.loads(line) for line in f]


class FollowFileTest(unittest.TestCase):
    def setUp(self) -> None:
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.dir = tmp.name

    def test_cases(self) -> None:
        for case in load("basic.jsonl") + load("edge.jsonl"):
            with self.subTest(case["name"]), tempfile.TemporaryDirectory() as d:
                args = case["args"]
                got = follow_file(os.path.join(d, args["path"]), args["initial"], args["steps"])
                self.assertEqual(got, case["expect"])

    def test_not_found(self) -> None:
        with self.assertRaises(FileNotFoundError):
            follow_file(os.path.join(self.dir, "missing", "followed.txt"), "", ["poll"])

    def test_many_lines(self) -> None:
        path = os.path.join(self.dir, "followed.txt")
        Path(path).write_bytes(b"")
        follower = Follower(path)
        with open(path, "ab") as f:
            f.write("".join(f"{i}\n" for i in range(100000)).encode())
        lines = follower.poll()
        follower.close()
        self.assertEqual(len(lines), 100000)
        self.assertEqual(lines[0], "0")
        self.assertEqual(lines[-1], "99999")

    def test_follow(self) -> None:
        path = os.path.join(self.dir, "followed.txt")
        Path(path).write_bytes(b"old\n")
        deadline = time.monotonic() + 5
        lines = follow(path, interval=0.01, stop=lambda: time.monotonic() > deadline)
        with open(path, "ab") as f:
            f.write(b"a\nb\n")
        self.assertEqual([next(lines), next(lines)], ["a", "b"])
        lines.close()


if __name__ == "__main__":
    unittest.main()
