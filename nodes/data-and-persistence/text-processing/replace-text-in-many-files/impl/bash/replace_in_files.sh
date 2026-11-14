#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/replace-text-in-many-files/.

# The awk program reads a file line by line and writes the new text to the
# file OUT. It keeps the last length(OLD) - 1 bytes in buf, because an instance
# can start there and go on in the next line. index() finds plain text, not a
# pattern. ENVIRON gives the texts as they are: awk -v changes backslashes.
_replace_in_files_awk='
function flush(keep,   i) {
  while ((i = index(buf, old)) > 0) {
    printf "%s%s", substr(buf, 1, i - 1), new > out
    buf = substr(buf, i + n)
    count++
  }
  if (length(buf) > keep) {
    printf "%s", substr(buf, 1, length(buf) - keep) > out
    buf = substr(buf, length(buf) - keep + 1)
  }
}
BEGIN { old = ENVIRON["OLD"]; new = ENVIRON["NEW"]; out = ENVIRON["OUT"]; n = length(old) }
{ buf = buf (NR > 1 ? "\n" : "") $0; flush(n - 1) }
END { if (ENVIRON["LAST_LF"] == 1) buf = buf "\n"; flush(0); print count + 0 }
'

# replace_in_files OLD NEW FILE...
# Replaces each instance of OLD with NEW in each FILE, and prints the number of
# instances in each file, one number on each line. OLD and NEW are plain text,
# not patterns, and they can contain LF. Instances do not overlap. A file with
# no instance is not written. A changed file goes to a new file in the same
# directory, and mv puts it in the place of the old one. The files are done in
# order. Returns 2 with a message if OLD is empty or a file cannot be read or
# written, and the files after it do not change. NUL bytes are not supported.
replace_in_files() {
  local old=$1 new=$2 file tmp count last_lf
  shift 2
  if [[ -z $old ]]; then
    printf 'replace_in_files: OLD is empty\n' >&2
    return 2
  fi
  for file in "$@"; do
    [[ -f $file && -r $file ]] || { printf 'replace_in_files: cannot read %s\n' "$file" >&2; return 2; }
    # awk does not tell if the last line ends with LF. $(...) removes a final
    # LF, so the result is empty if the last byte is LF.
    last_lf=0
    if [[ -s $file && -z $(tail -c 1 -- "$file") ]]; then last_lf=1; fi
    tmp=$(mktemp "$file.XXXXXX") || return 2
    # cp -p gives the new file the permissions of the old one, and awk keeps them.
    # With LC_ALL=C, awk counts bytes, not characters.
    if ! { cp -p -- "$file" "$tmp" &&
      count=$(LC_ALL=C OLD=$old NEW=$new OUT=$tmp LAST_LF=$last_lf awk "$_replace_in_files_awk" "$file"); }; then
      rm -f -- "$tmp"
      return 2
    fi
    if ((count == 0)); then
      rm -f -- "$tmp"
    elif ! mv -f -- "$tmp" "$file"; then
      rm -f -- "$tmp"
      return 2
    fi
    echo "$count"
  done
}
