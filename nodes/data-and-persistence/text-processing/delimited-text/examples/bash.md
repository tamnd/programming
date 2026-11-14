```bash run id=ifs-and-cut
line='a::b:'

# read splits at each ':' in IFS, but drops the empty field at the end.
IFS=: read -r -a f <<< "$line"
echo "read: ${#f[@]} fields"

# cut and awk count the empty field at the end.
echo "cut field 4: [$(printf '%s\n' "$line" | cut -d: -f4)]"
printf '%s\n' "$line" | awk -F: '{ print "awk: " NF " fields" }'

# The default IFS splits at runs of white space and drops the ends.
read -r -a w <<< '  a  b '
echo "white space: ${#w[@]} fields"
```

```output for=ifs-and-cut
read: 3 fields
cut field 4: []
awk: 4 fields
white space: 2 fields
```

- Verdict: native. `read` with `IFS=:` splits at each colon, but Bash drops the empty field at the end of the line, as the output shows. `cut -d:` and `awk -F:` count it. With the default `IFS`, a run of spaces is one separator. The tools have no quotes, so a field cannot hold the delimiter.
