```python run id=null-logger
class ConsoleLogger:
    def log(self, message):
        print(f"log: {message}")


class NullLogger:
    def log(self, message):
        pass  # do nothing


class Checkout:
    def __init__(self, logger):
        self.logger = logger

    def pay(self, amount):
        self.logger.log(f"paid {amount}")  # no check for None
        return f"receipt: {amount}"


print(Checkout(ConsoleLogger()).pay(10))
print(Checkout(NullLogger()).pay(20))
```

```output
log: paid 10
receipt: 10
receipt: 20
```

- Verdict: idiom. Python has duck typing, so `NullLogger` needs only a `log` method that does nothing. The standard library has null objects of this kind, for example `contextlib.nullcontext` and `logging.NullHandler`.
