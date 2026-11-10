```python run id=schema
import io
import unittest

MUTANT = 0  # 0 runs the original code; 1 to 4 each switch on one change.
NAMES = {1: "> to >=", 2: "> to <", 3: "xs[i] to xs[best]", 4: "start at 0"}


def argmax(xs):
    """Returns the index of the first largest item of a non-empty list."""
    best = 0
    for i in range(0 if MUTANT == 4 else 1, len(xs)):
        left = xs[best] if MUTANT == 3 else xs[i]
        match MUTANT:
            case 1:
                bigger = left >= xs[best]
            case 2:
                bigger = left < xs[best]
            case _:
                bigger = left > xs[best]
        if bigger:
            best = i
    return best


class Weak(unittest.TestCase):
    def test_largest_last(self):
        self.assertEqual(argmax([1, 2, 3]), 2)

    def test_largest_in_middle(self):
        self.assertEqual(argmax([1, 3, 2]), 1)

    def test_largest_first(self):
        self.assertEqual(argmax([3, 1, 2]), 0)


class Strong(Weak):
    def test_tie_goes_to_first(self):
        self.assertEqual(argmax([2, 2, 1]), 0)


def passes(case):
    tests = unittest.defaultTestLoader.loadTestsFromTestCase(case)
    result = unittest.TextTestRunner(stream=io.StringIO()).run(tests)
    return result.wasSuccessful()


assert passes(Strong), "the original program must pass first"
print(f"{'mutant':<20}{'3 tests':<8}4 tests")
killed = {Weak: 0, Strong: 0}
for MUTANT, name in NAMES.items():
    cells = []
    for case in (Weak, Strong):
        dead = not passes(case)
        killed[case] += dead
        cells.append("killed" if dead else "lived")
    print(f"{name:<20}{cells[0]:<8}{cells[1]}")
print(f"{'killed':<20}{killed[Weak]} of 4  {killed[Strong]} of 4")
```

```output
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. Python has no mutation tool in the standard library. mutmut is a mutation tool for Python, and it runs the tests with pytest.
- The program is a mutant schema: one copy of `argmax` holds every mutant, and the global `MUTANT` picks one. The `for MUTANT, name in ...` loop at module level assigns the global, so `argmax` sees each value in turn.
- `Strong` inherits the three tests of `Weak` and adds one. A `unittest` runner with a `StringIO` stream runs a suite quietly, and `wasSuccessful()` gives the verdict for one mutant.
- The tie `[2, 2, 1]` kills `> to >=`. No test can kill `start at 0`, because the extra pass compares `xs[0]` with itself. mutmut can skip such a line with the comment `# pragma: no mutate`.
