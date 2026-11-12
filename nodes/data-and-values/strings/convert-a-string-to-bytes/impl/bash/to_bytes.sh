# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

# to_bytes S: prints the UTF-8 bytes of S as numbers, with one space between
# them. In the C locale, ${s:i:1} is one byte, and printf '%d' "'x" gives its
# value. The value is taken from 0 to 255 with & 255.
to_bytes() {
  (
    LC_ALL=C
    s=$1
    out=
    for ((i = 0; i < ${#s}; i++)); do
      printf -v b '%d' "'${s:i:1}"
      out+="${out:+ }$((b & 255))"
    done
    printf '%s' "$out"
  )
}
