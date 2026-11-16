#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-mean-and-median/.

setup() {
  source "$BATS_TEST_DIRNAME/meanmedian.sh"
}

# close GOT_MEAN GOT_MEDIAN WANT_MEAN WANT_MEDIAN
close() {
  awk -v a="$1" -v b="$2" -v c="$3" -v d="$4" 'function ok(g, w,  e, t) {
    e = g - w; if (e < 0) e = -e
    t = w < 0 ? -w : w; t = t * 1e-12; if (t < 1e-9) t = 1e-9
    return e <= t
  }
  BEGIN { exit !(ok(a, c) && ok(b, d)) }'
}

@test "odd" { run mean_and_median 3 1 2; [ "$status" -eq 0 ]; close $output 2 2; }
@test "even" { run mean_and_median 4 1 3 2; [ "$status" -eq 0 ]; close $output 2.5 2.5; }
@test "skewed" { run mean_and_median 1 2 3 4 100; [ "$status" -eq 0 ]; close $output 22 3; }
@test "single" { run mean_and_median 5; [ "$status" -eq 0 ]; close $output 5 5; }
@test "two-items" { run mean_and_median 1 2; [ "$status" -eq 0 ]; close $output 1.5 1.5; }
@test "negative" { run mean_and_median -3 -1 -2; [ "$status" -eq 0 ]; close $output -2 -2; }
@test "unsorted-even" { run mean_and_median 10 2 8 4; [ "$status" -eq 0 ]; close $output 6 6; }
@test "duplicates" { run mean_and_median 1 1 1 5; [ "$status" -eq 0 ]; close $output 2 1; }
@test "not-an-integer" { run mean_and_median 1 2 2; [ "$status" -eq 0 ]; close $output 1.6666666666666667 2; }
@test "large" { run mean_and_median 9223372036854775807 9223372036854775807; [ "$status" -eq 0 ]; close $output 9223372036854775808 9223372036854775808; }

@test "empty-fails" { run mean_and_median; [ "$status" -eq 1 ]; }
