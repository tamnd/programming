#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

setup() {
  source "$BATS_TEST_DIRNAME/cartesian.sh"
}

# close GOT_X GOT_Y WANT_X WANT_Y: the output has 6 decimals.
close() {
  awk -v a="$1" -v b="$2" -v c="$3" -v d="$4" 'BEGIN {
    e = a - c; if (e < 0) e = -e
    f = b - d; if (f < 0) f = -f
    exit !(e < 1e-6 && f < 1e-6)
  }'
}

@test "east" { run to_cartesian 1 0; [ "$status" -eq 0 ]; close $output 1 0; }
@test "north" { run to_cartesian 2 90; [ "$status" -eq 0 ]; close $output 0 2; }
@test "west" { run to_cartesian 2 180; [ "$status" -eq 0 ]; close $output -2 0; }
@test "diagonal" { run to_cartesian 1 45; [ "$status" -eq 0 ]; close $output 0.7071067811865476 0.7071067811865476; }
@test "thirty-degrees" { run to_cartesian 2 30; [ "$status" -eq 0 ]; close $output 1.7320508075688772 1; }
@test "zero-radius" { run to_cartesian 0 123; [ "$status" -eq 0 ]; close $output 0 0; }
@test "south" { run to_cartesian 1 270; [ "$status" -eq 0 ]; close $output 0 -1; }
@test "negative-angle" { run to_cartesian 1 -90; [ "$status" -eq 0 ]; close $output 0 -1; }
@test "full-turn" { run to_cartesian 1 360; [ "$status" -eq 0 ]; close $output 1 0; }
@test "two-turns" { run to_cartesian 3 720; [ "$status" -eq 0 ]; close $output 3 0; }
@test "negative-radius" { run to_cartesian -1 0; [ "$status" -eq 0 ]; close $output -1 0; }
@test "large-radius" { run to_cartesian 1000000 60; [ "$status" -eq 0 ]; close $output 500000 866025.4037844386; }

@test "zero-prints-as-zero" { run to_cartesian 1 90; [ "$output" = "0.000000 1.000000" ]; }
