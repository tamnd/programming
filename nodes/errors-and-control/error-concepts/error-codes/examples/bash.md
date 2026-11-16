```bash run id=codes-bash
names=("ok" "empty" "not a number" "out of range")

# Prints the port. The exit status is the code.
parse_port() {
  local text=$1
  [ -z "$text" ] && return 1
  case $text in
    *[!0-9]*) return 2 ;;
  esac
  [ "${#text}" -gt 5 ] && return 3
  [ "$text" -gt 65535 ] && return 3
  echo "$text"
  return 0
}

for text in "8080" "" "x" "99999"; do
  port=$(parse_port "$text")
  code=$?
  if [ "$code" -ne 0 ]; then
    echo "'$text' -> error code $code (${names[$code]})"
  else
    echo "'$text' -> port $port"
  fi
done
```

```output for=codes-bash
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: native. Each command and function in Bash returns an exit status, and a status that is not zero is an error code. A function sets it with `return`, and the caller reads it in `$?`.
