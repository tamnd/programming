#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/apply-a-patch/.

# apply_patch FILE PATCH
# Prints the text that the unified diff in the file PATCH makes from the file
# FILE, with no fuzz and no offset. Each hunk must match the lines of FILE at
# the start of its old range. Prints nothing on stdout and returns 1 if a file
# cannot be read, 2 if a hunk does not match, and 3 if the patch is malformed.
apply_patch() {
  local path
  for path in "$1" "$2"; do
    [[ -r $path && ! -d $path ]] || { printf 'apply_patch: cannot read %s\n' "$path" >&2; return 1; }
  done
  # LC_ALL=C compares the bytes of the lines.
  local LC_ALL=C
  local -a lines p marks texts numbers ends
  # Without -t, each line keeps its LF, and a last line with no LF has none.
  mapfile lines <"$1"
  # With -t, the LF goes. A CR stays in its line.
  mapfile -t p <"$2"
  local hunk_re='^@@ -([0-9]{1,18})(,([0-9]{1,18}))? \+([0-9]{1,18})(,([0-9]{1,18}))? @@'
  local out='' nl=${#lines[@]} np=${#p[@]}
  local cur=0 hunks=0 i=0 k pos old_start old_count new_count old_left new_left mark text line
  while ((i < np)); do
    if [[ ${p[i]} != @@* ]]; then
      ((i += 1)) # a header line, or another line outside a hunk
      continue
    fi
    [[ ${p[i]} =~ $hunk_re ]] || { _patch_error 3 $((i + 1)); return; }
    ((hunks += 1))
    old_start=$((10#${BASH_REMATCH[1]}))
    old_count=$((10#${BASH_REMATCH[3]:-1}))
    new_count=$((10#${BASH_REMATCH[6]:-1}))
    # An empty range starts after the line old_start.
    pos=$((old_count > 0 ? old_start - 1 : old_start))
    ((pos >= 0)) || { _patch_error 3 $((i + 1)); return; }
    ((pos >= cur && pos <= nl)) || { _patch_error 2 $((i + 1)); return; }
    for ((; cur < pos; cur++)); do out+=${lines[cur]}; done
    ((i += 1))
    # The mark, the text, the number in the patch and the end of each line:
    # an LF, or nothing before a backslash line.
    marks=() texts=() numbers=() ends=()
    old_left=$old_count new_left=$new_count
    while ((old_left > 0 || new_left > 0)); do
      ((i < np)) || { _patch_error 3 "$i"; return; }
      line=${p[i]}
      mark=${line:0:1} text=${line:1}
      [[ -n $line ]] || mark=' '
      if [[ $mark == '\' ]]; then
        ((${#marks[@]} > 0)) || { _patch_error 3 $((i + 1)); return; }
        ends[-1]='' # the line before has no LF
        ((i += 1))
        continue
      fi
      case $mark in
        ' ') old_left=$((old_left - 1)) new_left=$((new_left - 1)) ;;
        -) old_left=$((old_left - 1)) ;;
        +) new_left=$((new_left - 1)) ;;
        *) _patch_error 3 $((i + 1)); return ;;
      esac
      ((old_left >= 0 && new_left >= 0)) || { _patch_error 3 $((i + 1)); return; }
      marks+=("$mark") texts+=("$text") numbers+=($((i + 1))) ends+=($'\n')
      ((i += 1))
    done
    if ((i < np && ${#marks[@]} > 0)) && [[ ${p[i]} == '\'* ]]; then
      ends[-1]=''
      ((i += 1))
    fi
    for ((k = 0; k < ${#marks[@]}; k++)); do
      line=${texts[k]}${ends[k]}
      if [[ ${marks[k]} != + ]]; then
        ((cur < nl)) && [[ ${lines[cur]} == "$line" ]] || { _patch_error 2 "${numbers[k]}"; return; }
        ((cur += 1))
      fi
      [[ ${marks[k]} == - ]] || out+=$line
    done
  done
  ((hunks > 0)) || { _patch_error 3 "$np"; return; }
  for ((; cur < nl; cur++)); do out+=${lines[cur]}; done
  printf '%s' "$out"
}

# _patch_error STATUS LINE prints the error and returns STATUS.
_patch_error() {
  local kind=malformed
  (($1 == 2)) && kind=mismatch
  printf 'apply_patch: %s at line %s of the patch\n' "$kind" "$2" >&2
  return "$1"
}
