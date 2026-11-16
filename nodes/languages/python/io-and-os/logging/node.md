---
tag: QSJ6J5
kind: feature
owner: python
slug: logging
name: Logging
aliases: [logging module, logging.getLogger, basicConfig, log levels, log handlers, log formatters, NullHandler]
area: languages/python/io-and-os
level: core
summary: >-
  The logging module sends leveled messages from named loggers to handlers, which write them through formatters. basicConfig() sets up the root logger, and libraries only create loggers.
grammar: "logging.getLogger(name=None)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/logging.html", locator: "logging module"}
see_also: [python/standard-streams, python/error-output-redirection-and-program-termination, python/output-formatting]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#logging
    title: The Python Tutorial, 11. Brief tour of the standard library, part II
    locator: 11.5. Logging
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/logging.html
    title: logging, Python 3.15 documentation
    locator: module introduction; Logger.propagate; Logger.setLevel(); Logger.debug(); Logger.exception(); Formatter; basicConfig(); lastResort
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/logging.html
    title: Logging HOWTO, Python 3.15 documentation
    locator: When to use logging; Advanced logging tutorial; Configuring logging for a library; Optimization
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/logging.handlers.html
    title: logging.handlers, Python 3.15 documentation
    locator: StreamHandler; FileHandler; NullHandler
    accessed: 2026-11-16
  - url: https://pkg.go.dev/log
    title: Package log, Go standard library
    locator: Overview
    accessed: 2026-11-16
  - url: https://pkg.go.dev/log/slog
    title: Package slog, Go standard library
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import logging
import sys

logging.basicConfig(stream=sys.stdout, level=logging.INFO,
                    format='%(levelname)s %(name)s: %(message)s')
log = logging.getLogger('app.db')

log.debug('not shown')
log.info('connected to %s', 'db.example')
log.warning('slow query: %.1f s', 2.5)
```

```output for=synopsis
INFO app.db: connected to db.example
WARNING app.db: slow query: 2.5 s
```

`basicConfig()` gives the root logger a handler that writes to standard output with the given format. The `app.db` logger passes its records up to that handler, and the DEBUG message falls below the INFO level.

## Semantics

**r1.** "The logging module offers a full featured and flexible logging system."[^1] "The key benefit of having the logging API provided by a standard library module is that all Python modules can participate in logging, so your application log can include your own messages integrated with messages from third-party modules."[^2]

**r2.** The levels, from least to most severe, are DEBUG, INFO, WARNING, ERROR and CRITICAL.[^1] "The default level is WARNING, which means that only events of this severity and higher will be tracked, unless the logging package is configured to do otherwise."[^3] `setLevel(level)` sets a threshold: "Logging messages which are less severe than level will be ignored".[^2]

**r3.** "Loggers expose the interface that application code directly uses. Handlers send the log records (created by loggers) to the appropriate destination. Filters provide a finer grained facility for determining which log records to output. Formatters specify the layout of log records in the final output."[^3]

**r4.** Loggers form a tree by name: "a logger named ‘scan’ is the parent of loggers ‘scan.text’, ‘scan.html’ and ‘scan.pdf’."[^3] "Multiple calls to getLogger() with the same name will always return a reference to the same Logger object."[^2] The convention is "a module-level logger, in each module which uses logging, named as follows: logger = logging.getLogger(__name__)".[^3]

**r5.** With `propagate` true, "events logged to this logger will be passed to the handlers of higher level (ancestor) loggers, in addition to any handlers attached to this logger. Messages are passed directly to the ancestor loggers’ handlers - neither the level nor filters of the ancestor loggers in question are considered."[^2] Example: [`handlers`](#handlers).

**r6.** The message is a format string: "the args are the arguments which are merged into msg using the string formatting operator."[^2] "Formatting of message arguments is deferred until it cannot be avoided."[^3] Example: [`deferred`](#deferred).

**r7.** `basicConfig()` configures the root logger "by either creating a StreamHandler with a default Formatter or using the given formatter instance".[^2] "The functions debug(), info(), warning(), error() and critical() will call basicConfig() automatically if no handlers are defined for the root logger."[^2] "This function does nothing if the root logger already has handlers configured, unless the keyword argument force is set to True."[^2]

**r8.** With no configuration at all, records go to `logging.lastResort`, "a StreamHandler writing to sys.stderr with a level of WARNING".[^2] Example: [`default`](#default).

## Examples

```python run id=default
import logging

