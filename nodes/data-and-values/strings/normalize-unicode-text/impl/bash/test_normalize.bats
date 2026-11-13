# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/normalize-unicode-text/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/normalize.sh"
}

check() { # check S FORM WANT
  run normalize "$1" "$2"
  [ "$status" -eq 0 ]
  [ "$output" = "$3" ]
}

# Each string is UTF-8 bytes. U+00E9 is NFC, and e U+0301 is NFD. Both are the same text.
@test "nfc-composes" { check $'e\xcc\x81' NFC $'\xc3\xa9'; }
@test "nfd-decomposes" { check $'\xc3\xa9' NFD $'e\xcc\x81'; }
@test "ascii" { check 'abc' NFC 'abc'; }
@test "empty" { check '' NFD ''; }
@test "already-nfc" { check $'\xc3\xa9' NFC $'\xc3\xa9'; }
@test "already-nfd" { check $'e\xcc\x81' NFD $'e\xcc\x81'; }
@test "mixed-nfc" { check $'cafe\xcc\x81 caf\xc3\xa9' NFC $'caf\xc3\xa9 caf\xc3\xa9'; }
@test "mixed-nfd" { check $'cafe\xcc\x81 caf\xc3\xa9' NFD $'cafe\xcc\x81 cafe\xcc\x81'; }
@test "upper-grave" { check $'E\xcc\x80' NFC $'\xc3\x88'; }
@test "diaeresis" { check $'u\xcc\x88ber' NFC $'\xc3\xbcber'; }
@test "circumflex" { check $'\xc3\xae' NFD $'i\xcc\x82'; }
@test "umlauts" { check $'\xc3\x84\xc3\x96\xc3\x9c\xc3\xa4\xc3\xb6\xc3\xbc' NFD $'A\xcc\x88O\xcc\x88U\xcc\x88a\xcc\x88o\xcc\x88u\xcc\x88'; }
@test "spacing-accent" { check $'\xc2\xb4e' NFC $'\xc2\xb4e'; }
@test "no-break-space" { check $'a\xc2\xa0b' NFD $'a\xc2\xa0b'; }
@test "sharp-s" { check $'stra\xc3\x9fe' NFD $'stra\xc3\x9fe'; }
