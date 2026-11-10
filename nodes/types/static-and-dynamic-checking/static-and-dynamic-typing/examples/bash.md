```bash run id=types
count=3
echo "$((count + 1))"
echo "$count$count"

declare -i n
n="abc"
echo "$n"
n="2 + 3"
echo "$n"
declare -p n
```

```output
4
33
0
5
declare -i n="5"
```

- Verdict: native. A Bash variable holds a string. Arithmetic expansion `$(( ))` reads the strings as integers.
- `declare -i` gives a variable the integer attribute. Each assignment then evaluates the value as an arithmetic expression.
- The shell gives no error for text that is not a number. In arithmetic, the word `abc` is the name of a variable that is not set, so its value is 0.
