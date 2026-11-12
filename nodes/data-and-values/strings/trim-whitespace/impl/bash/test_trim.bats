# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/trim-whitespace/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/trim.sh"
}

check() { # check FUNCTION S WANT
  run "$1" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$3" ]
}

# Only the 6 ASCII white space bytes are removed, so each byte of 0x80 or more stays.
@test "both-ends" { check trim '  hello  ' 'hello'; }
@test "tab-newline" { check trim $'\thello world\n' 'hello world'; }
@test "none" { check trim 'hello' 'hello'; }
@test "empty" { check trim '' ''; }
@test "all-space" { check trim $' \t\n\r ' ''; }
@test "vt-ff" { check trim $'\v\fx\f\v' 'x'; }
@test "other-control" { check trim $'\x01x\x01' $'\x01x\x01'; }
@test "no-break-space" { check trim $'\xc2\xa0x\xc2\xa0' $'\xc2\xa0x\xc2\xa0'; }
@test "ideographic-space" { check trim $'\xe3\x80\x80x\xe3\x80\x80' $'\xe3\x80\x80x\xe3\x80\x80'; }
@test "next-line" { check trim $'\xc2\x85x\xc2\x85' $'\xc2\x85x\xc2\x85'; }
@test "bom" { check trim $'\xef\xbb\xbfx' $'\xef\xbb\xbfx'; }
@test "astral" { check trim $' \xf0\x9f\x99\x82 ' $'\xf0\x9f\x99\x82'; }
@test "combining" { check trim $' \xcc\x81x ' $'\xcc\x81x'; }
@test "start-both-ends" { check trim_start '  hello  ' 'hello  '; }
@test "start-tab-newline" { check trim_start $'\n\tx' 'x'; }
@test "start-empty" { check trim_start '' ''; }
@test "start-all-space" { check trim_start '   ' ''; }
@test "start-no-break-space" { check trim_start $'\xc2\xa0 x' $'\xc2\xa0 x'; }
@test "start-end-only" { check trim_start $'x \t' $'x \t'; }
