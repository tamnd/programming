# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-text-in-many-files/.

import json
import os
import shutil
import stat
import tempfile
import unittest
from pathlib import Path

from replace_in_files import replace_in_files

NODE = Path(__file__).resolve().parents[2]


def load(name):
    with open(NODE / "cases" / name, encoding="utf-8") as f:
        return [json.loads(line) for line in f if line.strip()]


class ReplaceInFilesTest(unittest.TestCase):
    def setUp(self):
        self.dir = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, self.dir)

    def copy_case(self, name):
        src = NODE / "fixtures" / name
        dst = self.dir / name
        if src.is_dir():
            shutil.copytree(src, dst)
        else:
            dst.mkdir()
        return dst

    def test_cases(self):
        for case in load("basic.jsonl") + load("edge.jsonl"):
            with self.subTest(case["name"]):
                name = case["name"]
                work = self.copy_case(name)
                args = case["args"]
                paths = [work / Path(p).name for p in args["paths"]]
                self.assertEqual(replace_in_files(paths, args["old"], args["new"]), case["expect"])
                after = NODE / "fixtures" / (name + ".after")
                for p in paths:
                    self.assertEqual(p.read_bytes(), (after / p.name).read_bytes())
                self.assertEqual(sorted(os.listdir(work)), sorted(p.name for p in paths))

    def test_many_lines(self):
        path = self.dir / "many.txt"
        path.write_text("".join(f"line {i}\n" for i in range(100000)))
        self.assertEqual(replace_in_files([path], "line ", "L"), [100000])
        self.assertEqual(path.read_text(), "".join(f"L{i}\n" for i in range(100000)))

    def test_keeps_the_permissions(self):
        path = self.dir / "a.txt"
        path.write_text("a\n")
        os.chmod(path, 0o640)
        self.assertEqual(replace_in_files([path], "a", "b"), [1])
        self.assertEqual(stat.S_IMODE(os.stat(path).st_mode), 0o640)

    def test_no_match_does_not_write(self):
        path = self.dir / "a.txt"
        path.write_text("a\n")
        before = os.stat(path).st_ino
        self.assertEqual(replace_in_files([path], "x", "y"), [0])
        self.assertEqual(os.stat(path).st_ino, before)

    def test_errors(self):
        cases = {c["name"]: c for c in load("errors.jsonl")}
        self.assertEqual(set(cases), {"not-found", "empty-old", "stop"})
        with self.assertRaises(FileNotFoundError):
            replace_in_files([self.dir / "missing.txt"], "a", "b")
        path = self.dir / "a.txt"
        path.write_text("a\n")
        with self.assertRaises(ValueError):
            replace_in_files([path], "", "b")
        self.assertEqual(path.read_text(), "a\n")

    def test_stop(self):
        work = self.copy_case("stop")
        with self.assertRaises(FileNotFoundError):
            replace_in_files([work / "a.txt", work / "missing.txt", work / "c.txt"], "a", "b")
        after = NODE / "fixtures" / "stop.after"
        for name in ("a.txt", "c.txt"):
            self.assertEqual((work / name).read_bytes(), (after / name).read_bytes())


if __name__ == "__main__":
    unittest.main()
