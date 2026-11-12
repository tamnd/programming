#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-with-buffers/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/copy_buffered.sh"
  src="$BATS_TEST_TMPDIR/in.bin"
  dst="$BATS_TEST_TMPDIR/out.bin"
}

check() { # check SIZE: copy src to dst and compare
  local n want
  want=$(wc -c <"$src")
  copy_buffered n "$src" "$dst" "$1"
  [ "$n" -eq "$want" ]
  cmp -s "$src" "$dst"
}

text() { printf 'alpha\nbeta\ngamma\n' >"$src"; }

# all_bytes N: write N copies of the 256 byte values to src.
all_bytes() {
  local i esc=''
  for ((i = 0; i < 256; i++)); do esc+=$(printf '\\%03o' "$i"); done
  printf "$esc" >"$BATS_TEST_TMPDIR/block.bin"
  for ((i = 0; i < $1; i++)); do cat "$BATS_TEST_TMPDIR/block.bin"; done >"$src"
}

@test "text" { text; check 4096; }
@test "several buffers" { text; check 4; }
@test "empty" { : >"$src"; check 8; }
@test "smaller than buffer" { printf 12345 >"$src"; check 8; }
@test "exact buffer" { printf 12345678 >"$src"; check 8; }
@test "one past buffer" { printf 123456789 >"$src"; check 8; }
@test "size one" { text; check 1; }
@test "binary" { all_bytes 1; [ "$(wc -c <"$src")" -eq 256 ]; check 64; }
@test "large" { all_bytes 4096; check 4096; }
@test "large, odd buffer" { all_bytes 4096; check 4093; }

@test "replaces a longer file" {
  printf new >"$src"
  printf 'old and longer' >"$dst"
  check 8
}

@test "missing src" {
  run -1 copy_buffered n "$BATS_TEST_TMPDIR/missing.bin" "$dst" 8
  [ ! -e "$dst" ]
}

@test "missing dst dir" {
  text
  run -1 copy_buffered n "$src" "$BATS_TEST_TMPDIR/no-such-dir/out.bin" 8
}

@test "invalid size" {
  text
  run -2 copy_buffered n "$src" "$dst" 0
  run -2 copy_buffered n "$src" "$dst" -1
  run -2 copy_buffered n "$src" "$dst" 4k
}

@test "full device" {
  [ -e /dev/full ] || skip "no /dev/full"
  text
  run -1 copy_buffered n "$src" /dev/full 4096
}
