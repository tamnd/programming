#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-lazy-sequences/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/primes.sh"
}

check() { # check WANT N
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  got=$(first_primes "$2"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "five" { check "2\n3\n5\n7\n11\n" 5; }
@test "ten" { check "2\n3\n5\n7\n11\n13\n17\n19\n23\n29\n" 10; }
@test "twenty-five" { check "2\n3\n5\n7\n11\n13\n17\n19\n23\n29\n31\n37\n41\n43\n47\n53\n59\n61\n67\n71\n73\n79\n83\n89\n97\n" 25; }
@test "zero" { check "" 0; }
@test "one" { check "2\n" 1; }
@test "two" { check "2\n3\n" 2; }
@test "past-a-gap" { check "2\n3\n5\n7\n11\n13\n17\n19\n23\n29\n31\n37\n41\n43\n47\n53\n59\n61\n67\n71\n73\n79\n83\n89\n97\n101\n103\n107\n109\n113\n127\n" 31; }
@test "is_prime" { is_prime 97; run ! is_prime 91; }
