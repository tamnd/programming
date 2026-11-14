#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-text-in-many-files/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/replace_in_files.sh"
  dir="$BATS_TEST_TMPDIR/case"
  mkdir -p "$dir"
  file="$dir/a.txt"
}

# The x keeps a final LF, which $(...) removes.
same() { # same FILE BYTES
  [ "$(cat "$1"; printf x)" = "$(printf '%bx' "$2")" ]
}

check() { # check OLD NEW BEFORE WANT AFTER
  printf '%b' "$3" >"$file"
  [ "$(replace_in_files "$1" "$2" "$file")" = "$4" ]
  same "$file" "$5"
  # No temporary file stays in the directory.
  [ "$(ls -A "$dir")" = "a.txt" ]
}

@test "one-file" { check 1 2 'x = 1\ny = 1\n' 2 'x = 2\ny = 2\n'; }
@test "no-match" { check x y 'abc\n' 0 'abc\n'; }
@test "empty-file" { check a b '' 0 ''; }
@test "delete" { check - '' 'a-b-c\n' 2 'abc\n'; }
@test "whole-file" { check abc '' 'abc' 1 ''; }
@test "new-contains-old" { check a aa 'a a\n' 2 'aa aa\n'; }
@test "overlapping" { check aa b 'aaa\n' 1 'ba\n'; }
@test "adjacent" { check aa b 'aaaa\n' 2 'bb\n'; }
@test "special-chars" { check 1.5 '\&$1 $& [x]' '1.5 and 125\n' 1 '\\&$1 $& [x] and 125\n'; }
@test "glob-chars" { check '*.txt' '?' 'a.txt *.txt\n' 1 'a.txt ?\n'; }
@test "across-lines" { check $'end\nstart' 'end start' 'end\nstart\n' 1 'end start\n'; }
@test "lf-to-crlf" { check $'\n' $'\r\n' 'a\nb\n' 2 'a\r\nb\r\n'; }
@test "crlf" { check a c 'a\r\nb\r\n' 1 'c\r\nb\r\n'; }
@test "no-final-newline" { check b c 'a b' 1 'a c'; }
@test "non-ascii" { check $'\xc3\xa9' e 'caf\xc3\xa9, cafe\n' 1 'cafe, cafe\n'; }
@test "case-sensitive" { check hello bye 'Hello hello\n' 1 'Hello bye\n'; }

@test "rosetta" {
  printf 'Goodbye London!\n' >"$dir/a.txt"
  printf 'Goodbye London! Goodbye London!\nGoodbye\n' >"$dir/b.txt"
  [ "$(replace_in_files 'Goodbye London!' 'Hello New York!' "$dir/a.txt" "$dir/b.txt")" = "$(printf '1\n2')" ]
  same "$dir/a.txt" 'Hello New York!\n'
  same "$dir/b.txt" 'Hello New York! Hello New York!\nGoodbye\n'
}

@test "some-files-match" {
  printf 'cat\n' >"$dir/a.txt"
  printf 'dog\ncat dog\n' >"$dir/b.txt"
  [ "$(replace_in_files dog fox "$dir/a.txt" "$dir/b.txt")" = "$(printf '0\n2')" ]
  same "$dir/a.txt" 'cat\n'
  same "$dir/b.txt" 'fox\ncat fox\n'
  [ "$(ls -A "$dir" | wc -l | tr -d ' ')" = 2 ]
}

@test "no-files" {
  [ "$(replace_in_files a b)" = "" ]
}

@test "many-lines" {
  awk 'BEGIN { for (i = 0; i < 100000; i++) print "line " i }' >"$file"
  [ "$(replace_in_files 'line ' L "$file")" = 100000 ]
  [ "$(awk 'BEGIN { for (i = 0; i < 100000; i++) print "L" i }' | cksum)" = "$(cksum <"$file")" ]
}

@test "keeps-the-permissions" {
  printf 'a\n' >"$file"
  chmod 640 "$file"
  [ "$(replace_in_files a b "$file")" = 1 ]
  [ "$(ls -l "$file" | cut -c1-10)" = "-rw-r-----" ]
}

@test "not-found" {
  run replace_in_files a b "$dir/missing.txt"
  [ "$status" -eq 2 ]
}

@test "empty-old" {
  printf 'a\n' >"$file"
  run replace_in_files '' b "$file"
  [ "$status" -eq 2 ]
  same "$file" 'a\n'
}

@test "stop" {
  printf 'a\n' >"$dir/a.txt"
  printf 'a\n' >"$dir/c.txt"
  run replace_in_files a b "$dir/a.txt" "$dir/missing.txt" "$dir/c.txt"
  [ "$status" -eq 2 ]
  same "$dir/a.txt" 'b\n'
  same "$dir/c.txt" 'a\n'
}
