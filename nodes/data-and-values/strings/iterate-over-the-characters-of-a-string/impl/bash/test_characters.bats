# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/characters.sh"
}

check() { # check S WANT...: characters S must give the arguments after S
  characters "$1"
  shift
  [ "${#CHARS[@]}" -eq "$#" ]
  local i=0
  for c in "$@"; do
    [ "${CHARS[i]}" = "$c" ]
    ((i += 1))
  done
}

# Each argument is a string of UTF-8 bytes, and each element of CHARS is one whole code point.
@test "ascii" { check 'abc' 'a' 'b' 'c'; }
@test "word" { check 'hello' 'h' 'e' 'l' 'l' 'o'; }
@test "space" { check 'a b' 'a' ' ' 'b'; }
@test "empty" { check ''; }
@test "single" { check 'x' 'x'; }
@test "non-ascii" { check $'h\xc3\xa9' 'h' $'\xc3\xa9'; }
@test "cjk" { check $'\xe6\x97\xa5\xe6\x9c\xac' $'\xe6\x97\xa5' $'\xe6\x9c\xac'; }
@test "astral" { check $'a\xf0\x9f\x99\x82b' 'a' $'\xf0\x9f\x99\x82' 'b'; }
@test "combining" { check $'e\xcc\x81' 'e' $'\xcc\x81'; }
@test "regional-indicator" { check $'\xf0\x9f\x87\xba\xf0\x9f\x87\xb8' $'\xf0\x9f\x87\xba' $'\xf0\x9f\x87\xb8'; }
@test "zwj" { check $'\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb' $'\xf0\x9f\x91\xa9' $'\xe2\x80\x8d' $'\xf0\x9f\x92\xbb'; }
@test "control" { check $'a\tb\nc' 'a' $'\t' 'b' $'\n' 'c'; }
