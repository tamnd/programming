# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

# _grapheme_kind C: sets K to the Grapheme_Cluster_Break kind of the code
# point C: cr, lf, control, extend, zwj, ri, pict (Extended_Pictographic) or
# other.
_grapheme_kind() {
  local c=$1
  if ((c == 0x0D)); then
    K=cr
  elif ((c == 0x0A)); then
    K=lf
  elif ((c < 0x20 || (c >= 0x7F && c <= 0x9F) || c == 0xAD)); then
    K=control
  elif (((c >= 0x300 && c <= 0x36F) || (c >= 0xFE00 && c <= 0xFE0F) || (c >= 0x1F3FB && c <= 0x1F3FF))); then
    K=extend
  elif ((c == 0x200D)); then
    K=zwj
  elif ((c >= 0x1F1E6 && c <= 0x1F1FF)); then
    K=ri
  elif ((c == 0xA9 || c == 0xAE || c == 0x2764 || (c >= 0x1F400 && c <= 0x1F4FD) || (c >= 0x1F600 && c <= 0x1F64F))); then
    K=pict
  else
    K=other
  fi
}

# count_graphemes S: prints the number of extended grapheme clusters in the
# UTF-8 string S. In the C locale, ${#S} counts bytes, so the function decodes
# the code points from the bytes.
count_graphemes() {
  (
    LC_ALL=C
    s=$1
    len=${#s}
    n=0 i=0 prev='' ri_run=0 pict=0 zwj_after_pict=0
    while ((i < len)); do
      printf -v b '%d' "'${s:i:1}"
      ((b < 0)) && ((b += 256))
      if ((b < 0x80)); then
        m=1
      elif ((b < 0xE0)); then
        m=2
      elif ((b < 0xF0)); then
        m=3
      else
        m=4
      fi
      ((c = m == 1 ? b : b & (0x7F >> m)))
      for ((j = 1; j < m; j++)); do
        printf -v b '%d' "'${s:i+j:1}"
        ((b < 0)) && ((b += 256))
        ((c = (c << 6) | (b & 0x3F)))
      done
      ((i += m))
      _grapheme_kind "$c"
      join=0
      if [[ -z $prev ]]; then
        join=0
      elif [[ $prev == cr && $K == lf ]]; then
        join=1 # GB3
      elif [[ $prev =~ ^(cr|lf|control)$ || $K =~ ^(cr|lf|control)$ ]]; then
        join=0 # GB4, GB5
      elif [[ $K == extend || $K == zwj ]]; then
        join=1 # GB9
      elif [[ $K == pict ]] && ((zwj_after_pict)); then
        join=1 # GB11
      elif [[ $K == ri ]] && ((ri_run % 2 == 1)); then
        join=1 # GB12, GB13
      fi
      ((join)) || ((n += 1))
      zwj_after_pict=0
      [[ $K == zwj ]] && ((pict)) && zwj_after_pict=1
      if [[ $K == pict ]]; then
        pict=1
      elif [[ $K != extend ]]; then
        pict=0
      fi
      if [[ $K == ri ]]; then
        ((ri_run += 1))
      else
        ri_run=0
      fi
      prev=$K
    done
    echo "$n"
  )
}
