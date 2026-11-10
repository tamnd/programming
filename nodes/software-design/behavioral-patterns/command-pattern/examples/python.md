```python run id=editor
from collections.abc import Callable

# A command runs once and returns the command that reverses it.
Command = Callable[[], "Command"]


class Buffer:
    def __init__(self) -> None:
        self.text = ""


def append(buffer: Buffer, text: str) -> Command:
    def run() -> Command:
        buffer.text += text
        return delete(buffer, len(text))

    return run


def delete(buffer: Buffer, count: int) -> Command:
    def run() -> Command:
        removed = buffer.text[len(buffer.text) - count :]  # read when the command runs
        buffer.text = buffer.text[: len(buffer.text) - count]
        return append(buffer, removed)

    return run


class History:
    def __init__(self) -> None:
        self.undo_stack: list[Command] = []
        self.redo_stack: list[Command] = []

    def run(self, command: Command) -> None:
        self.undo_stack.append(command())
        self.redo_stack.clear()  # a fresh command ends the redo branch

    def undo(self) -> bool:
        if not self.undo_stack:
            return False
        self.redo_stack.append(self.undo_stack.pop()())
        return True

    def redo(self) -> bool:
        if not self.redo_stack:
            return False
        self.undo_stack.append(self.redo_stack.pop()())
        return True


buffer, history = Buffer(), History()
show = lambda label: print(f'{label}: "{buffer.text}"')
history.run(append(buffer, "hello")); show("append")
history.run(append(buffer, " world")); show("append")
history.run(delete(buffer, 5)); show("delete")
history.undo(); show("undo")
history.undo(); show("undo")
history.redo(); show("redo")
history.run(append(buffer, "!")); show("append")
if not history.redo():
    print("redo: nothing to redo")
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. A command is a closure. It holds the buffer and its argument, and it runs when it is called.
- Each command returns its inverse when it runs. The inverse of `append` is a `delete` of the same length, and the inverse of `delete` is an `append` of the removed text. Undo calls the inverse and gets back a command that redoes the change. This is the shape of the self-referential options of Rob Pike (see the node).
- `delete` reads the text to remove when it runs, not when it is created. The node shows the bug that the other order causes.
- A class with `do` and `undo` methods is the classic form. IDLE, the editor that comes with CPython, uses that form in `idlelib/undo.py`.
