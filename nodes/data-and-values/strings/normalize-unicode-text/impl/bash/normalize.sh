# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/normalize-unicode-text/.

# normalize S FORM: prints S in the form NFC or NFD, for the code points of the
# contract. The table has the 40 pairs of a vowel and a mark. Each composite is
# from U+00C0 to U+00FF, so its UTF-8 form is the byte 0xC3 and one more byte.
normalize() {
  (
    LC_ALL=C
    s=$1
    vowels=(A E I O U a e i o u)
    marks=($'\xcc\x80' $'\xcc\x81' $'\xcc\x82' $'\xcc\x88')
    upper=(0xC0 0xC1 0xC2 0xC4 0xC8 0xC9 0xCA 0xCB 0xCC 0xCD 0xCE 0xCF
      0xD2 0xD3 0xD4 0xD6 0xD9 0xDA 0xDB 0xDC)
    for ((i = 0; i < 10; i++)); do
      for ((j = 0; j < 4; j++)); do
        ((cp = upper[(i % 5) * 4 + j] + (i >= 5 ? 0x20 : 0)))
        printf -v comp "\\xc3\\x$(printf '%x' $((cp - 0x40)))"
        pair=${vowels[i]}${marks[j]}
        if [[ $2 == NFD ]]; then
          s=${s//"$comp"/"$pair"}
        else
          s=${s//"$pair"/"$comp"}
        fi
      done
    done
    printf '%s\n' "$s"
  )
}
