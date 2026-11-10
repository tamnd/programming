---
tag: NEW-command-pattern
kind: pattern
slug: command-pattern
name: Command pattern
aliases: [Command, Action, Transaction, command object]
area: software-design/behavioral-patterns
level: core
summary: >-
  A request becomes an object that holds the operation and its arguments. The object can be stored, queued, logged, and run later, and with a second operation it can be undone.
family: behavioral-design
polarity: positive
intent: "Encapsulate a request as an object, so that code can pass requests as values, queue them or log them, and undo them."
also_known_as: [Action, Transaction]
refactorings: [replace-function-with-command, replace-command-with-function, replace-conditional-dispatcher-with-command]
history:
  year: 1994
  people: [Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides]
  note: Gamma, Helm, Johnson and Vlissides named and described the pattern in Design Patterns (1994), with Action and Transaction as other names, and menus of a user interface toolkit and undoable editing as the motivating examples.
see_also: [closures, strategy-pattern, composite-pattern, observer-pattern, extract-function]
sources:
  - book: isbn:9780201633610
    title: Gamma, Helm, Johnson and Vlissides, Design Patterns, Elements of Reusable Object-Oriented Software, Addison-Wesley, 1994
    locator: Chapter 5, Command (Intent, Also Known As, Motivation, Participants, Collaborations, Consequences, Implementation, Related Patterns)
  - url: https://norvig.com/design-patterns/design-patterns.pdf
    title: Peter Norvig, Design Patterns in Dynamic Programming, Object World, May 1996
    locator: slide 10 (first-class functions make Command simpler) and slide 14 (closures)
  - url: https://refactoring.com/catalog/replaceFunctionWithCommand.html
    title: Martin Fowler, Refactoring catalog, Replace Function with Command
    locator: catalog entry (also known as Replace Method with Method Object; inverse of Replace Command with Function)
  - url: https://refactoring.com/catalog/replaceCommandWithFunction.html
    title: Martin Fowler, Refactoring catalog, Replace Command with Function
    locator: catalog entry
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L116-L160
    title: CPython v3.14.0 source, Lib/idlelib/undo.py
    locator: UndoDelegator.addcmd, undo_event and redo_event, lines 116 to 160
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L162-L192
    title: CPython v3.14.0 source, Lib/idlelib/undo.py
    locator: class Command, lines 162 to 192
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L300-L330
    title: CPython v3.14.0 source, Lib/idlelib/undo.py
    locator: class CommandSequence, lines 300 to 330
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/javax/swing/undo/UndoManager.java#L571-L592
    title: OpenJDK jdk-25+36 source, javax/swing/undo/UndoManager.java
    locator: addEdit, lines 571 to 592
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.desktop/javax/swing/undo/UndoManager.html
    title: Java SE 25 API, javax.swing.undo.UndoManager
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.windows.input.icommand
    title: .NET API reference, System.Windows.Input.ICommand
    locator: interface description, methods and events
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Runnable.html
    title: Java SE 25 API, java.lang.Runnable
    locator: interface description
  - url: https://commandcenter.blogspot.com/2014/01/self-referential-functions-and-design.html
    title: Rob Pike, Self-referential functions and the design of options, command center, January 24, 2014
    locator: the option type that returns an option to restore the previous value
  - url: https://docs.python.org/3.14/library/functools.html#functools.partial
    title: The Python Standard Library, functools, Python 3.14
    locator: functools.partial
  - book: isbn:9780321213358
    title: Joshua Kerievsky, Refactoring to Patterns, Addison-Wesley, 2004
    locator: Chapter 7, Simplification, Replace Conditional Dispatcher with Command
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Function with Command and Replace Command with Function
---

## Intent

Encapsulate a request as an object. Code can then pass requests as values, put them in a queue or a log, and support operations that can be undone.[^1] The object holds what to do and the data that it needs. The code that runs it does not need to know what it does.

## Problem

A user interface toolkit has menus and buttons. The toolkit's author writes the menu item, but not the operation that the item runs. A text editor that uses the toolkit wants one item to save the file and another to paste text. The menu item must run a request that it knows nothing about. Design Patterns starts from this problem.[^1]

An editor also wants undo. To undo a change, the program must know what the change was and how to reverse it. A change that is only a method call is gone after the call returns. Nothing remains to undo.

So the forces are these. The code that triggers a request must not depend on the code that does the work. A request must exist after it runs, so that it can be undone, done again, logged, or sent to another place. And the requests must have one interface, so that one history can hold all of them.

## Solution

Design Patterns names five participants.[^1]

- **Command** declares an operation to execute, and often one to undo.
- **ConcreteCommand** binds a receiver to an action. It holds the receiver and the arguments, and calls the receiver in `execute`.
- **Client** makes a concrete command and sets its receiver.
- **Invoker** asks the command to run. A menu item, a button, a queue and a history are invokers.
- **Receiver** knows how to do the work. In the examples it is the text buffer.

