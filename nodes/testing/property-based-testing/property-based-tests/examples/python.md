```python test id=pages
import functools
import unittest

SEED = 2463534242  # the seed in Marsaglia's xorshift paper


class Rng:
    def __init__(self, seed):
        self.state = seed

    def next(self):
        y = self.state
        y ^= (y << 13) & 0xFFFFFFFF
        y ^= y >> 17
        y ^= (y << 5) & 0xFFFFFFFF
        self.state = y
        return y

    def between(self, lo, hi):
        return lo + self.next() % (hi - lo + 1)


def shrink_int(x, lo):
    candidates = [lo] if x > lo else []
    d = (x - lo) // 2
    while d > 0:
        candidates.append(x - d)
        d //= 2
    return candidates


def for_all(generate, shrink, trials=100, seed=SEED):
    """Runs the test body on generated cases; reports the shrunk failure."""

    def decorate(body):
        def fails(self, case):
            try:
                body(self, *case)
            except AssertionError:
                return True
            return False

        @functools.wraps(body)
        def run(self):
            rng = Rng(seed)
            for _ in range(trials):
                case = generate(rng)
                if fails(self, case):
                    while (smaller := next((c for c in shrink(case) if fails(self, c)), None)) is not None:
                        case = smaller
                    self.fail(f"falsified by {case} (seed {seed})")

        return run

    return decorate


def page_cases(rng):
    return rng.between(0, 1000), rng.between(1, 50)


def shrink_pages(case):
    items, per_page = case
    return [(n, per_page) for n in shrink_int(items, 0)] + [(items, p) for p in shrink_int(per_page, 1)]


def buggy_pages(items, per_page):
    return items // per_page


def pages(items, per_page):
    return -(-items // per_page)  # floor division of the negation rounds up


class PagesTest(unittest.TestCase):
    @for_all(page_cases, shrink_pages)
    def test_pages_hold_all_items(self, items, per_page):
        self.assertGreaterEqual(pages(items, per_page) * per_page, items)

    @for_all(page_cases, shrink_pages)
    def test_one_page_fewer_is_not_enough(self, items, per_page):
        self.assertLess((pages(items, per_page) - 1) * per_page, items)

    def test_the_property_catches_the_bug(self):
        @for_all(page_cases, shrink_pages)
        def check(self, items, per_page):
            self.assertGreaterEqual(buggy_pages(items, per_page) * per_page, items)

        with self.assertRaisesRegex(AssertionError, r"falsified by \(1, 2\)"):
            check(self)


if __name__ == "__main__":
    unittest.main(verbosity=2)
```

```output stream=stderr
test_one_page_fewer_is_not_enough (test_main.PagesTest.test_one_page_fewer_is_not_enough) ... ok
test_pages_hold_all_items (test_main.PagesTest.test_pages_hold_all_items) ... ok
test_the_property_catches_the_bug (test_main.PagesTest.test_the_property_catches_the_bug) ... ok

----------------------------------------------------------------------
Ran 3 tests in $TIME

OK
```

- Verdict: library. Hypothesis is a property testing library for Python. Its decorator `@given` takes strategies, which are generators passed as values.
- `for_all` is a decorator that turns a test method with parameters into a method with none, so `unittest` can run it. Each case runs the body, and an `AssertionError` counts as a failure.
- The shrinker calls the body again on each candidate. It keeps the first candidate that still fails, until none does, and the final message names the small case and the seed.
- `-(-items // per_page)` rounds up, because floor division of a negative number rounds toward minus infinity. Python integers do not overflow, so the sum `items + per_page - 1` is also safe here; in C and Rust it is not (see their examples).
- The third test checks the checker. It runs the property on the buggy version and expects the failure message with the case (1, 2).
