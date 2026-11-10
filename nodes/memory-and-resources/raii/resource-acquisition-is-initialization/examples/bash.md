```bash run id=trap-exit
#!/usr/bin/env bash
set -eu

copy() (
  workdir=$(mktemp -d ./copy.XXXXXX)
  trap 'rm -rf "$workdir"; echo "removed workdir"' EXIT
  echo "made workdir"
  if [ "$1" = fail ]; then
    echo "copy failed" >&2
    exit 1
  fi
  echo "copied"
)

copy ok
copy fail 2>/dev/null || echo "copy returned $?"
leftover=$(find . -maxdepth 1 -name 'copy.*' | wc -l)
echo "workdirs left: $((leftover))"
```

```output
made workdir
copied
removed workdir
made workdir
removed workdir
copy returned 1
workdirs left: 0
```

- Verdict: idiom. `trap ... EXIT` runs its commands when the shell exits. The `exit` builtin runs the EXIT trap before the shell ends.
- The body of `copy` is a subshell `( ... )`, so the trap belongs to the subshell and runs when the function returns.
- The second call exits with status 1, and the trap still removes its directory. The last line shows that no work directory is left.
- `mktemp -d` makes the directory before the trap is set. Set the trap at once after the acquisition, so no command runs between them.
