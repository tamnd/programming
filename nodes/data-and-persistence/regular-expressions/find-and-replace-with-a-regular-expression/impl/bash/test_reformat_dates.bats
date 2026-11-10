#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

setup() {
  source "$BATS_TEST_DIRNAME/reformat_dates.sh"
}

@test "reformat one-date" {
  got=$(reformat_dates $'Due 2024-01-02.' $'-' $'/'; printf .)
  [ "$got" = $'Due 02/01/2024.'. ]
}

@test "reformat two-dates" {
  got=$(reformat_dates $'From 2024-01-02 to 2024-12-31' $'-' $'/'; printf .)
  [ "$got" = $'From 02/01/2024 to 31/12/2024'. ]
}

@test "reformat no-date" {
  got=$(reformat_dates $'no dates here' $'-' $'/'; printf .)
  [ "$got" = $'no dates here'. ]
}

@test "reformat empty-text" {
  got=$(reformat_dates $'' $'-' $'/'; printf .)
  [ "$got" = $''. ]
}

@test "reformat empty-to-sep" {
  got=$(reformat_dates $'2024-01-02' $'-' $''; printf .)
  [ "$got" = $'02012024'. ]
}

@test "reformat whole-text" {
  got=$(reformat_dates $'2024-01-02' $'-' $'/'; printf .)
  [ "$got" = $'02/01/2024'. ]
}

@test "reformat adjacent" {
  got=$(reformat_dates $'2024-01-02,2024-01-03' $'-' $'/'; printf .)
  [ "$got" = $'02/01/2024,03/01/2024'. ]
}

@test "reformat digit-before" {
  got=$(reformat_dates $'12024-01-02' $'-' $'/'; printf .)
  [ "$got" = $'12024-01-02'. ]
}

@test "reformat digit-after" {
  got=$(reformat_dates $'2024-01-023' $'-' $'/'; printf .)
  [ "$got" = $'2024-01-023'. ]
}

@test "reformat date-after-rejected" {
  got=$(reformat_dates $'2024-01-0234-05-06' $'-' $'/'; printf .)
  [ "$got" = $'2024-01-06/05/0234'. ]
}

@test "reformat chained" {
  got=$(reformat_dates $'2024-01-02-03' $'-' $'/'; printf .)
  [ "$got" = $'02/01/2024-03'. ]
}

@test "reformat other-separator" {
  got=$(reformat_dates $'2024/01/02' $'-' $'.'; printf .)
  [ "$got" = $'2024/01/02'. ]
}

@test "reformat dot-separator" {
  got=$(reformat_dates $'2024x01x02 2024.01.02' $'.' $'-'; printf .)
  [ "$got" = $'2024x01x02 02-01-2024'. ]
}

@test "reformat backslash-separator" {
  got=$(reformat_dates $'2024\\01\\02' $'\\' $'-'; printf .)
  [ "$got" = $'02-01-2024'. ]
}

@test "reformat plus-separator" {
  got=$(reformat_dates $'2024+01+02 202401102' $'+' $'-'; printf .)
  [ "$got" = $'02-01-2024 202401102'. ]
}

@test "reformat dollar-in-replacement" {
  got=$(reformat_dates $'2024-01-02' $'-' $'$1'; printf .)
  [ "$got" = $'02$101$12024'. ]
}

@test "reformat backslash-in-replacement" {
  got=$(reformat_dates $'2024-01-02' $'-' $'\\1\\'; printf .)
  [ "$got" = $'02\\1\\01\\1\\2024'. ]
}

@test "reformat digit-in-replacement" {
  got=$(reformat_dates $'2024-01-02' $'-' $'0'; printf .)
  [ "$got" = $'0200102024'. ]
}

@test "reformat whole-match-in-replacement" {
  got=$(reformat_dates $'2024-01-02' $'-' $'$&'; printf .)
  [ "$got" = $'02$&01$&2024'. ]
}

@test "reformat unicode-digits" {
  got=$(reformat_dates $'٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 2024-01-02' $'-' $'/'; printf .)
  [ "$got" = $'٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 02/01/2024'. ]
}

@test "reformat non-ascii-text" {
  got=$(reformat_dates $'Café ☕ 2024-01-02 🙂' $'-' $'/'; printf .)
  [ "$got" = $'Café ☕ 02/01/2024 🙂'. ]
}

@test "reformat non-ascii-separator" {
  got=$(reformat_dates $'2024–01–02' $'–' $'·'; printf .)
  [ "$got" = $'02·01·2024'. ]
}

@test "reformat multiline" {
  got=$(reformat_dates $'2024-01-02\n2024-01-03\n' $'-' $'/'; printf .)
  [ "$got" = $'02/01/2024\n03/01/2024\n'. ]
}

@test "reformat not-a-calendar-date" {
  got=$(reformat_dates $'2024-13-45' $'-' $'/'; printf .)
  [ "$got" = $'45/13/2024'. ]
}

@test "find two-dates" {
  got=$(find_dates $'From 2024-01-02 to 2024-12-31' $'-')
  [ "$got" = $'2024 01 02\n2024 12 31' ]
}

@test "find none" {
  got=$(find_dates $'no dates here' $'-')
  [ "$got" = $'' ]
}

@test "find empty" {
  got=$(find_dates $'' $'-')
  [ "$got" = $'' ]
}

@test "find date-after-rejected" {
  got=$(find_dates $'2024-01-0234-05-06' $'-')
  [ "$got" = $'0234 05 06' ]
}

@test "find dot-separator" {
  got=$(find_dates $'2024x01x02 2024.01.02' $'.')
  [ "$got" = $'2024 01 02' ]
}

@test "find unicode-digits" {
  got=$(find_dates $'٢٠٢٤-٠١-٠٢ 2024-01-02' $'-')
  [ "$got" = $'2024 01 02' ]
}

@test "the locale stays the same" {
  LC_ALL=C.UTF-8
  reformat_dates "2024-01-02" "-" "/" >/dev/null
  [ "$LC_ALL" = C.UTF-8 ]
}
