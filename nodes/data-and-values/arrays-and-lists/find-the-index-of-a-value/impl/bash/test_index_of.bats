#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-index-of-a-value/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/index_of.sh"
}

@test "middle" { got=$(index_of 7 4 7 1) || return 1; [ "$got" = 1 ]; }
@test "absent" { if index_of 9 4 7 1 >/dev/null; then return 1; fi; got=$(index_of 9 4 7 1 || true); [ -z "$got" ]; }
@test "empty" { if index_of 0  >/dev/null; then return 1; fi; got=$(index_of 0  || true); [ -z "$got" ]; }
@test "single" { got=$(index_of 5 5) || return 1; [ "$got" = 0 ]; }
@test "single-absent" { if index_of 0 5 >/dev/null; then return 1; fi; got=$(index_of 0 5 || true); [ -z "$got" ]; }
@test "first" { got=$(index_of 7 7 8 9) || return 1; [ "$got" = 0 ]; }
@test "last" { got=$(index_of 9 7 8 9) || return 1; [ "$got" = 2 ]; }
@test "duplicates" { got=$(index_of 5 3 5 3 5) || return 1; [ "$got" = 1 ]; }
@test "all-same" { got=$(index_of 2 2 2 2) || return 1; [ "$got" = 0 ]; }
@test "negative" { got=$(index_of -2 0 -1 -2) || return 1; [ "$got" = 2 ]; }
@test "limit" { got=$(index_of -2147483648 2147483647 -2147483648) || return 1; [ "$got" = 1 ]; }
@test "limit-absent" { if index_of -2147483648 2147483647 >/dev/null; then return 1; fi; got=$(index_of -2147483648 2147483647 || true); [ -z "$got" ]; }
@test "past-255" { got=$(index_of x $(seq 300) x) || return 1; [ "$got" = 300 ]; }
@test "pattern" { got=$(index_of "*" a "*") || return 1; [ "$got" = 1 ]; }
