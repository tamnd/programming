```bash run id=strings
a="hello"
b="hel"
b+="lo"
if [[ $a == "$b" ]]; then
  echo "same text"
fi

pattern='h*'
if [[ $a == $pattern ]]; then
  echo "unquoted: matches as a pattern"
fi
if [[ $a == "$pattern" ]]; then
  echo "quoted: matches as text"
else
  echo "quoted: does not match as text"
fi

if [[ 1 -eq 01 ]]; then
  echo "1 -eq 01: equal as numbers"
fi
if [[ 1 != 01 ]]; then
  echo "1 != 01: different as text"
fi
```

```output for=strings
same text
unquoted: matches as a pattern
quoted: does not match as text
1 -eq 01: equal as numbers
1 != 01: different as text
```

- Verdict: idiom. Shell values are strings. `==` inside `[[ ]]` matches the left side against a pattern on the right side. A quoted right side matches as plain text. `-eq` compares integers.
- Bash has no objects, so it has no identity test.

```bash wrong id=pattern
expected='*'
actual="anything"
if [[ $actual == $expected ]]; then
  echo "match"
else
  echo "no match"
fi
```

```output for=pattern
match
```

The right side is not quoted, so `*` is a pattern that matches any string. Quote the right side: `[[ $actual == "$expected" ]]`.
