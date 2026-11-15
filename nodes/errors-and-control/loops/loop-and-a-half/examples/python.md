```python run id=iter-sentinel-and-input-validation
import io

f = io.StringIO("10\n20\nend\n30\n")
# iter(callable, sentinel) calls readline until it returns the sentinel.
for line in iter(f.readline, "end\n"):
    print("got", line.strip())

# Ask, test, then complain and ask again.
answers = iter(["abc", "-4", "7"])
while True:
    text = next(answers)
    if text.isdigit():
        break
    print("not a positive number:", text)
print("accepted", int(text))
```

```output for=iter-sentinel-and-input-validation
got 10
got 20
not a positive number: abc
not a positive number: -4
accepted 7
```

- Verdict: idiom. `iter(f.readline, "end\n")` turns the read and the test into an iterator, so a plain `for` loop does the loop and a half with no `break`. The prompt loop reads 3 times and complains 2 times, with the `break` in the middle of the body.
