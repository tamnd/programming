---
tag: 4PXDGG
kind: pattern
slug: command-processor
name: Command Processor
aliases: [command processor pattern, command processor]
area: software-design/behavioral-patterns
level: advanced
summary: >-
  A command processor receives command objects, executes them and keeps them. Because all requests pass through it, it can add services such as undo, redo, logging and scheduling.
family: behavioral-design
polarity: positive
intent: "Separate the request for a service from its execution: a command processor component manages requests as separate objects, schedules their execution and provides services such as undo."
also_known_as: []
history:
  year: 1996
  people: [Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad, Michael Stal]
  note: Command Processor is one of the design patterns of Pattern-Oriented Software Architecture, which John Wiley and Sons published in 1996. A study of the CommandProcessor classes of the ET++ framework motivated the description.
see_also: [command-pattern, memento-pattern, interpreter-pattern, singleton-pattern]
sources:
  - url: https://www.cs.uni.edu/~wallingf/teaching/062/sessions/support/pattern-examples.pdf
    title: "Examples to Accompany: Design Patterns, Elements of Reusable Object-Oriented Software, AG Communication Systems, 1999"
    locator: Command Processor, page A-47
  - book: isbn:9780471958697
    title: "Frank Buschmann, Regine Meunier, Hans Rohnert, Peter Sommerlad and Michael Stal, Pattern-Oriented Software Architecture, Volume 1: A System of Patterns, John Wiley and Sons"
    locator: Command Processor, pages 277 to 290 (Example, Context, Problem, Solution, Structure, Variants, Known Uses, Consequences, See Also, Credits)
  - url: https://github.com/qt/qtbase/blob/v6.8.0/src/gui/util/qundostack.cpp#L310-L324
    title: Qt v6.8.0 source, qtbase/src/gui/util/qundostack.cpp
    locator: class QUndoStack documentation, lines 310 to 324
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/idlelib/undo.py#L116-L159
    title: CPython v3.14.0 source, Lib/idlelib/undo.py
    locator: UndoDelegator.addcmd, undo_event and redo_event, lines 116 to 159
---

## Intent

The Command Processor pattern "separates the request for a service from its execution".[^1]

A command processor component manages requests as separate objects, schedules their execution, and provides additional services, for example it stores request objects for later undo.[^2]

## Problem

An application with many features needs a clear mapping from its interface to its internal functions. Users work in different ways: with pop-up menus, with keyboard shortcuts, or with a scripting language. The application also needs services that go beyond the core functions, such as undo, redo, macros, logging and scheduling of requests. These services must work in the same way for all requests, and new features must not break existing code.[^2]

If each user interface element calls the functional core directly, each element must also do the work of each service. Here the menu records its requests for undo, but the keyboard shortcut does not, so undo removes the wrong word:

```python wrong id=bypass
class Insert:
    def __init__(self, words, word):
        self.words, self.word = words, word

    def execute(self):
        self.index = len(self.words)
        self.words.append(self.word)

    def undo(self):
        del self.words[self.index]


words, history = [], []


def menu_insert(word):
    command = Insert(words, word)
    command.execute()
    history.append(command)


def key_insert(word):
    words.append(word)  # calls the core directly, so undo does not know


menu_insert("hello")
key_insert("world")
print("text:", " ".join(words))
history.pop().undo()  # meant to undo "world"
print("text:", " ".join(words))
```

```output for=bypass
text: hello world
text: world
```

## Solution

Turn each request into a command object, and give all command objects to one command processor. The pattern builds on the Command pattern of Gamma, Helm, Johnson and Vlissides, and it shows in more detail how command objects are managed.[^2]

```text
  Controller  --creates-->  Command  --uses-->  Supplier
      |                       ^
      | transfers             | implements
      v                       |
  Command Processor  ---> Abstract Command
  do_it(command)          execute()
  undo_it()               undo()
  stack of commands
```

The participants work as follows:[^2]

