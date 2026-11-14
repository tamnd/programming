# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/align-columns-of-text/.

# Aligns the fields of the text on stdin in columns and writes the result to
# stdout. A $ separates two fields, and $1 is left, right or center.
align_columns() {
  local align=$1
  case $align in
    left | right | center) ;;
    *)
      echo "align_columns: align must be left, right or center: $align" >&2
      return 2
      ;;
  esac
  # In the C locale, bash works on bytes, so ${#t} counts bytes and no byte
  # of a UTF-8 text is a bad character. A $ byte is always a $ in UTF-8.
  local LC_ALL=C
  local -a lines=() widths=() fields=()
  local line width j n=0 pad left out
  # read returns 1 for a last line with no LF, but it sets the line.
  while IFS= read -r line || [[ -n $line ]]; do
    lines[n++]=$line
  done
  # In bash 3.2, ${lines[i]} walks the array from the start, but a for loop
  # over "${lines[@]}" does not. The + form keeps set -u quiet when there
  # are no lines.
  for line in ${lines[@]+"${lines[@]}"}; do
    _align_columns_split "$line"
    for ((j = 0; j < ${#fields[@]}; j++)); do
      _align_columns_width "${fields[j]}"
      if ((width > ${widths[j]:-0})); then
        widths[j]=$width
      fi
    done
  done
  for line in ${lines[@]+"${lines[@]}"}; do
    _align_columns_split "$line"
    out=
    for ((j = 0; j < ${#fields[@]}; j++)); do
      _align_columns_width "${fields[j]}"
      pad=$((${widths[j]:-0} - width))
      case $align in
        left) left=0 ;;
        right) left=$pad ;;
        center) left=$((pad / 2)) ;;
      esac
      ((j > 0)) && out+=' '
      # printf %*s counts bytes, so it pads an empty string, not the field.
      printf -v out '%s%*s%s' "$out" "$left" '' "${fields[j]}"
      if ((j + 1 < ${#fields[@]})); then
        printf -v out '%s%*s' "$out" "$((pad - left))" ''
      fi
    done
    printf '%s\n' "$out"
  done
}

# Puts the fields of $1 in the array fields. A $ at the end does not start a
# new field, and an empty line has no fields.
_align_columns_split() {
  local rest=$1
  fields=()
  while [[ -n $rest ]]; do
    if [[ $rest == *'$'* ]]; then
      fields+=("${rest%%\$*}")
      rest=${rest#*\$}
    else
      fields+=("$rest")
      rest=
    fi
  done
}

# Puts the number of code points in the UTF-8 text $1 in width. The caller
# sets the C locale, so ${#t} counts bytes. The code first removes each byte
# from 0x80 to 0xBF, which continues a code point.
_align_columns_width() {
  local t=${1//[$'\x80'-$'\xbf']/}
  width=${#t}
}