logging.debug('Debugging information')
logging.info('Informational message')
logging.warning('Warning:config file %s not found', 'server.conf')
logging.error('Error occurred')
logging.critical('Critical error -- shutting down')
print('done')
```

```output for=default
done
```

```output for=default stream=stderr
WARNING:root:Warning:config file server.conf not found
ERROR:root:Error occurred
CRITICAL:root:Critical error -- shutting down
```

This is the tutorial's example. "By default, informational and debugging messages are suppressed and the output is sent to standard error."[^1]

```python run id=handlers
import logging
import sys

root = logging.getLogger()
handler = logging.StreamHandler(sys.stdout)
handler.setFormatter(logging.Formatter('[%(name)s] %(levelname)s %(message)s'))
root.addHandler(handler)
root.setLevel(logging.WARNING)

net = logging.getLogger('app.net')
net.setLevel(logging.DEBUG)
net.debug('sending %d bytes', 3)
logging.getLogger('app.db').info('hidden by the root level')
```

```output for=handlers
[app.net] DEBUG sending 3 bytes
```

`app.net` has its own DEBUG level, so its record reaches the root handler even though the root level is WARNING (rule r5). `app.db` has no level of its own. A new logger gets NOTSET, which means "delegation to the parent when the logger is a non-root logger", and "the root logger is created with level WARNING."[^2] A `StreamHandler` uses the given stream, "otherwise, sys.stderr will be used."[^4]

```python run id=file
import logging

log = logging.getLogger('worker')
handler = logging.FileHandler('worker.log', encoding='utf-8')
handler.setFormatter(logging.Formatter('%(levelname)s:%(message)s'))
log.addHandler(handler)

try:
    1 / 0
except ZeroDivisionError:
    log.exception('job %d failed', 7)
handler.close()

with open('worker.log', encoding='utf-8') as f:
    lines = f.read().splitlines()
print(lines[0])
print(lines[1])
print(lines[-1])
```

```output for=file
ERROR:job 7 failed
Traceback (most recent call last):
ZeroDivisionError: division by zero
```

`FileHandler` opens a file and uses it "as the stream for logging. If mode is not specified, 'a' is used."[^4] `exception()` "Logs a message with level ERROR on this logger." and adds the exception information. "This method should only be called from an exception handler."[^2]

```python run id=library
import logging

logging.getLogger('quietlib').addHandler(logging.NullHandler())
logging.getLogger('quietlib.io').warning('dropped')
logging.getLogger('loudlib').warning('reaches stderr')
print('done')
```

```output for=library
done
```

```output for=library stream=stderr
reaches stderr
```

The `quietlib.io` record finds the `NullHandler` on its parent, whose `emit()` "does nothing."[^4] The `loudlib` record finds no handler, so the last-resort handler takes it (rule r8). "The end result is to just print the message to sys.stderr."[^2]

## Pitfalls

Pass the values as arguments, not as an f-string. An f-string is built even when the level is off:

```python run id=deferred
import logging

class Expensive:
    def __str__(self):
        print('formatting!')
        return 'big'

log = logging.getLogger('app')
log.debug('value %s', Expensive())
log.debug(f'value {Expensive()}')
print(log.isEnabledFor(logging.DEBUG))
```

```output for=deferred
formatting!
False
```

The first call never formats its argument (rule r6). For costly arguments, `isEnabledFor()` "takes a level argument and returns true if the event would be created by the Logger for that level of call."[^3]

A second `basicConfig()` call does nothing:

```python run id=basicconfig
import logging
import sys

logging.basicConfig(stream=sys.stdout, format='first: %(message)s')
logging.basicConfig(stream=sys.stdout, format='second: %(message)s')
logging.warning('hello')
logging.basicConfig(stream=sys.stdout, format='third: %(message)s', force=True)
logging.warning('hello')
```

```output for=basicconfig
first: hello
third: hello
```

With `force=True`, "any existing handlers attached to the root logger are removed and closed, before carrying out the configuration".[^2] A call to `logging.warning()` before any configuration also counts as a first call (rule r7).

Libraries should not configure logging. "It is strongly advised that you do not log to the root logger in your library."[^3] "It is strongly advised that you do not add any handlers other than NullHandler to your library’s loggers."[^3]

## Comparison

Go has two packages. "Package log implements a simple logging package."[^5] "Package slog provides structured logging, in which log records include a message, a severity level, and various other attributes expressed as key-value pairs."[^6] Python's logging has the levels too, and adds the logger tree of rule r4.
