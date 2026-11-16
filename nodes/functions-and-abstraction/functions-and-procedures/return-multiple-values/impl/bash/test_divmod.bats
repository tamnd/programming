#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/return-multiple-values/.

setup() {
  source "$BATS_TEST_DIRNAME/divmod.sh"
}

@test "positive" { run div_mod 17 5; [ "$output" = "3 2" ]; }
@test "exact" { run div_mod 20 4; [ "$output" = "5 0" ]; }
@test "smaller-dividend" { run div_mod 3 10; [ "$output" = "0 3" ]; }
@test "negative-dividend" { run div_mod -17 5; [ "$output" = "-4 3" ]; }
@test "negative-divisor" { run div_mod 17 -5; [ "$output" = "-4 -3" ]; }
@test "both-negative" { run div_mod -17 -5; [ "$output" = "3 -2" ]; }
@test "exact-negative" { run div_mod -20 4; [ "$output" = "-5 0" ]; }
@test "small-negative" { run div_mod -3 10; [ "$output" = "-1 7" ]; }
@test "zero-dividend" { run div_mod 0 7; [ "$output" = "0 0" ]; }
@test "zero-negative-divisor" { run div_mod 0 -7; [ "$output" = "0 0" ]; }
@test "divisor-one" { run div_mod 9 1; [ "$output" = "9 0" ]; }
@test "int-max" { run div_mod 2147483647 2; [ "$output" = "1073741823 1" ]; }
@test "int-min" { run div_mod -2147483648 3; [ "$output" = "-715827883 1" ]; }
@test "min-by-max" { run div_mod -2147483648 2147483647; [ "$output" = "-2 2147483646" ]; }
@test "max-by-min" { run div_mod 2147483647 -2147483648; [ "$output" = "-1 -1" ]; }

@test "read into two variables" {
  read -r q r < <(div_mod -17 5)
  [ "$q" = "-4" ]
  [ "$r" = "3" ]
}
