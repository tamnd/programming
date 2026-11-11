---
tag: MFKQP4
kind: pattern
slug: memento-pattern
name: Memento pattern
aliases: [Memento, Token, snapshot, saved state]
area: software-design/behavioral-patterns
level: core
summary: >-
  An object saves its internal state in a memento, an object that other code can store but cannot read. Later the object restores that state from the memento. Undo is the usual use.
family: behavioral-design
polarity: positive
intent: "Capture the internal state of an object in a separate object, so that the object can return to that state later, and do this without breaking its encapsulation."
also_known_as: [Token]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with undo in a graphical editor as the motivating example.
see_also: [command-pattern, prototype-pattern, state-pattern, observer-pattern]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Memento (Intent, Also Known As, Motivation, Applicability, Participants, Consequences, Implementation, Related Patterns)
  - url: https://docs.python.org/3.14/library/copy.html#shallow-vs-deep-copy
    title: The Python Standard Library, copy, Python 3.14
    locator: copy.deepcopy, the difference between shallow and deep copying
  - url: https://docs.python.org/3.14/library/dataclasses.html#frozen-instances
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: Frozen instances
  - url: https://docs.python.org/3.14/library/dataclasses.html#dataclasses.replace
    title: The Python Standard Library, dataclasses, Python 3.14
    locator: dataclasses.replace
  - url: https://docs.python.org/3.14/library/pickle.html#module-pickle
    title: The Python Standard Library, pickle, Python 3.14
    locator: the warning at the top of the page
  - url: https://redux.js.org/usage/implementing-undo-history
    title: Redux documentation, Implementing Undo History
    locator: Understanding Undo History; Designing the State Shape; Handling Undo
  - url: https://martinfowler.com/eaaDev/EventSourcing.html
    title: Martin Fowler, Event Sourcing
    locator: Reversing Events
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-6.html#jls-6.6.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 6.6.1 Determining Accessibility
  - url: https://doc.rust-lang.org/reference/visibility-and-privacy.html
    title: The Rust Reference, Visibility and privacy
    locator: rule vis.access
  - url: https://html.spec.whatwg.org/multipage/nav-history-apis.html#dom-history-pushstate
    title: HTML Living Standard, WHATWG
    locator: 7.2.5 The History interface, pushState
---

## Intent

Capture the internal state of an object in a separate object, so that the object can return to that state later. Do this without breaking the encapsulation of the object.[^1] The separate object is the **memento**. Other code stores it and gives it back, but only the object that made it can read it.

## Problem

Design Patterns starts with undo in a graphical editor. The user can move a shape that is connected to other shapes, and a constraint solver keeps the connections. To undo the move, the editor must put the shapes and the solver back as they were. The solver keeps internal state for this, and it does not make that state public.[^1]

The same need appears in a text editor, a form with a cancel button, a game that saves a checkpoint, a transaction that rolls back, and a search that tries a move and takes it back.

The forces are these. Undo needs the earlier state. The earlier state is private to the object, and making it public breaks the object's encapsulation: the code that saves it then depends on its fields and can change them. And the code that decides when to save and when to restore, such as an undo stack, is not the object itself.

## Solution

Design Patterns names three participants.[^1]

- **Originator** is the object whose state is saved. It makes a memento of its current state, and it restores its state from a memento.
- **Memento** stores some or all of the internal state of the originator. It protects that state from all objects except the originator.
- **Caretaker** keeps the mementos. It never reads or changes the contents of a memento.

Design Patterns describes the memento as having two interfaces. The caretaker sees a **narrow** interface: it can only pass the memento on. The originator sees a **wide** interface: it can read all the data that it needs to restore itself.[^1]

```text
  Caretaker (History)              Originator (Editor)
   undo stack: [m1, m2, m3]          text, cursor (private)
        |                                 |
        |  save() ------------------->  makes Memento(text, cursor)
        |  <------------------- m3 ----   (the caretaker cannot read m3)
        |
        |  restore(m3) -------------->  reads m3, sets text and cursor
```

Each example file builds a text editor and an undo history. The editor has text and a cursor. The history saves a memento before each edit, and `undo` restores the last one:

