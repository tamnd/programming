```python run id=range-steps-and-float-counters
# Adding 0.1 ten times does not give exactly 1.0.
x = 0.0
for _ in range(10):
    x += 0.1
print(x, x == 1.0)

# Compute each value from an integer counter instead.
values = [i / 10 for i in range(1, 11)]
print(values[-1], values[-1] == 1.0)

# range does not build a list, and it knows its length.
r = range(0, 1_000_000_000, 7)
print(len(r), r[3], 21 in r)

# A negative step counts down.
for i in range(10, 0, -3):
    print("countdown", i)
```

```output for=range-steps-and-float-counters
0.9999999999999999 False
1.0 True
142857143 21 True
countdown 10
countdown 7
countdown 4
countdown 1
```

- Verdict: idiom. Python has no three-part loop, and a counted loop is a `for` over a `range`. A float counter that adds 0.1 ten times ends at 0.9999999999999999, so the loop computes each value from an integer counter. A `range` of 142857143 numbers uses little memory, because it computes its values. A negative step counts down from 10 by 3.
