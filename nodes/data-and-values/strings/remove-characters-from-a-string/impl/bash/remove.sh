# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-characters-from-a-string/.

# remove_chars S CHARS: prints S without the code points of CHARS. In the C
# locale, ${chars:i:1} is one byte, so the loop finds the length of each UTF-8
# sequence from its first byte. Then ${s//"$c"/} removes each copy of the whole
# sequence. The quotes make the sequence a plain string, not a pattern.
remove_chars() {
  (
    LC_ALL=C
    s=$1
    chars=$2
    i=0
    while ((i < ${#chars})); do
      b=${chars:i:1}
      n=1
      if [[ $b > $'\xef' ]]; then
        n=4
      elif [[ $b > $'\xdf' ]]; then
        n=3
      elif [[ $b > $'\x7f' ]]; then
        n=2
      fi
      c=${chars:i:n}
      s=${s//"$c"/}
      ((i += n))
    done
    printf '%s' "$s"
  )
}
