#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-patch/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/apply_a_patch.sh"
  old="$BATS_TEST_TMPDIR/old.txt"
  patch="$BATS_TEST_TMPDIR/a.patch"
}

check() { # check OLD PATCH WANT, each as printf %b bytes
  printf '%b' "$1" >"$old"
  printf '%b' "$2" >"$patch"
  # The x keeps the final LFs, which $(...) removes.
  got=$(apply_patch "$old" "$patch" && printf x)
  [ "$got" = "$(printf '%bx' "$3")" ]
}

check_error() { # check_error OLD PATCH STATUS
  printf '%b' "$1" >"$old"
  printf '%b' "$2" >"$patch"
  run -"$3" apply_patch "$old" "$patch"
}

@test "change-a-line" { check 'a\nb\nc\n' '--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n' 'a\nB\nc\n'; }
@test "two-hunks" { check 'apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n' '--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n' 'apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n'; }
@test "git-header" { check 'def main():\n    print(1)\n' 'diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n' 'def main():\n    print(2)\n'; }
@test "add-at-end" { check 'a\n' '@@ -1 +1,2 @@\n a\n+b\n' 'a\nb\n'; }
@test "old-empty" { check '' '--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n' 'a\nb\n'; }
@test "delete-all" { check 'a\nb\n' '@@ -1,2 +0,0 @@\n-a\n-b\n' ''; }
@test "no-count" { check 'a\nb\nc\n' '@@ -2 +2 @@\n-b\n+x\n' 'a\nx\nc\n'; }
@test "zero-context" { check 'apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n' '@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n' 'apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n'; }
@test "insert-at-start" { check 'b\n' '@@ -0,0 +1 @@\n+a\n' 'a\nb\n'; }
@test "old-no-final-newline" { check 'a\nb' '@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n' 'a\nb\n'; }
@test "new-no-final-newline" { check 'a\nb\n' '@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n' 'a\nb'; }
@test "empty-context-line" { check 'a\n\nb\n' '@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n' 'a\n\nc\n'; }
@test "space-context-line" { check 'a\n\nb\n' '@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n' 'a\n\nc\n'; }
@test "crlf" { check 'a\r\nb\r\n' '@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n' 'a\r\nc\r\n'; }
@test "header-like-line" { check 'x\n-- y\n' '--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n' 'x\n'; }
@test "non-ascii" { check 'caf\xc3\xa9\n' '@@ -1 +1 @@\n-caf\xc3\xa9\n+na\xc3\xafve\n' 'na\xc3\xafve\n'; }

@test "mismatch" { check_error 'a\nb\nc\n' '@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n' 2; }
@test "malformed" { check_error 'a\nb\nc\n' '@@ -1,3 +1,3 @@\n a\n-b\n' 3; }
@test "no-hunk" { check_error 'a\n' 'no hunk here\n' 3; }
@test "bad-mark" { check_error 'a\n' '@@ -1 +1 @@\n*a\n' 3; }
@test "past-the-end" { check_error 'a\n' '@@ -2 +2 @@\n-a\n+b\n' 2; }

@test "many-hunks" {
  # 100 lines, and 10 hunks with no context change 10 of them. A new bash
  # keeps the loops fast, with no DEBUG trap of bats.
  seq 0 99 >"$old"
  for ((i = 5; i < 100; i += 10)); do
    printf '@@ -%d +%d @@\n-%d\n+x%d\n' $((i + 1)) $((i + 1)) "$i" "$i"
  done >"$patch"
  out=$(bash -c '. "$1" && apply_patch "$2" "$3"' bash "$BATS_TEST_DIRNAME/apply_a_patch.sh" "$old" "$patch")
  [ "$out" = "$(seq 0 99 | awk '{ print ($1 % 10 == 5 ? "x" $1 : $1) }')" ]
}

@test "not-found" {
  printf '@@ -1 +1 @@\n-a\n+b\n' >"$patch"
  run -1 apply_patch "$BATS_TEST_TMPDIR/missing.txt" "$patch"
  [ "$output" = "apply_patch: cannot read $BATS_TEST_TMPDIR/missing.txt" ]
}
