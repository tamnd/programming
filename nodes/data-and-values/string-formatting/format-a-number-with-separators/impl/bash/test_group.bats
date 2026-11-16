#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-separators/.

setup() {
  source "$BATS_TEST_DIRNAME/group.sh"
}

@test "million" { run group_thousands 1234567; [ "$output" = '1,234,567' ]; }
@test "thousand" { run group_thousands 1000; [ "$output" = '1,000' ]; }
@test "small" { run group_thousands 42; [ "$output" = '42' ]; }
@test "zero" { run group_thousands 0; [ "$output" = '0' ]; }
@test "three-digits" { run group_thousands 999; [ "$output" = '999' ]; }
@test "negative" { run group_thousands -1234567; [ "$output" = '-1,234,567' ]; }
@test "negative-small" { run group_thousands -999; [ "$output" = '-999' ]; }
@test "negative-thousand" { run group_thousands -1000; [ "$output" = '-1,000' ]; }
@test "int64-max" { run group_thousands 9223372036854775807; [ "$output" = '9,223,372,036,854,775,807' ]; }
@test "int64-min" { run group_thousands -9223372036854775808; [ "$output" = '-9,223,372,036,854,775,808' ]; }
@test "six-digits" { run group_thousands 100000; [ "$output" = '100,000' ]; }
@test "seven-digits" { run group_thousands 1000000; [ "$output" = '1,000,000' ]; }
