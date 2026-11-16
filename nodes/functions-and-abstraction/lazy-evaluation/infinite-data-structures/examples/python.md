```python run id=infinite
from itertools import accumulate, count, cycle, islice, repeat, takewhile
import operator


def primes():
    found = []
    for n in count(2):
        if all(n % p for p in takewhile(lambda p: p * p <= n, found)):
            found.append(n)
            yield n


powers_of_two = accumulate(repeat(2), operator.mul, initial=1)

print(list(islice(primes(), 10)))
print(list(islice(powers_of_two, 8)))
print(list(islice(cycle("abc"), 7)))
```

```output for=infinite
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
[1, 2, 4, 8, 16, 32, 64, 128]
['a', 'b', 'c', 'a', 'b', 'c', 'a']
```

- Verdict: stdlib. `itertools.count`, `repeat` and `cycle` are infinite iterators, and a generator with an endless loop, such as `primes`, is another. `islice` takes a finite prefix.
- `primes` keeps the primes it has found and tests each number only against the primes up to its square root. The list grows with the number of primes read, so the generator is lazy but not constant in memory.
- `accumulate(repeat(2), operator.mul, initial=1)` is an iterate: each element is the previous one times 2.
