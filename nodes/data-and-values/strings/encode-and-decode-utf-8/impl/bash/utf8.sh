# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

# encode_utf8 CP: prints the UTF-8 bytes of the code point CP as decimal
# numbers, with one space between them.
encode_utf8() {
  local cp=$(($1))
  if ((cp < 0x80)); then
    echo "$cp"
  elif ((cp < 0x800)); then
    echo "$((0xc0 | cp >> 6)) $((0x80 | (cp & 0x3f)))"
  elif ((cp < 0x10000)); then
    echo "$((0xe0 | cp >> 12)) $((0x80 | (cp >> 6 & 0x3f))) $((0x80 | (cp & 0x3f)))"
  else
    echo "$((0xf0 | cp >> 18)) $((0x80 | (cp >> 12 & 0x3f))) $((0x80 | (cp >> 6 & 0x3f))) $((0x80 | (cp & 0x3f)))"
  fi
}

# decode_utf8 BYTE...: prints the code point of the one UTF-8 sequence in the
# arguments, or -1 if they are not exactly one well-formed sequence.
decode_utf8() {
  local b0=${1:--1} len cp min i b
  if ((b0 < 0)); then
    echo -1
    return
  elif ((b0 < 0x80)); then
    len=1 cp=$b0 min=0
  elif ((b0 >= 0xc0 && b0 < 0xe0)); then
    len=2 cp=$((b0 & 0x1f)) min=$((0x80))
  elif ((b0 >= 0xe0 && b0 < 0xf0)); then
    len=3 cp=$((b0 & 0x0f)) min=$((0x800))
  elif ((b0 >= 0xf0 && b0 < 0xf8)); then
    len=4 cp=$((b0 & 0x07)) min=$((0x10000))
  else
    echo -1
    return
  fi
  if (($# != len)); then
    echo -1
    return
  fi
  shift
  for b in "$@"; do
    if (((b & 0xc0) != 0x80)); then
      echo -1
      return
    fi
    cp=$((cp << 6 | (b & 0x3f)))
  done
  if ((cp < min || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff))); then
    echo -1
    return
  fi
  echo "$cp"
}
