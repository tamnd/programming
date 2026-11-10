```python run id=todo
from collections.abc import Callable
from dataclasses import dataclass


@dataclass
class Item:
    title: str
    done: bool = False


class TodoModel:
    """Knows the items and the rules. Knows nothing about screens or input."""

    def __init__(self) -> None:
        self._items: list[Item] = []
        self._observers: list[Callable[[], None]] = []

    def subscribe(self, observer: Callable[[], None]) -> None:
        self._observers.append(observer)

    def items(self) -> list[Item]:
        return list(self._items)

    def add(self, title: str) -> None:
        if not title:
            raise ValueError("a title is required")
        self._items.append(Item(title))
        self._changed()

    def complete(self, number: int) -> None:
        if not 1 <= number <= len(self._items):
            raise ValueError(f"no item {number}")
        self._items[number - 1].done = True
        self._changed()

    def _changed(self) -> None:
        for observer in self._observers:
            observer()


class ListView:
    """Shows every item. Asks the model for data when the model changes."""

    def __init__(self, model: TodoModel) -> None:
        self._model = model
        model.subscribe(self.render)

    def render(self) -> None:
        for n, item in enumerate(self._model.items(), 1):
            print(f"  [{'x' if item.done else ' '}] {n} {item.title}")


class SummaryView:
    """A second view of the same model."""

    def __init__(self, model: TodoModel) -> None:
        self._model = model
        model.subscribe(self.render)

    def render(self) -> None:
        items = self._model.items()
        done = sum(item.done for item in items)
        print(f"  summary: {len(items) - done} open, {done} done")


class Controller:
    """Turns user input into calls on the model. Shows input errors."""

    def __init__(self, model: TodoModel) -> None:
        self._model = model

    def handle(self, line: str) -> None:
        print(f"> {line}")
        command, _, argument = line.partition(" ")
        try:
            if command == "add":
                self._model.add(argument)
            elif command == "done" and argument.isdigit():
                self._model.complete(int(argument))
            else:
                print(f"  error: cannot read {line!r}")
        except ValueError as err:
            print(f"  error: {err}")


model = TodoModel()
ListView(model)
SummaryView(model)
controller = Controller(model)
for line in ["add buy milk", "add write report", "done 1", "done 7", "rename 2"]:
    controller.handle(line)
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. Python has no construct for the pattern. Three classes and a list of callbacks give the classic form.
- The model calls its observers with no arguments. Each view then asks the model for the data it shows, so the model does not know what a view needs.
- The controller never prints the list. It changes the model, and the change reaches both views through the observers. Errors in the input are the controller's to report.
- The input is a fixed list of lines, so the program runs without a person at a terminal. A loop over `sys.stdin` would make it interactive and change nothing else.
