#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-file/.

setup() {
  source "$BATS_TEST_DIRNAME/write_text.sh"
  dir=$BATS_TEST_TMPDIR/d
  mkdir "$dir"
  path=$dir/out.txt
}

# check TEXT MODE BEFORE WANT. BEFORE is "-" when no file is there.
check() {
  if [ "$3" != - ]; then
    printf '%s' "$3" >"$path"
  fi
  write_text "$path" "$1" "$2"
  printf '%s' "$4" >"$BATS_TEST_TMPDIR/want"
  cmp "$path" "$BATS_TEST_TMPDIR/want"
  [ "$(ls -A "$dir")" = out.txt ]
}

non_ascii=$'na\xc3\xafve caf\xc3\xa9\n\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\n\xf0\x9f\x99\x82\n'

@test "new-file" { check $'alpha\nbeta\n' truncate - $'alpha\nbeta\n'; }
@test "append-existing" { check $'two\n' append $'one\n' $'one\ntwo\n'; }
@test "atomic-replace" { check $'new\n' atomic $'old\n' $'new\n'; }
@test "truncate-longer" { check $'short\n' truncate $'a much longer old line\n' $'short\n'; }
@test "empty-truncate" { check '' truncate $'old\n' ''; }
@test "empty-new" { check '' truncate - ''; }
@test "append-missing" { check $'first\n' append - $'first\n'; }
@test "append-no-newline" { check two append one onetwo; }
@test "non-ascii" { check "$non_ascii" truncate - "$non_ascii"; }
@test "crlf" { check $'one\r\ntwo\r\n' truncate - $'one\r\ntwo\r\n'; }
@test "mixed-endings" { check $'lf\ncrlf\r\ncr\rend' truncate - $'lf\ncrlf\r\ncr\rend'; }
@test "backslashes" { check $'C:\\temp\\new\n' truncate - $'C:\\temp\\new\n'; }
@test "format-chars" { check $'-n 100% %s\n' truncate - $'-n 100% %s\n'; }
@test "atomic-new" { check $'new\n' atomic - $'new\n'; }
@test "atomic-empty" { check '' atomic $'old\n' ''; }
@test "atomic-non-ascii" { check "$non_ascii" atomic $'old\n' "$non_ascii"; }

missing() {
  run write_text "$dir/missing/out.txt" $'x\n' "$1"
  [ "$status" -ne 0 ]
  [ -z "$(ls -A "$dir")" ]
}

@test "missing-dir-truncate" { missing truncate; }
@test "missing-dir-append" { missing append; }
@test "missing-dir-atomic" { missing atomic; }

@test "atomic-onto-directory" {
  mkdir "$path"
  run write_text "$path" $'x\n' atomic
  [ "$status" -ne 0 ]
  [ -d "$path" ]
  [ -z "$(ls -A "$path")" ]
  [ "$(ls -A "$dir")" = out.txt ]
}
