#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/embed_file.sh"
}

@test "first-line" {
  [ "$(embedded_line 1)" = alpha ]
}

@test "second-line" {
  [ "$(embedded_line 2)" = "$(printf 'caf\303\251')" ]
}

@test "last-line" {
  [ "$(embedded_line 4)" = gamma ]
}

@test "non-ascii-line" {
  [ "$(embedded_line 2 | wc -c | tr -d ' ')" = 5 ]
}

@test "empty-line" {
  run embedded_line 3
  [ "$status" -eq 0 ]
  [ "$output" = "" ]
}

@test "zero" {
  run embedded_line 0
  [ "$status" -eq 1 ]
  [ "$output" = "" ]
}

@test "negative" {
  run embedded_line -1
  [ "$status" -eq 1 ]
}

@test "past-the-end" {
  run embedded_line 5
  [ "$status" -eq 1 ]
}

@test "the text is the bytes of lines.txt" {
  cmp <(embedded_text) "$BATS_TEST_DIRNAME/lines.txt"
}

@test "the working directory does not matter" {
  cd "$BATS_TEST_TMPDIR"
  [ "$(embedded_line 4)" = gamma ]
}
