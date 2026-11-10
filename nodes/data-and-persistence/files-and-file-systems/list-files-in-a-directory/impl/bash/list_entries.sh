# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/list-files-in-a-directory/.

# list_entries [-r] DIR prints the relative path of each entry under DIR, one
# per line, sorted by byte. With -r it lists the whole tree. A directory has a
# "/" at the end. A symbolic link is listed, but it is never followed. It
# returns 1 with a message on stderr if DIR is missing or is not a directory.
list_entries() {
  local recursive=0
  if [[ $1 == -r ]]; then
    recursive=1
    shift
  fi
  if [[ ! -e $1 && ! -L $1 ]]; then
    printf 'list_entries: %s: No such file or directory\n' "$1" >&2
    return 1
  fi
  if [[ ! -d $1 ]]; then
    printf 'list_entries: %s: Not a directory\n' "$1" >&2
    return 1
  fi
  _list_entries_walk "$1" "" "$recursive" | LC_ALL=C sort
}

# _list_entries_walk DIR PREFIX RECURSIVE runs in a subshell, so the shopt
# settings do not leak into the caller.
_list_entries_walk() (
  # dotglob matches names that start with ".", but never "." and "..".
  # nullglob makes "*" expand to nothing in an empty directory.
  shopt -s dotglob nullglob
  local path rel
  for path in "$1"/*; do
    rel=$2${path##*/}
    # -d follows a symbolic link, so test -L too.
    if [[ -d $path && ! -L $path ]]; then
      printf '%s/\n' "$rel"
      if (($3)); then
        _list_entries_walk "$path" "$rel/" 1
      fi
    else
      printf '%s\n' "$rel"
    fi
  done
)
