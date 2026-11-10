```bash run id=player
#!/usr/bin/env bash
set -euo pipefail

state=stopped

# One case statement on "state:event" is the transition table.
handle() {
  local event=$1 next action
  case "$state:$event" in
    stopped:play) next=playing action="start at 0:00" ;;
    playing:pause) next=paused action="hold the position" ;;
    paused:play) next=playing action=resume ;;
    playing:stop | paused:stop) next=stopped action=rewind ;;
    *) next=$state action=ignored ;;
  esac
  echo "$event: $action ($state -> $next)"
  state=$next
}

for event in play pause play stop pause; do
  handle "$event"
done
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. The shell has no objects, so the state is a string, and a `case` on the string `"state:event"` is the table. This is the enum and match form of the node page.
- `|` joins two patterns in one arm, and `*` is the default arm.
- A shell function can be a state too: the variable holds a function name, and `"$state" "$event"` calls it. The `case` form keeps the whole machine in one place.
