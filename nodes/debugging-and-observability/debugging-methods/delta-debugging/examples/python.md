```python run id=ddmin
def ddmin(chars, fails):
    """Returns a 1-minimal part of chars on which fails() is still true."""
    n = 2
    while len(chars) >= 2:
        size = len(chars)
        bounds = [(i * size // n, (i + 1) * size // n) for i in range(n)]
        subsets = [chars[lo:hi] for lo, hi in bounds]
        complements = [chars[:lo] + chars[hi:] for lo, hi in bounds]
        smaller = next((s for s in subsets if fails(s)), None)
        if smaller is not None:  # reduce to subset
            chars, n = smaller, 2
            print("subset    ", len(chars))
            continue
        smaller = next((c for c in complements if fails(c)), None)
        if smaller is not None:  # reduce to complement
            chars, n = smaller, max(n - 1, 2)
            print("complement", len(chars))
            continue
        if n == size:  # each single character was removed
            break
        n = min(size, 2 * n)  # increase granularity
    return chars


tests = 0


def browser_crashes(text):
    """Stands for one run of the browser: it crashes on a <SELECT ...> tag."""
    global tests
    tests += 1
    start = text.find("<SELECT")
    return start >= 0 and ">" in text[start:]


page = '<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>'
minimal = ddmin(page, browser_crashes)
print(f"{len(page)} characters reduced to {minimal} in {tests} tests")
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

The test function decides what counts as "the failure". The program below has two bugs. The test treats every exception as the failure, so `ddmin` drifts from the crash in the bug report to a different bug, an unclosed tag.

```python wrong id=any-exception
def ddmin(chars, fails):
    n = 2
    while len(chars) >= 2:
        size = len(chars)
        bounds = [(i * size // n, (i + 1) * size // n) for i in range(n)]
        subsets = [chars[lo:hi] for lo, hi in bounds]
        complements = [chars[:lo] + chars[hi:] for lo, hi in bounds]
        smaller = next((s for s in subsets if fails(s)), None)
        if smaller is not None:
            chars, n = smaller, 2
            continue
        smaller = next((c for c in complements if fails(c)), None)
        if smaller is not None:
            chars, n = smaller, max(n - 1, 2)
            continue
        if n == size:
            break
        n = min(size, 2 * n)
    return chars


def render(text):
    """The program under test. It has two different bugs."""
    if text.rfind("<") > text.rfind(">"):
        raise ValueError("unclosed tag")
    start = text.find("<SELECT")
    if start >= 0 and ">" in text[start:]:
        raise RuntimeError("crash in the SELECT layout")


def fails(text):
    try:
        render(text)
    except Exception:  # any error counts as the failure
        return True
    return False


page = '<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>'
print(repr(ddmin(page, fails)))
```

```output for=any-exception
'<'
```

The fixed test returns `True` only for the same failure. Any other error is an unresolved outcome, so it returns `False`. Zeller and Hildebrandt compare the backtrace for the same reason.

```python run id=same-failure
def ddmin(chars, fails):
    n = 2
    while len(chars) >= 2:
        size = len(chars)
        bounds = [(i * size // n, (i + 1) * size // n) for i in range(n)]
        subsets = [chars[lo:hi] for lo, hi in bounds]
        complements = [chars[:lo] + chars[hi:] for lo, hi in bounds]
        smaller = next((s for s in subsets if fails(s)), None)
        if smaller is not None:
            chars, n = smaller, 2
            continue
        smaller = next((c for c in complements if fails(c)), None)
        if smaller is not None:
            chars, n = smaller, max(n - 1, 2)
            continue
        if n == size:
            break
        n = min(size, 2 * n)
    return chars


def render(text):
    """The program under test. It has two different bugs."""
    if text.rfind("<") > text.rfind(">"):
        raise ValueError("unclosed tag")
    start = text.find("<SELECT")
    if start >= 0 and ">" in text[start:]:
        raise RuntimeError("crash in the SELECT layout")


def fails(text):
    try:
        render(text)
    except RuntimeError as error:  # the failure of the bug report
        return str(error) == "crash in the SELECT layout"
    except Exception:  # another bug: the outcome is unresolved
        return False
    return False


page = '<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>'
print(repr(ddmin(page, fails)))
```

```output for=same-failure
'<SELECT>'
```

- Verdict: idiom. The standard library has no delta debugging, and the algorithm is about 20 lines.
- Slices build each subset and each complement in one expression.
- `next()` over a generator stops at the first candidate that fails, so the program runs no extra tests.
- The test must name one failure. A broad `except Exception` lets the search move to a different bug.
