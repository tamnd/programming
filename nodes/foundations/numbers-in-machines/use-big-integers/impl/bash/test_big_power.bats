#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-big-integers/.

setup() {
  source "$BATS_TEST_DIRNAME/big_power.sh"
}

check() { # check WANT BASE EXP
  local got
  got=$(big_power "$2" "$3")
  [ "$got" = "$1" ] || { printf 'got: %s\nwant: %s\n' "$got" "$1"; return 1; }
}

zeros() { printf '%0*d' "$1" 0; }

@test "small" { check 1024 2 10; }
@test "two-to-the-100" { check 1267650600228229401496703205376 2 100; }
@test "ten-to-the-30" { check "1$(zeros 30)" 10 30; }
@test "zero-exponent" { check 1 7 0; }
@test "zero-to-the-zero" { check 1 0 0; }
@test "zero-base" { check 0 0 5; }
@test "minus-one-odd" { check -1 -1 199; }
@test "negative-odd" { check -36472996377170786403 -3 41; }
@test "negative-even" { check 12157665459056928801 -3 40; }
@test "int64-max-plus-one" { check 9223372036854775808 2 63; }
@test "uint64-max-plus-one" { check 18446744073709551616 2 64; }
@test "two-to-the-200" { check 1606938044258990275541962092341162602522202993782792835301376 2 200; }
@test "largest" { check "1$(zeros 600)" 1000 200; }
@test "smallest" { check "-1$(zeros 597)" -1000 199; }

@test "plain arithmetic wraps" {
  [ "$(( 2 ** 63 ))" = "-9223372036854775808" ]
  [ "$(( 2 ** 64 ))" = "0" ]
}