1. Type `Hello`. The editor shows `Hello|`, where `|` is the cursor.
2. Type ` world`. The editor shows `Hello world|`.
3. Move the cursor to 5 and type `,`. The editor shows `Hello,| world`.
4. Undo three times, back to the empty text. A fourth undo prints `nothing to undo`.

### A memento must not share mutable state

A memento must not change after it is made. When it holds a reference to a list or a dict that the originator keeps changing, it changes with the originator, and the undo restores the present state:

```python wrong id=shared-list
class Editor:
    def __init__(self):
        self.lines = []

    def add(self, line):
        self.lines.append(line)

    def save(self):
        return self.lines  # the memento is the live list, not a copy

    def restore(self, memento):
        self.lines = memento


editor = Editor()
editor.add("first")
saved = editor.save()
editor.add("second")
editor.restore(saved)
print(editor.lines)
```

```output for=shared-list
['first', 'second']
```

The undo did nothing, because `saved` and `editor.lines` are one list. The fix is a copy when the memento is made: `list(self.lines)` for a list of strings, or `copy.deepcopy` for nested data.[^2] This is the same decision about shallow and deep copies that [[prototype-pattern]] needs. A field of the state that is immutable needs no copy: Python strings, Java strings, Go strings and C# strings cannot change, so a memento can share them.

### Immutable state makes the memento free

When the whole state of the originator is an immutable value, the state is its own memento. Saving is storing a reference, and restoring is assigning one. A frozen dataclass in Python raises `FrozenInstanceError` on an attempt to set a field.[^3] `dataclasses.replace` makes a copy with some fields changed.[^4] A tuple of lines shares each line that did not change:

```python run id=immutable
from dataclasses import dataclass, replace


@dataclass(frozen=True)
class Document:
    lines: tuple[str, ...]
    cursor: int


def edit_line(doc: Document, index: int, text: str) -> Document:
    return replace(doc, lines=doc.lines[:index] + (text,) + doc.lines[index + 1 :])


history: list[Document] = []
doc = Document(lines=("title", "body", "footer"), cursor=0)

history.append(doc)  # the snapshot is a reference: no copy
doc = edit_line(doc, 1, "body, edited")
history.append(doc)
doc = replace(doc, cursor=2)

saved = history[0]
shared = sum(a is b for a, b in zip(saved.lines, doc.lines))
print("now:", doc.lines, "cursor", doc.cursor)
print("lines shared with the first snapshot:", shared, "of", len(doc.lines))
while history:
    doc = history.pop()
    print("undo:", doc.lines, "cursor", doc.cursor)
```

```output for=immutable
now: ('title', 'body, edited', 'footer') cursor 2
lines shared with the first snapshot: 2 of 3
undo: ('title', 'body, edited', 'footer') cursor 0
undo: ('title', 'body', 'footer') cursor 0
```

The two snapshots and the present state share the strings `"title"` and `"footer"`. Each edit allocates only what changed. The Redux documentation builds undo this way. The state shape holds an array of past states, the present state and an array of future states. Undo moves the present state to the future array and takes the last past state as the present.[^6] The state is never changed in place, so each entry in the arrays is a full snapshot that costs one reference.

### Memento and Command for undo

[[command-pattern]] gives the other way to undo. Each command knows how to reverse itself: an insert command deletes what it inserted, and a move command moves back. A memento saves the state before the change, and an undo restores it.

The two differ in what they store. A command stores the change, which is small. A memento stores the state, which can be large. But a reverse operation is not always possible from the change alone. Here an editor makes text upper case, and the undo command makes it lower case:

```python wrong id=inverse
class UpperCase:
    def execute(self, text):
        return text.upper()

    def undo(self, text):
        return text.lower()  # the reverse loses the original case


text = "Hello World"
command = UpperCase()
text = command.execute(text)
print("after:", text)
print("undo:", command.undo(text))
```

```output for=inverse
after: HELLO WORLD
undo: hello world
```

The upper-case operation loses information, so no reverse function exists. The command must save the earlier text before it runs, which is a memento. Design Patterns says that commands can use mementos to keep the state that undoable operations need.[^1] Most undo systems mix the two: commands for operations with a cheap exact reverse, and mementos for the rest.

Fowler describes the same choice for event sourcing. A system can reverse an event by applying its inverse. Or it can go back to an earlier snapshot of the state and replay the events after it, up to the point it wants.[^7]

### Serialization

