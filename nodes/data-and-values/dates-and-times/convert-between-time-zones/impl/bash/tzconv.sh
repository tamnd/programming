#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-between-time-zones/.

# The functions need GNU date for -d @SECONDS and %:z.

# _tz_offset ZONE SECONDS
# Prints the UTC offset of ZONE, in seconds, at SECONDS after the epoch.
_tz_offset() {
  local z sign=1
  z=$(TZ="$1" date -d "@$2" +%z) || return 1
  [[ $z == -* ]] && sign=-1
  echo "$((sign * (10#${z:1:2} * 3600 + 10#${z:3:2} * 60)))"
}

# _tz_check ZONE
# With an unknown TZ value, date uses UTC and reports no error, so the zone
# file is checked first.
_tz_check() {
  case $1 in
    "" | /* | *..*) return 1 ;;
  esac
  [[ -r ${TZDIR:-/usr/share/zoneinfo}/$1 ]]
}

# convert_time_zone YEAR MONTH DAY HOUR MINUTE SECOND FROM_ZONE TO_ZONE
# Prints the local time in TO_ZONE as YYYY-MM-DDTHH:MM:SS+HH:MM. A time in a
# gap moves forward by the length of the gap, and a time in an overlap gives
# the earlier of its two instants.
convert_time_zone() {
  local from=$7 to=$8 t before after offset u instant=
  _tz_check "$from" || { echo "unknown zone: $from" >&2; return 1; }
  _tz_check "$to" || { echo "unknown zone: $to" >&2; return 1; }
  t=$(date -u -d "$(printf '%04d-%02d-%02d %02d:%02d:%02d' "$1" "$2" "$3" "$4" "$5" "$6")" +%s) || return 1
  before=$(_tz_offset "$from" "$((t - 86400))")
  after=$(_tz_offset "$from" "$((t + 86400))")
  for offset in "$before" "$after"; do
    u=$((t - offset))
    if [[ $(_tz_offset "$from" "$u") == "$offset" ]] && [[ -z $instant || $u -lt $instant ]]; then
      instant=$u
    fi
  done
  : "${instant:=$((t - before))}"
  TZ="$to" date -d "@$instant" +%Y-%m-%dT%H:%M:%S%:z
}
