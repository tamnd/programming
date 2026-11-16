---
tag: JY353Z
kind: feature
owner: python
slug: signals
name: Signals
aliases: [signal module, signal.signal, SIGTERM, SIGINT, KeyboardInterrupt, signal handler, signal.alarm, graceful shutdown]
area: languages/python/io-and-os
level: advanced
summary: >-
  signal.signal() installs a Python function as the handler of a signal. The handler runs in the main thread between bytecode instructions. SIGINT raises KeyboardInterrupt by default.
grammar: "signal.signal(signalnum, handler)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/signal.html", locator: "signal module"}
see_also: [python/subprocess, python/error-output-redirection-and-program-termination, python/the-exception-hierarchy, python/operating-system-interface]
sources:
  - url: https://docs.python.org/3.15/library/signal.html
    title: signal, Python 3.15 documentation
    locator: General rules; Execution of Python signal handlers; Signals and threads; Module contents; Examples; Note on SIGPIPE; Note on signal handlers and exceptions
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os/signal
    title: Package signal, Go standard library
    locator: Overview; Default behavior of signals in Go programs; func Notify
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import signal

stop = False

def on_term(signum, frame):
    global stop
    print('got', signal.Signals(signum).name)
    stop = True

signal.signal(signal.SIGTERM, on_term)

count = 0
while not stop:
    count += 1
    if count == 3:
        signal.raise_signal(signal.SIGTERM)
print('stopped after', count)
```

```output for=synopsis
got SIGTERM
stopped after 3
```

The handler only sets a flag, and the loop checks the flag and ends on its own. Here the program sends SIGTERM to itself. A service manager or `kill` would send it from outside.

## Semantics

**r1.** "The signal.signal() function allows defining custom handlers to be executed when a signal is received."[^1] It will "Set the handler for signal signalnum to the function handler. handler can be a callable Python object taking two arguments (see below), or one of the special values signal.SIG_IGN or signal.SIG_DFL. The previous signal handler will be returned".[^1]

**r2.** "The handler is called with two arguments: the signal number and the current stack frame".[^1] `signal.Signals` is an "enum.IntEnum collection of SIG* constants",[^1] so `Signals(signum).name` gives the name.

**r3.** "A small number of default handlers are installed: SIGPIPE is ignored (so write errors on pipes and sockets can be reported as ordinary Python exceptions) and SIGINT is translated into a KeyboardInterrupt exception if the parent process has not changed it."[^1] Example: [`sigint`](#sigint).

**r4.** "A Python signal handler does not get executed inside the low-level (C) signal handler. Instead, the low-level signal handler sets a flag which tells the virtual machine to execute the corresponding Python signal handler at a later point (for example, at the next bytecode instruction)."[^1] "A long-running calculation implemented purely in C (such as regular expression matching on a large body of text) may run uninterrupted for an arbitrary amount of time, regardless of any signals received."[^1]

**r5.** "Python signal handlers are always executed in the main Python thread of the main interpreter, even if the signal was received in another thread."[^1] "Besides, only the main thread of the main interpreter is allowed to set a new signal handler."[^1] From another thread, `signal.signal()` raises `ValueError`.[^1] Example: [`thread`](#thread).

**r6.** "A handler for a particular signal, once set, remains installed until it is explicitly reset".[^1] `SIG_DFL` "will simply perform the default function for the signal", and `SIG_IGN` "will simply ignore the given signal."[^1]

**r7.** "If the handler raises an exception, it will be raised "out of thin air" in the main thread."[^1] Example: [`raise-in-handler`](#raise-in-handler).

**r8.** `signal.alarm(time)` "requests that a SIGALRM signal be sent to the process in time seconds."[^1] `signal.raise_signal(signum)` "Sends a signal to the calling process."[^1] Example: [`alarm`](#alarm).

## Examples

```python run id=sigint
import signal

try:
    signal.raise_signal(signal.SIGINT)
    print('not reached')
except KeyboardInterrupt:
    print('KeyboardInterrupt')
print(signal.strsignal(signal.SIGINT))
```

```output for=sigint
KeyboardInterrupt
Interrupt
```

The default SIGINT handler raises `KeyboardInterrupt` (rule r3). `strsignal()` "Returns the description of signal signalnum, such as "Interrupt" for SIGINT."[^1]

```python run id=alarm
import signal
import time

def on_alarm(signum, frame):
    raise TimeoutError('took too long')

signal.signal(signal.SIGALRM, on_alarm)
signal.alarm(1)
try:
    time.sleep(5)
except TimeoutError as err:
    print(err)
finally:
    signal.alarm(0)
```

```output for=alarm
took too long
```

This is the pattern of the docs' example: "set a 5-second alarm before opening the file; if the operation takes too long, the alarm signal will be sent, and the handler raises an exception."[^1] `alarm(0)` cancels a pending alarm (rule r8).

```python run id=ignore
import signal

old = signal.signal(signal.SIGTERM, signal.SIG_IGN)
signal.raise_signal(signal.SIGTERM)
print('still running')
print(old == signal.SIG_DFL)
```

```output for=ignore
still running
True
```

With `SIG_IGN`, SIGTERM does nothing. `signal()` returns the old handler, which was the default (rules r1 and r6).

```python run id=thread
import signal
import threading

def install():
    try:
        signal.signal(signal.SIGTERM, signal.SIG_IGN)
    except ValueError as err:
        print(type(err).__name__)

t = threading.Thread(target=install)
t.start()
t.join()
```

```output for=thread
ValueError
```

Only the main thread can set a handler (rule r5).

## Pitfalls

An exception from a handler can come at any point in the main thread:

```python fails id=raise-in-handler expect=exception:RuntimeError
import signal

def on_usr1(signum, frame):
    raise RuntimeError('stop now')

signal.signal(signal.SIGUSR1, on_usr1)
total = 0
for i in range(10):
    total += i
    if i == 4:
        signal.raise_signal(signal.SIGUSR1)
print('not reached', total)
```

The docs warn that such an exception "may on rare occasions put the program in an unexpected state."[^1] "applications that are complex or require high reliability should avoid raising exceptions from signal handlers. They should also avoid catching KeyboardInterrupt as a means of gracefully shutting down. Instead, they should install their own SIGINT handler."[^1] A handler that sets a flag, as in the synopsis, is the safe form.

Do not take a lock in a handler. "Synchronization primitives such as threading.Lock should not be used within signal handlers. Doing so can lead to unexpected deadlocks."[^1]

Signals do not reach a thread. They "can't be used as a means of inter-thread communication. You can use the synchronization primitives from the threading module instead."[^1]

Output piped to `head` can end in "an exception like BrokenPipeError: [Errno 32] Broken pipe."[^1] The docs advise against the quick fix: "Do not set SIGPIPE's disposition to SIG_DFL in order to avoid BrokenPipeError."[^1]

## Comparison

Go's "Package signal implements access to incoming signals."[^2] In Go, "A SIGHUP, SIGINT, or SIGTERM signal causes the program to exit" by default.[^2] `signal.Notify` "causes package signal to relay incoming signals to c", a channel,[^2] so a goroutine receives the signal as a value. Python instead calls a handler function in the main thread (rule r5), and SIGINT becomes an exception (rule r3).
