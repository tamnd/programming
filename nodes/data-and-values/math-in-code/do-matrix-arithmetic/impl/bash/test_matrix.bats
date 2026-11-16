#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-matrix-arithmetic/.

setup() {
  source "$BATS_TEST_DIRNAME/matrix.sh"
}

@test "two-by-two" { run matrix_multiply '1 2;3 4' '5 6;7 8'; [ "$status" -eq 0 ]; [ "$output" = '19 22;43 50' ]; }
@test "two-by-three-times-three-by-two" { run matrix_multiply '1 2 3;4 5 6' '7 8;9 10;11 12'; [ "$status" -eq 0 ]; [ "$output" = '58 64;139 154' ]; }
@test "one-by-one" { run matrix_multiply '3' '4'; [ "$status" -eq 0 ]; [ "$output" = '12' ]; }
@test "row-times-column" { run matrix_multiply '1 2 3' '4;5;6'; [ "$status" -eq 0 ]; [ "$output" = '32' ]; }
@test "column-times-row" { run matrix_multiply '1;2;3' '4 5 6'; [ "$status" -eq 0 ]; [ "$output" = '4 5 6;8 10 12;12 15 18' ]; }
@test "identity" { run matrix_multiply '1 0;0 1' '5 6;7 8'; [ "$status" -eq 0 ]; [ "$output" = '5 6;7 8' ]; }
@test "other-order" { run matrix_multiply '5 6;7 8' '1 2;3 4'; [ "$status" -eq 0 ]; [ "$output" = '23 34;31 46' ]; }
@test "zero-matrix" { run matrix_multiply '0 0;0 0' '1 2;3 4'; [ "$status" -eq 0 ]; [ "$output" = '0 0;0 0' ]; }
@test "negative-numbers" { run matrix_multiply '-1 2;3 -4' '2 0;1 -1'; [ "$status" -eq 0 ]; [ "$output" = '0 -2;2 4' ]; }
@test "beyond-32-bit" { run matrix_multiply '100000 0;0 100000' '100000;3'; [ "$status" -eq 0 ]; [ "$output" = '10000000000;300000' ]; }

@test "transpose" { run transpose '1 2 3;4 5 6'; [ "$output" = '1 4;2 5;3 6' ]; }

@test "sizes-must-match" { run matrix_multiply '1 2' '1 2'; [ "$status" -eq 1 ]; }
