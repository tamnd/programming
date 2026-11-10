# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-an-integer/.

# parse_integer S [BASE]: prints the value of S as a signed 64-bit integer in
# BASE, from 2 to 36 (10 by default). Returns 1 when S is not an optional sign
# and ASCII digits of the base, and 2 when the value does not fit in 64 bits.
parse_integer() {
  local s=$1 base=${2:-10}
  local lower=0123456789abcdefghijklmnopqrstuvwxyz
  local upper=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
  # List the digits one by one. Outside the POSIX locale, the meaning of a
  # range such as a-z is not specified.
  local class=${lower:0:base}
  if ((base > 10)); then
    class+=${upper:10:base-10}
  fi
  local re="^[+-]?[$class]+$"
  if ! [[ $s =~ $re ]]; then
    echo "parse_integer: invalid integer for base $base: '$s'" >&2
    return 1
  fi
  # Do not use $((base#digits)): shell arithmetic wraps on overflow. Add the
  # digits toward the negative side, so the minimum fits, and check each step.
  local min=$((-9223372036854775807 - 1))
  local digits=${s#[+-]} v=0 i c rest d
  for ((i = 0; i < ${#digits}; i++)); do
    c=${digits:i:1}
    rest=${lower%%"$c"*}
    if ((${#rest} == 36)); then
      rest=${upper%%"$c"*}
    fi
    d=${#rest}
    if ((v < (min + d) / base)); then
      echo "parse_integer: integer out of range: '$s'" >&2
      return 2
    fi
    v=$((v * base - d))
  done
  if [[ $s != -* ]]; then
    if ((v == min)); then
      echo "parse_integer: integer out of range: '$s'" >&2
      return 2
    fi
    v=$((-v))
  fi
  echo "$v"
}
