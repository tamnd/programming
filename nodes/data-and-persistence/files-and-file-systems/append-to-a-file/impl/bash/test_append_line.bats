#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/append_line.sh"
  file="$BATS_TEST_TMPDIR/log.txt"
}

check() { # check OLD LINE WANT; OLD is "-" when no file is there
  local got want
  if [[ $1 != - ]]; then printf '%b' "$1" >"$file"; fi
  append_line "$file" "$2"
  got=$(cat -- "$file" && printf x)
  want=$(printf '%b' "$3" && printf x)
  [ "$got" = "$want" ]
}

@test "new-file" { check - 'first' 'first\n'; }
@test "append-existing" { check 'one\n' 'two' 'one\ntwo\n'; }
@test "new-file-empty-line" { check - '' '\n'; }
@test "empty-line" { check 'a\n' '' 'a\n\n'; }
@test "empty-file" { check '' 'x' 'x\n'; }
@test "no-final-newline" { check 'one' 'two' 'onetwo\n'; }
@test "crlf-file" { check 'one\r\n' 'two' 'one\r\ntwo\n'; }
@test "whitespace" { check '' $'  two spaces\ttab ' '  two spaces\ttab \n'; }
@test "non-ascii" { check 'naïve\n' 'café 日本語 🙂' 'naïve\ncafé 日本語 🙂\n'; }

@test "missing-dir" {
  run ! append_line "$BATS_TEST_TMPDIR/missing/log.txt" x
}

@test "newline-in-line" {
  run ! append_line "$file" $'a\nb'
  [ ! -e "$file" ]
}

@test "cr-in-line" {
  run ! append_line "$file" $'a\rb'
  [ ! -e "$file" ]
}

@test "many-writers" {
  local w i x40
  x40=$(printf 'x%.0s' {1..40})
  for w in 0 1 2 3 4 5 6 7; do
    (for i in $(seq -w 0 249); do append_line "$file" "writer $w line $i $x40"; done) &
  done
  wait
  [ "$(wc -l <"$file")" -eq 2000 ]
  [ "$(sort -u -- "$file" | grep -c -E "^writer [0-7] line [0-9]{3} x{40}$")" -eq 2000 ]
}