A memento that must outlive the process, such as a saved game or a draft, is serialized to bytes. Python's `pickle` serializes most objects with no code in the class. The documentation warns that the module is not secure: unpickling data can run arbitrary code, so a program must never unpickle data from a source that it does not trust.[^5] A memento written to a file that a user can edit needs a format with no code in it, such as JSON, and a check of its contents on load. A browser's `history.pushState` is a caretaker of this kind. It adds an entry to the session history with a serialization of the state that the page gives it, and the page reads the state back when the user goes back.[^10]

## Example

The table shows how each language builds the narrow interface, so that the caretaker cannot read the memento.

| Language | Narrow interface |
|---|---|
| Python | a private nested class by convention, typed as `object` for the caretaker |
| JavaScript | an empty frozen object as a key into a private static `WeakMap` |
| TypeScript | a branded type with a `unique symbol` key |
| Java | a nested class with `private` fields and constructor |
| C# | an empty public interface and a `private` nested record |
| Go | a struct with unexported fields |
| Rust | a struct with fields private to the originator's module |
| C++ | a class with `private` members and the originator as a `friend` |

In Java, a `private` member is accessible from anywhere in the body of the top level class that encloses it.[^8] In Rust, a private item is visible in its module and the modules below it.[^9] So in Java the originator must be a top level class, and in Rust the memento must be in the originator's module, for the caretaker to be shut out.

## Consequences

Design Patterns lists these consequences.[^1]

- **It keeps encapsulation boundaries.** The memento hides the originator's state from the caretaker, so other objects do not depend on it.
- **It simplifies the originator.** Without mementos, the originator would keep every version of its state for the clients that want them. The caretaker keeps them instead.
- **Mementos can be expensive.** A memento that copies a large state, or an originator that makes mementos often, costs memory and time. Design Patterns suggests that a memento can store only the change since the last one, when mementos are made and restored in a known order.
- **Narrow and wide interfaces are hard in some languages.** A language must let the originator, and no other class, read the memento. The table above shows how far each language gets.
- **The cost of keeping mementos is hidden.** The caretaker does not know how much state a memento holds, so a caretaker that looks cheap can hold a large amount of memory.

The last point needs a limit in practice. An undo stack with no limit grows for the life of the document. Editors cap the number of entries, merge small changes such as single key strokes into one entry, or drop old entries.

A memento also captures only the originator's own state. An object that refers to other objects must decide whether to save them too. A memento of the editor that does not save the clipboard restores the text but not the clipboard. A memento of a shape that refers to a shared style must not restore the style for other shapes.

## When not to use it

Do not use it when the state is public and the class is a plain data record. A copy of the record is enough, and the narrow interface protects nothing.

Do not use it when each change has a cheap exact reverse and the state is large. A command that stores the change takes less memory than a snapshot of the whole state.

Do not use mementos to send state across a trust boundary, such as to a browser and back, unless the format is safe to parse and the originator checks it on restore. A memento from outside is input.

Do not build it by hand when the state is already immutable. The state value is the memento, and a list of references is the caretaker, as the block above shows.

## Variants and related patterns

**Command.** Design Patterns says that commands can use mementos to keep state for undoable operations.[^1] A command saves a memento before it runs, and its `undo` restores it. See [[command-pattern]].

**Prototype.** A prototype and a memento both copy the state of an object. A prototype copy is a new object that lives on its own. A memento is a saved state that its originator reads back, and no other code uses it as an object. See [[prototype-pattern]].

**State.** A state object holds the behaviour of one state of a context, and the context replaces it on each transition. A memento holds a value of the state, not its behaviour. See [[state-pattern]].

**Snapshots and event sourcing.** An event store keeps each change as an event. A snapshot is a memento of the state after some event, so that a restore does not need to replay every event from the start.[^7]

**Persistent data structures.** An immutable structure that shares the unchanged parts between versions, as the tuple of lines does above, makes each version a memento at low cost. Each version is a value, and the history is a list of values.[^6]

## Known uses

- Redux: the undo history keeps past, present and future states in the store, and undo and redo move states between them.[^6]
- Browsers: `history.pushState` adds an entry to the session history with a serialization of the page's state.[^10]
- Event sourcing: a snapshot of the state lets a system restore a past state and then replay the later events.[^7]

## References
