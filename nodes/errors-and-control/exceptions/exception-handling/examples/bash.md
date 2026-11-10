```bash run id=ports
parse_port() {
  local text=$1
  if [[ ! $text =~ ^[0-9]+$ ]]; then
    return 2
  fi
  if (( text < 1 || text > 65535 )); then
    return 3
  fi
  echo "$text"
}

for text in 8080 70000 abc; do
  if port=$(parse_port "$text"); then
    echo "port $port"
  else
    echo "error: status $? for $text"
  fi
done

(
  set -e
  trap 'echo "trap ERR: status $?"' ERR
  parse_port abc > /dev/null
  echo "not reached"
)
echo "subshell status $?"
```

```output
port 8080
error: status 3 for 70000
error: status 2 for abc
trap ERR: status 2
subshell status 2
```

- Verdict: idiom. Bash has no exceptions. Each command ends with an exit status, 0 for success, and `if`, `&&` and `||` test it.
- A function returns a status with `return`. Its value comes back on standard output, here through command substitution.
- `set -e` ends the shell when a command fails, and `trap ... ERR` runs a command first. A subshell `( ... )` limits the exit to the subshell, so the outer script can test its status like a handler.
- `set -e` has no effect on a command that is the test of an `if`, part of a `&&` or `||` list, or not the last command of a pipeline (see Pitfalls on the concept page).
- `trap ... EXIT` runs cleanup when the shell exits on any path.
