```bash run id=read-loop
# read -r removes the LF, but the CR of a CRLF stays in the line.
printf 'one\ntwo\r\nthree' | while IFS= read -r line; do
  printf '%q\n' "$line"
done
echo "---"
# read fails on a last line with no LF, so the loop above lost "three".
printf 'one\ntwo\r\nthree' | while IFS= read -r line || [ -n "$line" ]; do
  printf '%q\n' "${line%$'\r'}"
done
echo "---"
# A script saved with CRLF endings: the CR is part of the value of x.
printf 'x=1\r\necho "x has ${#x} characters"\n' > /tmp/eol-crlf.sh
bash /tmp/eol-crlf.sh
rm -f /tmp/eol-crlf.sh
```

```output for=read-loop
one
$'two\r'
---
one
two
three
---
x has 2 characters
```

- Verdict: native. `read -r` removes the LF, and the CR of a CRLF stays in the variable. `read` returns a failure status for a last line with no LF, so a loop needs `|| [ -n "$line" ]` to keep it. A script saved with CRLF endings gives each command an extra CR.
