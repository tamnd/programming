#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/sha256_hash.sh"
  file="$BATS_TEST_TMPDIR/in.bin"
}

check_text() { # check_text TEXT WANT
  run sha256_text "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

# repeat CHAR N prints N copies of CHAR.
repeat() { printf "%$2s" '' | tr ' ' "$1"; }

@test "abc" { check_text abc ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad; }
@test "two-block-56" { check_text abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq 248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1; }
@test "fox" { check_text 'The quick brown fox jumps over the lazy dog' d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592; }
@test "empty" { check_text '' e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855; }
@test "a-55" { check_text "$(repeat a 55)" 9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318; }
@test "a-56" { check_text "$(repeat a 56)" b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a; }
@test "a-63" { check_text "$(repeat a 63)" 7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34; }
@test "a-64" { check_text "$(repeat a 64)" ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb; }
@test "multi-block-112" { check_text abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1; }
@test "non-ascii" { check_text $'na\xc3\xafve caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e \xf0\x9f\x99\x82' 9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d; }
@test "nfc-cafe" { check_text $'caf\xc3\xa9' 850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e; }
@test "nfd-cafe" { check_text $'cafe\xcc\x81' 81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a; }
@test "trailing-newline" { check_text $'abc\n' edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb; }

check_file() { # check_file BYTES WANT; BYTES is a printf %b string
  printf '%b' "$1" >"$file"
  run sha256_file "$file"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

# The cases of cases/files.jsonl.
@test "abc-file" { check_file 'abc' ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad; }
@test "abc-newline-file" { check_file 'abc\n' edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb; }
@test "abc-crlf-file" { check_file 'abc\r\n' 552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025; }
@test "empty-file" { check_file '' e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855; }
@test "binary-file" { check_file '\0000\0001\0377' 26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092; }

# One million repetitions of "a".
@test "million-a-file" {
  repeat a 1000000 >"$file"
  run sha256_file "$file"
  [ "$status" -eq 0 ]
  [ "$output" = cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0 ]
}

@test "missing-file" {
  run --separate-stderr sha256_file "$BATS_TEST_TMPDIR/missing.txt"
  [ "$status" -ne 0 ]
  [ -z "$output" ]
}
