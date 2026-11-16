---
tag: FYKM3T
kind: feature
owner: python
slug: gui-libraries
name: GUI libraries
aliases: [tkinter, Tk, ttk, Qt for Python, PySide6, Kivy, wxPython, Toga, BeeWare, event loop, mainloop]
area: languages/python/ecosystem
level: advanced
summary: >-
  tkinter, the interface to Tcl/Tk, comes with most builds of Python. Qt for Python, wxPython, Kivy and Toga are on PyPI. A GUI runs an event loop, so a handler must return fast.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/tkinter.html", locator: "tkinter, Python interface to Tcl/Tk"}
see_also: [python/the-python-package-index, python/cli-and-automation-libraries]
sources:
  - url: https://docs.python.org/3/library/tkinter.html
    title: tkinter, Python interface to Tcl/Tk
    locator: introduction; Threading model
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/tk.html
    title: Graphical user interfaces with Tk
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/faq/gui.html
    title: Graphic User Interface FAQ
    locator: What GUI toolkits exist for Python?; How do I freeze Tkinter applications?
    accessed: 2026-11-16
  - url: https://doc.qt.io/qtforpython-6/
    title: Qt for Python
    locator: introduction; Quick Start
    accessed: 2026-11-16
  - url: https://wxpython.org/
    title: Welcome to wxPython!
    locator: home page
    accessed: 2026-11-16
  - url: https://kivy.org/
    title: "Kivy: Cross-platform Python Framework for GUI apps Development"
    locator: home page
    accessed: 2026-11-16
  - url: https://toga.readthedocs.io/en/stable/
    title: Toga
    locator: introduction
    accessed: 2026-11-16
  - url: https://fyne.io/
    title: Fyne.io
    locator: home page
    accessed: 2026-11-16
---

## Synopsis

```python norun reason=gui group=counter file=counter.py id=hello-tk
import tkinter as tk
from tkinter import ttk

root = tk.Tk()
root.title('Counter')
count = tk.IntVar(value=0)
ttk.Label(root, textvariable=count).pack(padx=40, pady=10)
ttk.Button(root, text='Add one', command=lambda: count.set(count.get() + 1)).pack(pady=10)
root.mainloop()
```

The program needs a display, so the site does not run it. This program reads it and prints its last statement and its number of handlers:

```python run group=counter file=main.py id=hello-tk-read
import ast

with open('counter.py') as f:
    tree = ast.parse(f.read())
print(ast.unparse(tree.body[-1]))
print(sum(isinstance(node, ast.Lambda) for node in ast.walk(tree)), 'handler')
```

```output for=hello-tk-read
root.mainloop()
1 handler
```

The program opens a window with a number and a button. `tkinter` comes with most builds of Python, and it is the interface to the Tcl/Tk toolkit. `root.mainloop()` runs the event loop: it waits for an event, such as a click, and calls the handler of the event. Qt for Python, wxPython, Kivy and Toga are GUI libraries on PyPI, and each one also has an event loop.

## Semantics

**r1.** "The tkinter package (“Tk interface”) is the standard Python interface to the Tcl/Tk GUI toolkit. Both Tk and tkinter are available on most Unix platforms, including macOS, as well as on Windows systems."[^1] "Running python -m tkinter from the command line should open a window demonstrating a simple Tk interface, letting you know that tkinter is properly installed on your system".[^1]

