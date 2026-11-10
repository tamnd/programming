#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/base64-encode-and-decode/.

# Base64 with the two alphabets of RFC 4648: "standard" pads with "=",
# and "url" uses "-" and "_" for "+" and "/" and has no padding.

_B64_ALNUM='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'

# b64_encode ALPHABET: reads bytes on stdin and prints their Base64 text.
b64_encode() {
  local text
  case $1 in
    standard | url) ;;
    *)
      echo "b64_encode: unknown alphabet: $1" >&2
      return 2
      ;;
  esac
  # GNU base64 breaks the output into lines of 76 characters, and BSD
  # base64 does not. Remove every newline, so both give one line.
  text=$(base64 | tr -d '\n') || return
  if [[ $1 == url ]]; then
    text=${text//+/-}
    text=${text//\//_}
    text=${text%%=*}
  fi
  printf '%s\n' "$text"
}

# b64_decode TEXT ALPHABET: writes the bytes of TEXT to stdout. TEXT must be
# exactly what b64_encode prints for some bytes, or the status is 1.
b64_decode() {
  local text=$1 c re
  # The pattern checks the alphabet, the length and the padding. The last
  # data character must have zero pad bits: its value is a multiple of 16
  # (A, Q, g, w) before "==", and a multiple of 4 before "=".
  case $2 in
    standard)
      c="[${_B64_ALNUM}+/]"
      re="^($c{4})*($c[AQgw]==|$c{2}[AEIMQUYcgkosw048]=)?\$"
      ;;
    url)
      c="[${_B64_ALNUM}_-]"
      re="^($c{4})*($c[AQgw]|$c{2}[AEIMQUYcgkosw048])?\$"
      ;;
    *)
      echo "b64_decode: unknown alphabet: $2" >&2
      return 2
      ;;
  esac
  if ! [[ $text =~ $re ]]; then
    echo "b64_decode: invalid base64 text" >&2
    return 1
  fi
  text=${text//-/+}
  text=${text//_//}
  while ((${#text} % 4)); do
    text+='='
  done
  printf '%s' "$text" | base64 -d
}
