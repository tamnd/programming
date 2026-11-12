#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-a-binary-file/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/read_bytes.sh"
  file="$BATS_TEST_TMPDIR/input.bin"
}

# write HEX: write the bytes that HEX names to the file.
write() {
  local hex=$1 esc=''
  while [[ -n $hex ]]; do
    esc+="\\x${hex:0:2}"
    hex=${hex:2}
  done
  printf "$esc" >"$file"
}

check() { # check HEX
  local got
  write "$1"
  read_bytes got "$file"
  [ "$got" = "$1" ]
}

check_exact() { # check_exact HEX N WANT
  local got
  write "$1"
  read_exact got "$file" "$2"
  [ "$got" = "$3" ]
}

all_bytes() {
  local i
  for ((i = 0; i < 256; i++)); do printf '%02x' "$i"; done
}

@test "signature" { check 89504e470d0a1a0a0000000d49484452; }
@test "bytes" { check 0001027f80feff; }
@test "empty" { check ''; }
@test "all-bytes" { check "$(all_bytes)"; }
@test "zeros" { check 0000000000000000; }
@test "crlf" { check 610d0a620d0a; }
@test "ctrl-z" { check 611a62; }
@test "not-utf8" { check c328fffe80; }

@test "large" {
  local got want
  want=$(all_bytes)
  for _ in 1 2 3 4 5 6 7 8 9 10 11 12; do want+=$want; done # 256 * 4096 bytes
  write "$(all_bytes)"
  for _ in 1 2 3 4 5 6 7 8 9 10 11 12; do cat "$file" "$file" >"$file.2" && mv "$file.2" "$file"; done
  read_bytes got "$file"
  [ "${#got}" -eq $((2 << 20)) ]
  [ "$got" = "$want" ]
}

@test "signature-only" { check_exact 89504e470d0a1a0a0000000d49484452 8 89504e470d0a1a0a; }
@test "first-four" { check_exact "$(all_bytes)" 4 00010203; }
@test "whole-file" { check_exact 0000000000000000 8 0000000000000000; }
@test "zero-count" { check_exact '' 0 ''; }
@test "across-ctrl-z" { check_exact 611a62 3 611a62; }

@test "one-past-end" {
  write 89504e470d0a1a0a0000000d49484452
  run -2 read_exact got "$file" 17
}

@test "empty-file" {
  write ''
  run -2 read_exact got "$file" 1
}

@test "missing" {
  run -1 read_bytes got "$BATS_TEST_TMPDIR/missing.bin"
  run -1 read_exact got "$BATS_TEST_TMPDIR/missing.bin" 1
}

@test "directory" {
  run -1 read_bytes got "$BATS_TEST_TMPDIR"
  run -1 read_exact got "$BATS_TEST_TMPDIR" 1
}
