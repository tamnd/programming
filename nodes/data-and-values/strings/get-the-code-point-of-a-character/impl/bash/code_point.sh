# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

# code_point C: prints the code point of the one character in C. In the C
# locale, printf '%d' "'x" gives the value of one byte, so the function decodes
# the UTF-8 sequence from its bytes.
code_point() {
  (
    LC_ALL=C
    c=$1
    n=${#c}
    printf -v b0 '%d' "'${c:0:1}"
    ((b0 < 0)) && ((b0 += 256))
    if ((n == 1)); then
      echo "$b0"
      return
    fi
    ((cp = b0 & (0x7f >> n)))
    for ((i = 1; i < n; i++)); do
      printf -v b '%d' "'${c:i:1}"
      ((b < 0)) && ((b += 256))
      ((cp = (cp << 6) | (b & 0x3f)))
    done
    echo "$cp"
  )
}
