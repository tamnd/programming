```python run id=strict
def trace(label, value):
    print("evaluating", label)
    return value


def pick(flag, left, right):
    print("inside pick")
    return left if flag else right


print(pick(trace("flag", True), trace("left", 1), trace("right", 2)))
print(trace("x", False) and trace("y", True))
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating x
False
```

- Verdict: native. Python evaluates every argument, from left to right, before the body of the function runs. `pick` uses only `left`, but `right` is computed too.
- `and`, `or` and the conditional expression `a if c else b` are the non-strict forms: `trace("y", True)` never runs, because `False and ...` is false.
- Default parameter values are also computed eagerly, one time, when the `def` statement runs, and not at each call.

```python run id=defer
import logging

logging.basicConfig(level=logging.INFO, format="%(message)s")
log = logging.getLogger("demo")


def build_report():
    print("building report")
    return "3 errors"


log.debug("report: %s", build_report())
if log.isEnabledFor(logging.DEBUG):
    log.debug("report: %s", build_report())
print("done")
```

```output for=defer
building report
done
```

The first call builds the report even though the debug level is off, because the argument runs before `debug` checks the level. The guarded call skips the work.
