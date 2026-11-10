#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-from-standard-input/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/read_lines.sh"
  file="$BATS_TEST_TMPDIR/input.txt"
}

check() { # check BYTES WANT...
  printf '%b' "$1" >"$file"
  shift
  read_lines <"$file"
  [ "${#lines[@]}" -eq "$#" ] || return 1
  local i=0 want
  for want in "$@"; do
    [ "${lines[i]}" = "$want" ] || return 1
    i=$((i + 1))
  done
}

@test "lf" { check 'alpha\nbeta\ngamma\n' alpha beta gamma; }
@test "blank-lines" { check 'first\n\n\nlast line\n' first "" "" "last line"; }
@test "empty" { check ''; }
@test "only-newline" { check '\n' ""; }
@test "no-final-newline" { check 'one\ntwo\nthree' one two three; }
@test "crlf" { check 'one\r\ntwo\r\nthree\r\n' one two three; }
@test "crlf-no-final-newline" { check 'one\r\ntwo\r\nthree' one two three; }
@test "mixed-endings" { check 'one\r\ntwo\nthree\r\n' one two three; }
@test "trailing-blank-line" { check 'last\n\n' last ""; }
@test "whitespace" { check '  two spaces\ttab \n' $'  two spaces\ttab '; }
@test "backslashes" { check 'C:\\temp\\new\n' 'C:\temp\new'; }
@test "non-ascii" { check 'naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n' "naïve café" "日本語のテキスト" "🙂🙂🙂🙂🙂🙂"; }

@test "long-line" {
  local long
  long=$(head -c 1048576 /dev/zero | tr '\0' x)
  check "$long\nend\n" "$long" end
}
