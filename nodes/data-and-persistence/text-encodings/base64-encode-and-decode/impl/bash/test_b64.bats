#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/base64-encode-and-decode/.

setup() {
  source "$BATS_TEST_DIRNAME/b64.sh"
  set -o pipefail
}

# unhex HEX: writes the bytes of HEX to stdout.
unhex() {
  local hex=$1 escapes=''
  while [[ -n $hex ]]; do
    escapes+="\\x${hex:0:2}"
    hex=${hex:2}
  done
  printf '%b' "$escapes"
}

# tohex: prints the bytes on stdin in hexadecimal.
tohex() {
  od -An -tx1 -v | tr -d ' \n'
}

# check_encode HEX ALPHABET WANT: checks the text and the round trip.
check_encode() {
  local text back
  text=$(unhex "$1" | b64_encode "$2")
  [[ $text == "$3" ]] || { echo "got '$text', want '$3'" >&2; return 1; }
  back=$(b64_decode "$text" "$2" | tohex)
  [[ $back == "$1" ]] || { echo "round trip gave '$back'" >&2; return 1; }
}

# check_decode TEXT ALPHABET WANT: WANT is the bytes in hexadecimal.
check_decode() {
  local got
  got=$(b64_decode "$1" "$2" | tohex)
  [[ $got == "$3" ]] || { echo "got '$got', want '$3'" >&2; return 1; }
}

# check_invalid TEXT ALPHABET: decode must fail with status 1 and write no bytes.
check_invalid() {
  local got status=0
  got=$(b64_decode "$1" "$2" 2>/dev/null | tohex) || status=$?
  [[ $status -eq 1 && -z $got ]] || { echo "status $status, bytes '$got'" >&2; return 1; }
}

# The bytes are written in hexadecimal.
@test "encode rfc-f" { check_encode '66' standard 'Zg=='; }
@test "encode rfc-fo" { check_encode '666f' standard 'Zm8='; }
@test "encode rfc-foo" { check_encode '666f6f' standard 'Zm9v'; }
@test "encode rfc-foob" { check_encode '666f6f62' standard 'Zm9vYg=='; }
@test "encode rfc-fooba" { check_encode '666f6f6261' standard 'Zm9vYmE='; }
@test "encode rfc-foobar" { check_encode '666f6f626172' standard 'Zm9vYmFy'; }
@test "encode hello-standard" { check_encode '48656c6c6f2c20776f726c6421' standard 'SGVsbG8sIHdvcmxkIQ=='; }
@test "encode hello-url" { check_encode '48656c6c6f2c20776f726c6421' url 'SGVsbG8sIHdvcmxkIQ'; }
@test "encode empty-standard" { check_encode '' standard ''; }
@test "encode empty-url" { check_encode '' url ''; }
@test "encode two-pad-chars" { check_encode '00' standard 'AA=='; }
@test "encode one-pad-char" { check_encode '0000' standard 'AAA='; }
@test "encode zero-bytes" { check_encode '000000' standard 'AAAA'; }
@test "encode plus-and-slash" { check_encode 'fbff' standard '+/8='; }
@test "encode minus-and-underscore" { check_encode 'fbff' url '-_8'; }
@test "encode all-62-standard" { check_encode 'fbefbe' standard '++++'; }
@test "encode all-62-url" { check_encode 'fbefbe' url '----'; }
@test "encode all-63-standard" { check_encode 'ffffff' standard '////'; }
@test "encode all-63-url" { check_encode 'ffffff' url '____'; }
@test "encode url-one-byte" { check_encode '66' url 'Zg'; }
@test "encode url-two-bytes" { check_encode '666f' url 'Zm8'; }
@test "encode non-ascii-standard" { check_encode 'c3a9e282acf09f9982' standard 'w6nigqzwn5mC'; }
@test "encode non-ascii-url" { check_encode 'c3a9e282acf09f9982' url 'w6nigqzwn5mC'; }

@test "decode rfc-empty" { check_decode '' standard ''; }
@test "decode rfc-f" { check_decode 'Zg==' standard '66'; }
@test "decode rfc-fo" { check_decode 'Zm8=' standard '666f'; }
@test "decode rfc-foo" { check_decode 'Zm9v' standard '666f6f'; }
@test "decode rfc-foob" { check_decode 'Zm9vYg==' standard '666f6f62'; }
@test "decode rfc-fooba" { check_decode 'Zm9vYmE=' standard '666f6f6261'; }
@test "decode rfc-foobar" { check_decode 'Zm9vYmFy' standard '666f6f626172'; }
@test "decode empty-url" { check_decode '' url ''; }
@test "decode url-foob" { check_decode 'Zm9vYg' url '666f6f62'; }
@test "decode url-fooba" { check_decode 'Zm9vYmE' url '666f6f6261'; }
@test "decode url-foobar" { check_decode 'Zm9vYmFy' url '666f6f626172'; }
@test "decode plus-and-slash" { check_decode '+/8=' standard 'fbff'; }
@test "decode minus-and-underscore" { check_decode '-_8' url 'fbff'; }
@test "decode all-63-url" { check_decode '____' url 'ffffff'; }
@test "decode zero-bytes" { check_decode 'AAAA' standard '000000'; }
@test "decode non-ascii-standard" { check_decode 'w6nigqzwn5mC' standard 'c3a9e282acf09f9982'; }
@test "decode non-ascii-url" { check_decode 'w6nigqzwn5mC' url 'c3a9e282acf09f9982'; }

@test "invalid plus-in-url" { check_invalid '+/8' url; }
@test "invalid minus-in-standard" { check_invalid '-_8=' standard; }
@test "invalid bang" { check_invalid 'Zm9v!' standard; }
@test "invalid non-ascii-char" { check_invalid 'Zm9vé===' standard; }
@test "invalid space" { check_invalid 'Zm9v YmFy' standard; }
@test "invalid line-feed" { check_invalid $'Zm9v\nYmFy' standard; }
@test "invalid trailing-crlf" { check_invalid $'Zm9vYmFy\r\n' standard; }
@test "invalid line-feed-url" { check_invalid $'Zm9v\nYmFy' url; }
@test "invalid missing-padding" { check_invalid 'Zm8' standard; }
@test "invalid missing-one-pad" { check_invalid 'Zg=' standard; }
@test "invalid excess-padding" { check_invalid 'Zm9v=' standard; }
@test "invalid padding-in-middle" { check_invalid 'Zm=9' standard; }
@test "invalid three-pads" { check_invalid 'Z===' standard; }
@test "invalid only-padding" { check_invalid '====' standard; }
@test "invalid padding-then-data" { check_invalid 'Zg==Zm8=' standard; }
@test "invalid padding-in-url" { check_invalid 'Zm8=' url; }
@test "invalid length-1-mod-4-standard" { check_invalid 'Zm9vY' standard; }
@test "invalid length-1-mod-4-url" { check_invalid 'Zm9vY' url; }
@test "invalid one-char-url" { check_invalid 'Z' url; }
@test "invalid pad-bits-one-byte" { check_invalid 'QR==' standard; }
@test "invalid pad-bits-two-bytes" { check_invalid 'Zm9=' standard; }
@test "invalid pad-bits-url" { check_invalid 'Zm9' url; }