The invoker calls `execute` and nothing else. It does not know the receiver or the action. A history is an invoker that keeps each command after it runs, so that it can call `undo` later.

### Commands and closures

A command with only `execute` is a function value with its arguments bound. Norvig lists Command among the four patterns that first-class functions make less complex, and he describes a closure as behaviour with state data attached.[^2] In Python, a menu can hold functions. `functools.partial` binds the arguments of a function that takes them:[^13]

```python run id=functions
from functools import partial


def save(path):
    print(f"saved {path}")


def quit_editor():
    print("bye")


menu = {"Save": partial(save, "notes.txt"), "Quit": quit_editor}
queue = [menu["Save"], menu["Quit"]]  # the invoker knows only "call it"
for command in queue:
    command()
```

```output for=functions
saved notes.txt
bye
```

Undo needs a second operation, so a single function is not enough. There are three ways to give it. A pair of closures shares the state that undo needs. An object has two methods and fields for that state. And a command can return its inverse when it runs, so that undo is a call to the returned command. Pike used the last form for options in Go. Each option sets a value and returns an option that restores the previous value.[^12] The Python example file uses it for editing commands.

| Need | A function value | A command object |
|---|---|---|
| run later, queue, pass as an argument | yes | yes |
| undo | needs a second function, or a function that returns its inverse | an `undo` method |
| inspect, print, compare, serialize | no; a closure is opaque | yes, when the command is data |
| a command type that the compiler checks | a function type | an interface or an enum |

The Rust example makes the commands an enum. A command is data that the program can print and store, and the editor passes the buffer to `execute` and `undo`. A closure in Rust that captured `&mut String` would hold the borrow for as long as the history keeps it.

### Undo and redo stacks

The examples keep two stacks. The rules are short:

- **Run** a command: execute it, push it on the undo stack, and clear the redo stack.
- **Undo**: pop from the undo stack, call `undo`, and push the command on the redo stack.
- **Redo**: pop from the redo stack, call `execute` again, and push it on the undo stack.

A single list with an index does the same work. The commands before the index can be undone, and the commands after it can be redone. IDLE, the editor that comes with CPython, keeps an `undolist` and a `pointer`. When a command is added, `self.undolist[self.pointer:] = [cmd]` drops the commands after the pointer.[^5] The Swing `UndoManager` trims the edits from `indexOfNextAdd` to the end in `addEdit`.[^8] IDLE also limits the length of the list, and merges a typed character into the previous insert command, so one undo removes a word and not one letter.[^5]

## Example

The example files run one script on a text buffer. They append `"hello"` and `" world"`, delete five characters, undo twice, and redo once. Then they append `"!"`, which clears the redo stack, so the last redo has nothing to do:

| Language | Form |
|---|---|
| Python | closures; each command returns its inverse when it runs |
| JavaScript | object literals with `execute` and `undo` closures that share a variable |
| TypeScript | an interface and a class for each command |
| Java | an interface, a record for `Append` and a class for `Delete` |
| C# | an interface and classes with primary constructors |
| C++ | a struct of two `std::function` values |
| C | a struct with a pointer to a table of functions |
| Go | an interface and a struct with pointer receivers for each command |
| Rust | an enum of commands as data, run by the editor |

## Consequences

Design Patterns lists these consequences.[^1] Command decouples the object that invokes the operation from the one that knows how to do it. Commands are objects, so code can pass them around and extend them like other objects. Commands can be put together into a composite command. And a kind of command can be added with no change to the existing classes.

The costs are classes and state. In a language without closures, each action needs its own class. A command that can be undone must save the state that undo needs, and it must save it at the right moment.

**Save the state when the command runs, not when it is created.** A delete command must save the text it removes. If it reads the text when the command object is made, and other commands run between that time and `execute`, it saves the wrong text. This macro command makes both commands first and runs them later:

```python wrong id=early-state
class Buffer:
    text = "hello"


def append(buffer, text):
    def do():
        buffer.text += text

    def undo():
        buffer.text = buffer.text[: len(buffer.text) - len(text)]

    return do, undo


def delete(buffer, count):
    removed = buffer.text[len(buffer.text) - count :]  # read too early: the append has not run yet

    def do():
        buffer.text = buffer.text[: len(buffer.text) - count]

    def undo():
        buffer.text += removed

    return do, undo


buffer = Buffer()
macro = [append(buffer, " world"), delete(buffer, 5)]
for do, _ in macro:
    do()
print(f'after the macro: "{buffer.text}"')
_, undo_delete = macro[-1]
undo_delete()
print(f'after undo of the delete: "{buffer.text}"')
```

```output for=early-state
after the macro: "hello "
after undo of the delete: "hello hello"
```