- The **abstract command** defines a uniform interface to execute commands. The services of the processor add other methods, for example `undo`.
- A **command** encapsulates one function request. It uses zero or more suppliers to do the work. For undo, a command saves the state of its suppliers before it executes, and it restores that state when it is undone.
- The **controller** is the interface of the application. It accepts requests, such as a request to paste text, and creates the corresponding command objects.
- The **command processor** activates the execution of commands and keeps them. It knows only the abstract command interface, so it stays independent of specific commands.
- A **supplier** gives most of the functionality that the commands need. In the editor example of the book, the supplier is the internal text representation.

## Example

The example files show one program in each language. A document is a list of words. `Insert` is a command that adds a word, and it can undo itself. A `Processor` executes each command, prints a log line, and keeps the command on a stack. `undo` takes the last command from the stack and undoes it. The program prints:

```text
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

## Consequences

The benefits:[^2]

- **Flexible activation.** Different user interface elements can create the same kind of command object, so it is easy to remap user input, for example to an emacs or a WordStar keyboard.
- **Flexible number of requests.** A new command class does not change the controller or the command processor. Complex commands can be made from existing ones, for example as macros.
- **Services in one place.** A command processor can log commands, store them to a file for replay, queue them, schedule them by time or priority, or run them in a separate thread.
- **Testability.** The command processor is an entry point for tests, and a stored sequence of commands can be applied again after an error is corrected.

Here a macro is a command made of other commands. The processor undoes it as one step:

```python run id=macro
class Insert:
    def __init__(self, words, word):
        self.words, self.word = words, word

    def execute(self):
        self.words.append(self.word)

    def undo(self):
        self.words.pop()


class Macro:
    def __init__(self, *commands):
        self.commands = commands

    def execute(self):
        for command in self.commands:
            command.execute()

    def undo(self):
        for command in reversed(self.commands):
            command.undo()


class Processor:
    def __init__(self):
        self.done = []

    def do_it(self, command):
        command.execute()
        self.done.append(command)

    def undo_it(self):
        self.done.pop().undo()


words, processor = [], Processor()
processor.do_it(Insert(words, "dear"))
processor.do_it(Macro(Insert(words, "best"), Insert(words, "regards")))
print("text:", " ".join(words))
processor.undo_it()
print("text:", " ".join(words))
```

```output for=macro
text: dear best regards
text: dear
```

The liabilities:[^2]

- **Efficiency loss.** The extra indirection costs storage and time.
- **Many command classes.** A rich application can have many command classes. Group them around abstractions, pass the supplier as a parameter to simple commands, or build macros from a few low-level commands.
- **Command parameters.** A command that asks the user for more parameters during its execution complicates event handling, because events must go both to the controller and to the active command.
- **Concurrency.** Commands that run in parallel threads need synchronization when they access shared data, for example in a supplier.

## When not to use it

- The application has few requests and no services such as undo, logging or scheduling. A controller that performs each request directly has no efficiency penalty.[^2]
- New requests, changes to services and an undo mechanism are not expected. These changes need more effort with a direct controller.[^2]

## Variants and related patterns

- **Spread controller functionality.** Several components share the role of the controller. For example, each menu button creates a command object when it is activated.[^2]
- **Combination with Interpreter.** A scripting language gives a programmable interface. The parser takes the role of the controller and builds the abstract syntax tree from command objects. The command processor is the client of the Interpreter, and it activates the commands.[^2]
- **Command.** In Command Processor, the controller takes the role of the client of the Command pattern, and the command processor takes the role of the invoker. The suppliers correspond to the receivers, but a command can use more than one supplier.[^2]

## Known uses

- The ET++ framework has command processors that support unlimited, bounded and single undo and redo. MacApp uses the pattern for undoable operations, and InterViews has an action class that is an abstract command.[^2]
- Qt's `QUndoStack` is a stack of `QUndoCommand` objects. New commands are pushed on the stack with `push()`, and they are undone and redone with `undo()` and `redo()`.[^3]
- IDLE, the editor of Python, has an `UndoDelegator`. Its `addcmd` method executes a command and adds it to an undo list, and `undo_event` and `redo_event` move through that list.[^4]

## References