**r2.** "The tkinter package is a thin object-oriented layer on top of Tcl/Tk. To use tkinter, you don’t need to write Tcl code, but you will need to consult the Tk documentation, and occasionally the Tcl documentation."[^2] The module `tkinter.ttk` is the extension of the package.[^2] Example: [`hello-tk`](#hello-tk).

**r3.** "Standard builds of Python include an object-oriented interface to the Tcl/Tk widget set, called tkinter. This is probably the easiest to install (since it comes included with most binary distributions of Python) and use."[^3]

**r4.** "Tcl/Tk applications are normally event-driven, meaning that after initialization, the interpreter runs an event loop (that is, Tk.mainloop) and responds to events."[^1] "Because it is single-threaded, event handlers must respond quickly, otherwise they will block other events from being processed."[^1] Example: [`blocking-handler`](#blocking-handler).

**r5.** "To avoid this, any long-running computations should not run in an event handler, but are either broken into smaller pieces using timers, or run in another thread."[^1] Examples: [`chunked-work`](#chunked-work) and [`worker-thread`](#worker-thread).

**r6.** "Qt for Python offers the official Python bindings for Qt, which enables you to use Python to write your Qt applications."[^4] It has two main components, PySide6 and the binding generator Shiboken6.[^4] "You can obtain the latest stable version by running pip install pyside6."[^4]

**r7.** wxPython is "the cross-platform GUI toolkit for the Python language."[^5] "With wxPython software developers can create truly native user interfaces for their Python applications".[^5]

**r8.** "Kivy runs on Android, iOS, Linux, macOS and Windows."[^6]

**r9.** "Toga is a Python native, OS native, cross-platform GUI toolkit."[^7] "Toga is part of the BeeWare suite."[^7]

## Examples

The examples below use a small model of an event loop with a clock in milliseconds, so they run without a display. `after(ms, fn)` asks the loop to call `fn` later, as `after` does in Tk. In this model, `work(ms)` stands for a handler that keeps the loop busy for `ms` milliseconds.

A handler that takes 500 ms blocks every other event. A redraw that was due at 10 ms runs at 500 ms (rule r4):

```python run id=blocking-handler
import heapq
import itertools

class Loop:
    def __init__(self):
        self.now, self.events, self.seq = 0, [], itertools.count()
    def after(self, ms, fn):
        heapq.heappush(self.events, (self.now + ms, next(self.seq), fn))
    def work(self, ms):
        self.now += ms
    def run(self):
        while self.events:
            due, _, fn = heapq.heappop(self.events)
            self.now = max(self.now, due)
            fn()

loop = Loop()
loop.after(0, lambda: loop.work(500))
loop.after(10, lambda: print('redraw due at 10 ms, ran at', loop.now, 'ms'))
loop.run()
```

```output for=blocking-handler
redraw due at 10 ms, ran at 500 ms
```

The same work in five pieces of 100 ms. Each piece schedules the next one with `after`, so the redraw runs between two pieces (rule r5):

```python run id=chunked-work
import heapq
import itertools

class Loop:
    def __init__(self):
        self.now, self.events, self.seq = 0, [], itertools.count()
    def after(self, ms, fn):
        heapq.heappush(self.events, (self.now + ms, next(self.seq), fn))
    def work(self, ms):
        self.now += ms
    def run(self):
        while self.events:
            due, _, fn = heapq.heappop(self.events)
            self.now = max(self.now, due)
            fn()

loop = Loop()

def piece(left):
    loop.work(100)
    if left > 1:
        loop.after(0, lambda: piece(left - 1))
    else:
        print('work done at', loop.now, 'ms')

loop.after(0, lambda: piece(5))
loop.after(10, lambda: print('redraw due at 10 ms, ran at', loop.now, 'ms'))
loop.run()
```

```output for=chunked-work
redraw due at 10 ms, ran at 100 ms
work done at 500 ms
```

A worker thread does the work and puts the result in a `queue.Queue`. The thread of the event loop checks the queue with a timer and updates the window. Only that thread touches the widgets (rule r5):

```python run id=worker-thread
import queue
import threading

results = queue.Queue()

def worker(n):
    results.put(sum(range(n)))

threading.Thread(target=worker, args=(1_000_000,), daemon=True).start()

def poll():
    try:
        value = results.get(timeout=5)
    except queue.Empty:
        return 'still waiting'
    return f'show result {value}'

print(poll())
```

```output for=worker-thread
show result 499999500000
```

In a real `tkinter` program, `poll` uses `get_nowait()` and calls `root.after(100, poll)` again while the queue is empty, so the event loop never waits.

## Pitfalls

Do not run long work in an event handler (rule r4). The window stops responding until the handler returns. Break the work into pieces with `after`, or run it in another thread (rule r5).

"If the Tcl interpreter is not running the event loop and processing events, any tkinter calls made from threads other than the one running the Tcl interpreter will fail."[^1] Send results from a worker to the event loop through a queue, as in the example [`worker-thread`](#worker-thread).

A frozen `tkinter` program still needs Tcl and Tk. "When freezing Tkinter applications, the applications will not be truly stand-alone, as the application will still need the Tcl and Tk libraries."[^3]

`tkinter` is in most builds, but not in each one (rule r1). Run `python -m tkinter` to check an installation.

## Comparison

Fyne is a GUI toolkit for Go. It says: "Create desktop and mobile apps from a single Go codebase using the fast, native UI toolkit."[^8] A Go program imports it from the module path `fyne.io/fyne/v2`. A Python program can use `tkinter` from most builds of Python with no download (rule r3).

[^1]: [tkinter, Python interface to Tcl/Tk](https://docs.python.org/3/library/tkinter.html)
[^2]: [Graphical user interfaces with Tk](https://docs.python.org/3/library/tk.html)
[^3]: [Graphic User Interface FAQ](https://docs.python.org/3/faq/gui.html)
[^4]: [Qt for Python](https://doc.qt.io/qtforpython-6/)
[^5]: [Welcome to wxPython!](https://wxpython.org/)
[^6]: [Kivy: Cross-platform Python Framework for GUI apps Development](https://kivy.org/)
[^7]: [Toga](https://toga.readthedocs.io/en/stable/)
[^8]: [Fyne.io](https://fyne.io/)
