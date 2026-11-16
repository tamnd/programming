#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-floating-point-numbers/.

setup() {
  source "$BATS_TEST_DIRNAME/is_close.sh"
}

check() { # check WANT A B REL_TOL ABS_TOL; checks A, B and B, A
  local want=$1 got swapped
  got=$(is_close "$2" "$3" "$4" "$5")
  swapped=$(is_close "$3" "$2" "$4" "$5")
  [ "$got" = "$want" ] && [ "$swapped" = "$want" ] ||
    { printf 'got: %s %s\nwant: %s\n' "$got" "$swapped" "$want"; return 1; }
}

@test "equal" { check true 1.5 1.5 1e-9 0; }
@test "sum-of-tenths" { check true 0.30000000000000004 0.3 1e-9 0; }
@test "different" { check false 1.0 1.1 1e-9 0; }
@test "large-values" { check true 10000000000.0 10000000001.0 1e-9 0; }
@test "zero-and-zero" { check true 0.0 0.0 0 0; }
@test "one-ulp-apart" { check false 1.0 1.0000000000000002 0 0; }
@test "near-zero-relative" { check false 1e-12 0.0 1e-9 0; }
@test "near-zero-absolute" { check true 1e-12 0.0 1e-9 1e-9; }
@test "opposite-signs" { check false 1e-10 -1e-10 1e-9 0; }
@test "opposite-signs-absolute" { check true 1e-10 -1e-10 1e-9 1e-9; }
@test "on-the-boundary" { check true 10.0 9.0 0.1 0; }
@test "swapped-boundary" { check true 9.0 10.0 0.1 0; }
@test "past-the-boundary" { check false 10.0 8.875 0.1 0; }
@test "negative-values" { check true -1.0 -1.0000000001 1e-9 0; }