The undo of the delete should give back `"hello world"`. The delete saved `"hello"` when it was created, not `"world"`, so its undo adds the wrong text. The fix moves the read into `do`, as the example files do. The order of undo matters too. A macro undoes its commands in reverse order, because each command assumes the state that the commands before it left. IDLE's `CommandSequence` copies its list, reverses it, and undoes each command.[^7]

**Clear the redo stack when a command runs.** The redo stack holds the commands that undo reversed. They assume the state that existed before the undo. After another command runs, that state is gone. A history that keeps the redo stack applies an old command to a state it was not made for:

```python wrong id=stale-redo
class History:
    def __init__(self, buffer):
        self.buffer = buffer
        self.undo_stack, self.redo_stack = [], []

    def append(self, text):
        self.buffer.append(text)
        self.undo_stack.append(text)  # the redo stack is not cleared

    def undo(self):
        text = self.undo_stack.pop()
        self.buffer.pop()
        self.redo_stack.append(text)

    def redo(self):
        self.append(self.redo_stack.pop())


words = []
history = History(words)
history.append("hello")
history.append("world")
history.undo()
history.append("there")
history.redo()  # this should do nothing
print(" ".join(words))
```

```output for=stale-redo
hello there world
```

The user undid `"world"` and typed `"there"`. The redo then brought back `"world"` on top of the edit. Both IDLE and Swing drop the commands after the current position when a command is added.[^5][^8]

**Undo can drift.** When `undo` computes the old state from the new one, small differences can build up over many cycles of undo and redo. Design Patterns calls this error accumulation and suggests that a command save enough state to restore the receiver exactly. A memento of the receiver's state does this.[^1]

**Commands with references live long.** A history keeps commands for the life of the document. Each command keeps its receiver alive, and in C++ a command that captures a reference must not outlive its receiver. Limit the length of the history, as IDLE does.[^5]

## When not to use it

Do not write a command class for a callback that is never undone, stored or logged. Pass a function. Fowler's refactoring Replace Command with Function goes in this direction, and it is the inverse of Replace Function with Command.[^4][^3]

Do not use commands for undo when the state is small. A program can save a copy of the whole state before each change and restore it on undo. This needs no inverse operations, so it cannot have the bugs above. Commands pay off when the state is large and each change is small.

Do not turn each method call into a command object only for uniformity. The indirection hides what the code does, and a reader must find the class of each command to follow the program.

## Variants and related patterns

**Composite: macro commands.** A macro command holds a list of commands and runs them in order. Its undo runs their undo operations in reverse order. Design Patterns calls it MacroCommand.[^1] IDLE's `CommandSequence` is one, and it lets a set of edits be undone as a unit.[^7] See [[composite-pattern]].

**Memento.** A command can save a memento of the receiver's state before it runs, and restore it on undo. This trades memory for exact undo.[^1]

**Prototype.** A command that the program will change after it runs must be copied before it goes into the history. Design Patterns lists Prototype for this use.[^1]

**Strategy.** Both patterns put behaviour in an object. A strategy answers how one known task is done, and the context calls it as part of its own work. A command is a whole request, and the invoker does not know what it does. Strategy has no undo. The [[strategy-pattern]] page covers strategies. Its section on closures made in a loop applies to commands too: a closure reads a variable when it runs, not when it is made.

**Observer.** An observer is often a command. The subject keeps a list of callbacks and runs each of them when it changes. The [[observer-pattern]] page covers that pattern.

**Refactorings.** Fowler's Replace Function with Command turns a long function into an object whose fields hold its local variables, so that its parts can be split into methods.[^3][^15] Kerievsky's Replace Conditional Dispatcher with Command turns a chain of `if` statements on an action name into a map from names to command objects.[^14] See also [[extract-function]].

## Known uses

- CPython: [IDLE's UndoDelegator at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L116-L160) keeps an `undolist` and a `pointer`. `addcmd` runs a command, merges it with the previous one when it can, drops the commands after the pointer, and limits the list to `max_undo`. [Its Command class](https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L162-L192) has `do`, `redo`, `undo` and `merge`, and [CommandSequence](https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L300-L330) undoes a group in reverse order.[^5][^6][^7]
- OpenJDK Swing: `javax.swing.undo.UndoManager` holds `UndoableEdit` objects.[^9] [Its addEdit at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.desktop/share/classes/javax/swing/undo/UndoManager.java#L571-L592) trims the edits after the current index, adds the edit, and then enforces its limit.[^8]
- .NET: `System.Windows.Input.ICommand` has `Execute`, `CanExecute` and a `CanExecuteChanged` event. WPF implements it in `RoutedCommand` and `RoutedUICommand`, and it gives the commanding behaviour of XAML buttons.[^10]
- Java: `java.lang.Runnable` is a command with one method and no undo. A class whose instances run on a thread implements it.[^11]

## References
