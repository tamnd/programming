```python run id=undo
from dataclasses import dataclass


class Editor:
    """The originator. Only it reads and writes the contents of a memento."""

    @dataclass(frozen=True)
    class _Memento:
        text: str
        cursor: int

    def __init__(self):
        self._text = ""
        self._cursor = 0

    def type(self, chars: str) -> None:
        self._text = self._text[: self._cursor] + chars + self._text[self._cursor :]
        self._cursor += len(chars)

    def move_to(self, cursor: int) -> None:
        self._cursor = max(0, min(cursor, len(self._text)))

    def save(self) -> object:
        return Editor._Memento(self._text, self._cursor)

    def restore(self, memento: object) -> None:
        if not isinstance(memento, Editor._Memento):
            raise TypeError("not a memento of an Editor")
        self._text, self._cursor = memento.text, memento.cursor

    def show(self) -> str:
        return self._text[: self._cursor] + "|" + self._text[self._cursor :]


class History:
    """The caretaker. It keeps mementos and never looks inside them."""

    def __init__(self, editor: Editor):
        self._editor = editor
        self._undo: list[object] = []

    def save(self) -> None:
        self._undo.append(self._editor.save())

    def undo(self) -> bool:
        if not self._undo:
            return False
        self._editor.restore(self._undo.pop())
        return True


editor = Editor()
history = History(editor)

history.save()
editor.type("Hello")
print(editor.show())
history.save()
editor.type(" world")
print(editor.show())
history.save()
editor.move_to(5)
editor.type(",")
print(editor.show())

for _ in range(4):
    if history.undo():
        print("undo:", editor.show())
    else:
        print("nothing to undo")
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. `save` returns an object and `restore` takes one back. The caretaker types the memento as `object`, so a type checker reports any attempt to read its fields.
- Python has no access control. The leading underscore of `_Memento` and the `object` type are conventions that tell the caretaker not to look inside.
- The memento is a frozen dataclass, and both of its fields are immutable values. The memento needs no copy of the text, because a `str` cannot change.
- `copy.deepcopy` of the originator's state is the general form when the state holds lists or dicts. The node page shows the bug when the memento keeps a reference to a list that the originator still changes.
