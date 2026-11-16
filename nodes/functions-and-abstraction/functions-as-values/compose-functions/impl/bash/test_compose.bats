#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compose-functions/.

setup() {
  source "$BATS_TEST_DIRNAME/compose.sh"
}

times() {
  local n=$1 i
  shift
  for ((i = 0; i < n; i++)); do
    printf '%s\n' "$@"
  done
}

square() { echo $(($1 * $1)); }

@test "inc-after-double" { run compose_all 3 inc double; [ "$output" = "7" ]; }
@test "double-after-inc" { run compose_all 3 double inc; [ "$output" = "8" ]; }
@test "three-functions" { run compose_all 5 negate dec double; [ "$output" = "-9" ]; }
@test "empty" { run compose_all 42; [ "$output" = "42" ]; }
@test "single-zero" { run compose_all 0 negate; [ "$output" = "0" ]; }
@test "negative" { run compose_all -5 dec negate; [ "$output" = "4" ]; }
@test "low" { run compose_all -1000 inc; [ "$output" = "-999" ]; }
@test "duplicates" { run compose_all 7 inc inc inc; [ "$output" = "10" ]; }
@test "limit-high" { mapfile -t ops < <(times 20 double); run compose_all 1000 "${ops[@]}"; [ "$output" = "1048576000" ]; }
@test "limit-low" { mapfile -t ops < <(times 20 double); run compose_all -1000 "${ops[@]}"; [ "$output" = "-1048576000" ]; }
@test "mixed-20" { mapfile -t ops < <(times 10 double inc); run compose_all 1000 "${ops[@]}"; [ "$output" = "1026046" ]; }
@test "mixed-20-swapped" { mapfile -t ops < <(times 10 inc double); run compose_all 1000 "${ops[@]}"; [ "$output" = "1025023" ]; }
@test "another function" { run compose_all 4 square inc; [ "$output" = "25" ]; }
