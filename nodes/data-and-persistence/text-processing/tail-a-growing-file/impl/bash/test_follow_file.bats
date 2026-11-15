#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/tail-a-growing-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/follow_file.sh"
  f=$BATS_TEST_TMPDIR/followed.txt
}

check() { # check WANT INITIAL STEP...: WANT has N:LINE for each line of poll N
  local want=$1
  shift
  # The x keeps a final LF, which $(...) removes.
  [ "$(follow_file "$f" "$@"; printf x)" = "${want}x" ]
}

@test "append-lines" { check $'1:a\n1:b\n2:c\n' $'old\n' $'append:a\nb\n' poll $'append:c\n' poll; }
@test "partial-line" { check $'2:abc\n3:d\n' '' $'append:ab' poll $'append:c\nd' poll $'append:\n' poll; }
@test "rotate" { check $'1:2\n2:3\n' $'1\n' $'append:2\n' poll rotate $'append:3\n' poll; }
@test "truncate" { check $'1:a\n1:b\n3:c\n' '' $'append:a\nb\n' poll $'truncate:' poll $'append:c\n' poll; }
@test "no-change" { check '' $'x\n' poll poll; }
@test "empty-line" { check $'1:\n1:\n1:a\n' '' $'append:\n\na\n' poll; }
@test "crlf" { check $'1:a\r\n1:b\r\n' '' $'append:a\r\nb\r\n' poll; }
@test "truncate-and-write" { check $'1:aaaa\n2:b\n' '' $'append:aaaa\n' poll $'truncate:b\n' poll; }
@test "truncate-partial" { check $'2:abc\n2:d\n' '' $'append:abc' poll $'truncate:d\n' poll; }
@test "rotate-late-writes" { check $'1:1\n2:2\n2:3\n' '' $'append:1\n' poll rotate $'append-old:2\n' $'append:3\n' poll; }
@test "rotate-partial" { check $'2:a\n2:b\n' '' $'append:a' poll rotate $'append:b\n' poll; }
@test "rotate-twice" { check $'1:1\n2:2\n3:3\n' '' $'append:1\n' poll rotate $'append:2\n' poll rotate $'append:3\n' poll; }
@test "delete" { check $'1:1\n' '' $'append:1\n' poll delete poll poll; }
@test "delete-partial" { check $'2:x\n' '' $'append:x' poll delete poll; }
@test "delete-and-create" { check $'1:1\n3:2\n' '' $'append:1\n' poll delete poll $'create:2\n' poll; }
@test "delete-and-create-between-polls" { check $'1:1\n2:2\n' '' $'append:1\n' poll delete $'create:2\n' poll; }
@test "start-mid-line" { check $'1:d\n1:e\n' abc $'append:d\ne\n' poll; }
@test "non-ascii" { check $'1:caf\303\251\n1:na\303\257ve\n' '' $'append:caf\303\251\nna\303\257ve\n' poll; }
@test "character-across-polls" { check $'2:caf\303\251\n' '' $'append:caf\303' poll $'append:\251\n' poll; }

@test "not-found" {
  run follow_file "$BATS_TEST_TMPDIR/missing/followed.txt" '' poll
  [ "$status" -ne 0 ]
  run follow_open "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
}

@test "many-lines" {
  out=$(follow_file "$f" '' "append:$(seq 0 99999)"$'\n' poll)
  [ "$(printf '%s\n' "$out" | wc -l)" -eq 100000 ]
  [ "$(printf '%s\n' "$out" | head -n 1)" = 1:0 ]
  [ "$(printf '%s\n' "$out" | tail -n 1)" = 1:99999 ]
}

@test "follow" {
  # The lines are written first, so that the test ends on its own.
  printf 'old\n' > "$f"
  follow_open "$f"
  printf 'a\nb\n' >> "$f"
  [ "$(follow 2 0.1)" = $'a\nb' ]
  follow_close
}
