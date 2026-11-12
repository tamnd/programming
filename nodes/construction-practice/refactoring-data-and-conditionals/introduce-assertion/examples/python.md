```python run id=say-the-assumption
import subprocess
import sys


def refund_cents(paid_cents: int, days_paid: int, days_used: int) -> int:
    assert 0 <= days_used <= days_paid, "days used must be between 0 and days paid"
    return round(paid_cents * (days_paid - days_used) / days_paid)


assert refund_cents(3000, 30, 10) == 2000
try:
    refund_cents(3000, 30, 31)
    raise RuntimeError("expected AssertionError")
except AssertionError as e:
    assert "between 0" in str(e)

# With -O, the assert statement is gone, and the bug returns a negative refund.
code = "assert False, 'gone'\nprint('no assert')"
out = subprocess.run([sys.executable, "-O", "-c", code], capture_output=True, text=True)
assert out.stdout.strip() == "no assert"
print("introduce assertion: tests pass")
```

```output
introduce assertion: tests pass
```
