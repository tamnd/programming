```python run id=loop-else-finally-yield
# The else of a loop runs only when the loop ends without break.
def find(items, target):
    for i, item in enumerate(items):
        if item == target:
            print("found at", i)
            break
    else:
        print("not found")


find(["a", "b", "c"], "b")
find(["a", "b", "c"], "z")


# finally runs even when the try block returns.
def order():
    try:
        print("try")
        return "from try"
    finally:
        print("finally")


print(order())

# A generator gives control back to the caller at each yield, and resumes later.
def counter():
    print("start")
    yield 1
    print("resume")
    yield 2
    print("end")


for n in counter():
    print("got", n)

# A conditional expression chooses a value, not a statement.
n = 7
print("odd" if n % 2 else "even")
```

```output for=loop-else-finally-yield
found at 1
not found
try
finally
from try
start
got 1
resume
got 2
end
odd
```

- Verdict: native. The `else` of a `for` loop runs only when the loop ends without `break`. `finally` runs after `return` computes its value and before the function returns. A generator stops at each `yield`, gives control to the `for` loop, and resumes at the next request. A conditional expression chooses a value.
