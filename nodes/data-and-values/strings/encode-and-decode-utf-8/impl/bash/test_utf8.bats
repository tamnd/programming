# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/utf8.sh"
}

check_encode() { # check_encode CP WANT
  run encode_utf8 "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

check_decode() { # check_decode WANT BYTE...
  local want=$1
  shift
  run decode_utf8 "$@"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ]
}

# The bytes are decimal numbers with one space between them.
@test "encode ascii-a" { check_encode 65 '65'; }
@test "encode euro" { check_encode 8364 '226 130 172'; }
@test "encode emoji" { check_encode 128578 '240 159 153 130'; }
@test "encode nul" { check_encode 0 '0'; }
@test "encode one-byte-max" { check_encode 127 '127'; }
@test "encode two-byte-min" { check_encode 128 '194 128'; }
@test "encode two-byte" { check_encode 233 '195 169'; }
@test "encode two-byte-max" { check_encode 2047 '223 191'; }
@test "encode three-byte-min" { check_encode 2048 '224 160 128'; }
@test "encode three-byte-max" { check_encode 65535 '239 191 191'; }
@test "encode four-byte-min" { check_encode 65536 '240 144 128 128'; }
@test "encode max" { check_encode 1114111 '244 143 191 191'; }
@test "decode ascii-a" { check_decode 65 65; }
@test "decode e-acute" { check_decode 233 195 169; }
@test "decode emoji" { check_decode 128578 240 159 153 130; }
@test "decode max" { check_decode 1114111 244 143 191 191; }
@test "decode replacement" { check_decode 65533 239 191 189; }
@test "decode empty" { check_decode -1; }
@test "decode stray-continuation" { check_decode -1 128; }
@test "decode truncated" { check_decode -1 195; }
@test "decode bad-continuation" { check_decode -1 195 40; }
@test "decode overlong-two" { check_decode -1 192 129; }
@test "decode overlong-three" { check_decode -1 224 128 128; }
@test "decode surrogate" { check_decode -1 237 160 128; }
@test "decode above-max" { check_decode -1 244 144 128 128; }
@test "decode two-sequences" { check_decode -1 65 66; }
