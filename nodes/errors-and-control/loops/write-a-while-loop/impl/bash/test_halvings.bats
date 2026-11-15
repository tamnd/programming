#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-while-loop/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/halvings.sh"
}

check() { # check N -- WANT...
  local n=$1
  shift 2
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(halvings "$n"; printf x)
  want=$(if (($# > 0)); then printf '%s\n' "$@"; fi; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "rosetta" { check 1024 -- 1024 512 256 128 64 32 16 8 4 2 1; }
@test "ten" { check 10 -- 10 5 2 1; }
@test "zero" { check 0 --; }
@test "one" { check 1 -- 1; }
@test "odd" { check 7 -- 7 3 1; }
@test "limit" { check 2147483647 -- 2147483647 1073741823 536870911 268435455 134217727 67108863 33554431 16777215 8388607 4194303 2097151 1048575 524287 262143 131071 65535 32767 16383 8191 4095 2047 1023 511 255 127 63 31 15 7 3 1; }
