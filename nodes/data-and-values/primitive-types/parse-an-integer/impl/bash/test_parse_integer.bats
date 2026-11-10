#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-an-integer/.

setup() {
  source "$BATS_TEST_DIRNAME/parse_integer.sh"
  ZEROS=$(printf '0%.0s' {1..5000})
  NINES=$(printf '9%.0s' {1..5000})
}

@test "decimal" {
  run parse_integer '42' 10
  [ "$status" -eq 0 ]
  [ "$output" = "42" ]
}

@test "negative" {
  run parse_integer '-17' 10
  [ "$status" -eq 0 ]
  [ "$output" = "-17" ]
}

@test "hex" {
  run parse_integer 'ff' 16
  [ "$status" -eq 0 ]
  [ "$output" = "255" ]
}

@test "binary" {
  run parse_integer '101010' 2
  [ "$status" -eq 0 ]
  [ "$output" = "42" ]
}

@test "base-36" {
  run parse_integer 'zz' 36
  [ "$status" -eq 0 ]
  [ "$output" = "1295" ]
}

@test "plus-sign" {
  run parse_integer '+5' 10
  [ "$status" -eq 0 ]
  [ "$output" = "5" ]
}

@test "negative-zero" {
  run parse_integer '-0' 10
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "zero" {
  run parse_integer '0' 10
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "leading-zeros" {
  run parse_integer '007' 10
  [ "$status" -eq 0 ]
  [ "$output" = "7" ]
}

@test "many-leading-zeros" {
  run parse_integer "${ZEROS}42" 10
  [ "$status" -eq 0 ]
  [ "$output" = "42" ]
}

@test "max" {
  run parse_integer '9223372036854775807' 10
  [ "$status" -eq 0 ]
  [ "$output" = "9223372036854775807" ]
}

@test "min" {
  run parse_integer '-9223372036854775808' 10
  [ "$status" -eq 0 ]
  [ "$output" = "-9223372036854775808" ]
}

@test "max-hex" {
  run parse_integer '7fffffffffffffff' 16
  [ "$status" -eq 0 ]
  [ "$output" = "9223372036854775807" ]
}

@test "min-binary" {
  run parse_integer '-1000000000000000000000000000000000000000000000000000000000000000' 2
  [ "$status" -eq 0 ]
  [ "$output" = "-9223372036854775808" ]
}

@test "upper-case" {
  run parse_integer 'FF' 16
  [ "$status" -eq 0 ]
  [ "$output" = "255" ]
}

@test "mixed-case" {
  run parse_integer 'Zz' 36
  [ "$status" -eq 0 ]
  [ "$output" = "1295" ]
}

@test "above-safe-integer" {
  run parse_integer '9007199254740993' 10
  [ "$status" -eq 0 ]
  [ "$output" = "9007199254740993" ]
}

@test "empty" {
  run parse_integer '' 10
  [ "$status" -eq 1 ]
}

@test "sign-only" {
  run parse_integer '-' 10
  [ "$status" -eq 1 ]
}

@test "plus-only" {
  run parse_integer '+' 10
  [ "$status" -eq 1 ]
}

@test "double-sign" {
  run parse_integer '+-5' 10
  [ "$status" -eq 1 ]
}

@test "leading-space" {
  run parse_integer ' 42' 10
  [ "$status" -eq 1 ]
}

@test "trailing-space" {
  run parse_integer '42 ' 10
  [ "$status" -eq 1 ]
}

@test "trailing-newline" {
  run parse_integer $'42\n' 10
  [ "$status" -eq 1 ]
}

@test "trailing-garbage" {
  run parse_integer '42px' 10
  [ "$status" -eq 1 ]
}

@test "underscore" {
  run parse_integer '1_000' 10
  [ "$status" -eq 1 ]
}

@test "thousands-separator" {
  run parse_integer '1,000' 10
  [ "$status" -eq 1 ]
}

@test "decimal-point" {
  run parse_integer '4.0' 10
  [ "$status" -eq 1 ]
}

@test "exponent" {
  run parse_integer '1e3' 10
  [ "$status" -eq 1 ]
}

@test "hex-prefix" {
  run parse_integer '0x1A' 16
  [ "$status" -eq 1 ]
}

@test "binary-prefix" {
  run parse_integer '0b101' 2
  [ "$status" -eq 1 ]
}

@test "digit-outside-base" {
  run parse_integer '102' 2
  [ "$status" -eq 1 ]
}

@test "letter-outside-base" {
  run parse_integer 'fg' 16
  [ "$status" -eq 1 ]
}

@test "arabic-indic-digits" {
  run parse_integer $'\xd9\xa1\xd9\xa2' 10
  [ "$status" -eq 1 ]
}

@test "fullwidth-digits" {
  run parse_integer $'\xef\xbc\x91\xef\xbc\x92' 10
  [ "$status" -eq 1 ]
}

@test "kelvin-sign" {
  run parse_integer $'\xe2\x84\xaa' 36
  [ "$status" -eq 1 ]
}

@test "max-plus-one" {
  run parse_integer '9223372036854775808' 10
  [ "$status" -eq 2 ]
}

@test "min-minus-one" {
  run parse_integer '-9223372036854775809' 10
  [ "$status" -eq 2 ]
}

@test "two-to-the-64" {
  run parse_integer '18446744073709551616' 10
  [ "$status" -eq 2 ]
}

@test "hex-sign-bit" {
  run parse_integer '8000000000000000' 16
  [ "$status" -eq 2 ]
}

@test "hex-all-ones" {
  run parse_integer 'ffffffffffffffff' 16
  [ "$status" -eq 2 ]
}

@test "min-times-ten" {
  run parse_integer '-92233720368547758080' 10
  [ "$status" -eq 2 ]
}

@test "many-digits" {
  run parse_integer "$NINES" 10
  [ "$status" -eq 2 ]
}
