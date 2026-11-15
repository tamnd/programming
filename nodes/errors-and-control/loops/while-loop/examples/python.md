```python run id=walrus-read-loop-and-tolerance
import io

# := reads a line and tests it in one condition; "" ends the loop.
f = io.StringIO("alpha\nbeta\ngamma\n")
while line := f.readline():
    print("read", line.strip())

# Newton's method: loop until the estimate is close enough.
x = 2.0
guess = 1.0
steps = 0
while abs(guess * guess - x) > 1e-12:
    guess = (guess + x / guess) / 2
    steps += 1
print(round(guess, 10), steps)
```

```output for=walrus-read-loop-and-tolerance
read alpha
read beta
read gamma
1.4142135624 5
```

- Verdict: native. `while line := f.readline()` reads and tests in one condition, and the empty string at the end of the file is false. Newton's method for the square root of 2 stops when the error is under a tolerance, after 5 passes that the code does not state.
