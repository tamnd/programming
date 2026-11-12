# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/repeat-a-string/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/repeat.sh"
}

check() { # check S N WANT
  run repeat "$1" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$3" ]
}

# The case many has the 200 chars of 100 copies of ab in one string.
@test "word" { check 'ab' 3 'ababab'; }
@test "one" { check 'abc' 1 'abc'; }
@test "dash" { check '-' 5 '-----'; }
@test "zero" { check 'abc' 0 ''; }
@test "empty" { check '' 5 ''; }
@test "empty-zero" { check '' 0 ''; }
@test "many" { check 'ab' 100 'abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab'; }
@test "astral" { check $'\xf0\x9f\x99\x82' 3 $'\xf0\x9f\x99\x82\xf0\x9f\x99\x82\xf0\x9f\x99\x82'; }
@test "combining" { check $'e\xcc\x81' 2 $'e\xcc\x81e\xcc\x81'; }
