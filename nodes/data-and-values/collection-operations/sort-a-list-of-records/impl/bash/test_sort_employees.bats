#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list-of-records/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/sort_employees.sh"
}

check() { # check GIVEN WANT, with records as "name,dept,salary" and a space between records
  local -a given want
  read -ra given <<<"$1"
  read -ra want <<<"$2"
  run sort_employees <<<"$(printf '%s\n' "${given[@]}" | tr ',' '\t')"
  [ "$status" -eq 0 ]
  [ "$output" = "$(printf '%s\n' "${want[@]}" | tr ',' '\t')" ]
}

@test "mixed" { check "Ada,eng,120 Bob,sales,90 Cy,eng,150 Di,ops,100" "Cy,eng,150 Ada,eng,120 Di,ops,100 Bob,sales,90"; }
@test "same-salary-other-dept" { check "Ann,ops,100 Ben,eng,100 Cat,hr,100" "Ben,eng,100 Cat,hr,100 Ann,ops,100"; }
@test "empty" {
  run sort_employees </dev/null
  [ "$status" -eq 0 ]
  [ -z "$output" ]
}
@test "single" { check "Ada,eng,120" "Ada,eng,120"; }
@test "sorted" { check "Cy,eng,150 Ada,eng,120 Bob,ops,90" "Cy,eng,150 Ada,eng,120 Bob,ops,90"; }
@test "reversed" { check "Bob,ops,90 Ada,eng,120 Cy,eng,150" "Cy,eng,150 Ada,eng,120 Bob,ops,90"; }
@test "ties" { check "Zoe,eng,100 Xi,hr,80 Al,eng,100 Yu,hr,80 Mo,eng,100" "Zoe,eng,100 Al,eng,100 Mo,eng,100 Xi,hr,80 Yu,hr,80"; }
@test "many-ties" {
  # The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
  local all="" odd="" even="" i rec
  for ((i = 1; i <= 40; i++)); do
    if ((i % 2 == 0)); then rec=$(printf 'n%02d,a,1' "$i"); odd+="$rec "; else rec=$(printf 'n%02d,b,1' "$i"); even+="$rec "; fi
    all+="$rec "
  done
  check "$all" "$odd$even"
}
@test "case" { check "Ann,eng,100 Bea,Sales,90 Cal,ops,80" "Bea,Sales,90 Ann,eng,100 Cal,ops,80"; }
@test "non-ascii" { check "Eve,équipe,100 Fay,zoo,90 Gus,eng,80" "Gus,eng,80 Fay,zoo,90 Eve,équipe,100"; }
@test "negative" { check "Hal,eng,0 Ida,eng,-5 Jo,eng,5" "Jo,eng,5 Hal,eng,0 Ida,eng,-5"; }
@test "overflow" { check "Kim,eng,-2000000000 Lee,eng,2000000000" "Lee,eng,2000000000 Kim,eng,-2000000000"; }
