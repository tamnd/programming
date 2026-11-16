#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

# is_prime N
# Returns the exit status 0 if N is prime and 1 if not, with trial division.
is_prime() {
  local n=$1 i=3
  if ((n < 2)); then
    return 1
  fi
  if ((n % 2 == 0)); then
    ((n == 2))
    return
  fi
  while ((i <= n / i)); do
    if ((n % i == 0)); then
      return 1
    fi
    i=$((i + 2))
  done
  return 0
}
