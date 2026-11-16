```python run id=pipeline
from itertools import count, islice


def square(n):
    print("square", n)
    return n * n


odd_squares = filter(lambda s: s % 2 == 1, map(square, count(1)))
print(list(islice(odd_squares, 2)))
```

```output for=pipeline
square 1
square 2
square 3
[1, 9]
```

- Verdict: native. `map`, `filter`, `zip`, generator expressions and the functions of `itertools` return iterators that do no work until a consumer pulls. `count(1)` is infinite, and `islice` stops the pipeline after two odd squares.
- An iterator is single-pass. `itertools.tee` gives several independent iterators over one source, and stores the elements that one reader has seen and another has not.

```python wrong id=second-pass
squares = (n * n for n in range(4))
print(sum(squares))
print(sum(squares))
```

```output for=second-pass
14
0
```

The second `sum` finds the generator used up and returns 0 with no error. Build a list if the values are needed twice.
