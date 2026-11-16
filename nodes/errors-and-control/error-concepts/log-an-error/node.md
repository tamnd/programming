---
tag: TFJ8M7
kind: task
slug: log-an-error
name: Log an error
aliases: [log an exception, error logging, logging errors, log the error and continue]
area: errors-and-control/error-concepts
level: core
summary: >-
  Handle an error where it happens by writing one log record at the error level, with the context and the cause as escaped fields, and then go on with the next item.
contract: contract.yaml
category: errors
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/howto/logging.html
  title: Logging HOWTO, The Python Documentation
  locator: When to use logging
- url: https://docs.python.org/3.15/library/logging.html
  title: logging, The Python Standard Library
  locator: Logger Objects
- url: https://developer.mozilla.org/en-US/docs/Web/API/console/error_static
  title: "console: error() static method, MDN Web Docs"
  locator: the introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.logging/java/util/logging/Logger.html
  title: Logger, Java SE 25 API
  locator: Method Summary, log
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.logging/java/util/logging/Level.html
  title: Level, Java SE 25 API
  locator: Field Summary, SEVERE
- url: https://learn.microsoft.com/en-us/dotnet/core/extensions/logging/overview
  title: Logging in C# and .NET, Microsoft Learn
  locator: Log level; Log exceptions
- url: https://pkg.go.dev/log/slog
  title: slog, The Go standard library
  locator: the overview; TextHandler
- url: https://docs.rs/log/latest/log/
  title: log, docs.rs
  locator: the crate description, Usage
- url: https://dave.cheney.net/2016/04/27/dont-just-check-errors-handle-them-gracefully
  title: Dave Cheney, Don't just check errors, handle them gracefully, 27 April 2016
  locator: Only handle errors once
- url: https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html
  title: Logging Cheat Sheet, OWASP Cheat Sheet Series
  locator: Data to exclude; Event collection
---

## Task

Some errors stop only one item of a job. A server that reads a list of settings can skip a bad setting and use the others. The program must not hide the error. The Python logging guide gives the tool for this case: to "Report suppression of an error without raising an exception", use "A logger’s error(), exception() or critical() method"[^1].

Write `read_ports(texts, log)`. It reads each text as a TCP port number, with the parser of [[wrap-an-error-with-context]]: the messages are `"not a number: 'TEXT'"` and `"out of range: TEXT"`. It returns the good ports in order. For each bad text, it writes one log record at the error level, with the line number and the message of the error. At the end, it writes one record at the info level with the number of good ports and the number of texts. For example, `read_ports(["8080", "x", "443"], log)` returns `[8080, 443]` and writes:

```text
level=ERROR msg="read port failed" line=2 err="not a number: 'x'"
level=INFO msg="ports read" ok=2 total=3
```

The records use the form of the text handler of Go's `log/slog`, without the time. Each field is `key=value`. A value is in double quotes when it has a space or a special character, and a backslash, a double quote, a line feed, a carriage return or a tab in it is escaped. [[structured-logging]] explains why fields are better than free text.

## Solutions

| Language | The logger | The error level | The exception in the record |
|---|---|---|---|
| Python | `logging.Logger`[^2] | `log.error`[^1] | `log.exception`, or `exc_info=True`[^2] |
| JavaScript, TypeScript | a function, `console.error` by default[^3] | `console.error`[^3] | in the message |
| Java | `java.util.logging.Logger`[^4] | `Level.SEVERE`[^5] | `log(Level, String, Throwable)`[^4] |
| C# | a `TextWriter`; in an app, `ILogger`[^6] | `LogError`[^6] | an overload with an exception[^6] |
| Go | `*slog.Logger`[^7] | `Logger.Error`[^7] | an `err` attribute |
| Rust | an `io::Write`; in an app, the log crate[^8] | `error!`[^8] | in the message |
| C, C++ | a `FILE *`, a `std::ostream` | in the text | in the message |
| Bash | the standard error | in the text | in the message |

Each solution takes the logger as a parameter, so that the test can read what it wrote. Each solution handles the error at one place: it logs the error and does not throw it or return it again.

- **Python.** The logging guide describes the level ERROR with the words "Due to a more serious problem, the software has not been able to perform some function"[^1]. `make_logger` gets the logger with `logging.getLogger`, because "Loggers should NEVER be instantiated directly"[^2]. Its formatter writes `level=` and the level name. `read_ports` calls `log.error` with `%` arguments, and `quote` escapes the message. To add the traceback, call `log.exception` in the `except` block. "Exception info is added to the logging message. This method should only be called from an exception handler"[^2].
- **JavaScript and TypeScript.** "The console.error() static method outputs a message to the console", at the error log level[^3]. `readPorts` takes a function that writes one line, and `console.error` is the default. The test gives a function that keeps the lines in a string.
- **Java.** `java.util.logging` names its top level `SEVERE`: "SEVERE is a message level indicating a serious failure"[^5]. The solution calls `log(Level level, String msg, Throwable thrown)`, which logs "a message, with associated Throwable information"[^4]. So a handler can write the stack trace. The formatter of the solution writes `ERROR` for `SEVERE` and does not write the stack trace, so that the output of the test is short.
- **C#.** .NET apps log through `ILogger`, from the NuGet package `Microsoft.Extensions.Logging`[^6]. Its `LogError` is "For errors and exceptions that can't be handled", and "The logger methods have overloads that take an exception parameter"[^6]. The solution writes to a `TextWriter`, so that it needs only the base library. It writes `"\n"` and not `WriteLine`, so that each line ends in the same way on each platform.
- **Go.** "Package slog provides structured logging, in which log records include a message, a severity level, and various other attributes expressed as key-value pairs"[^7]. The solution calls `log.Error` with the message and the pairs `line` and `err`, and gets such a record. `NewLogger` removes the time with `ReplaceAttr`, as the examples of the package do[^7]. The text handler does the escaping: "Keys and values are quoted with strconv.Quote if they contain Unicode space characters, non-printing characters"[^7].
- **Rust.** The standard library has no logger. The log crate is "A lightweight logging facade", with "five logging macros: error!, warn!, info!, debug! and trace!"[^8]. The solution writes to an `io::Write` with `writeln!`, and gives an error of the writer to the caller.
- **C and C++.** The solutions write the lines to a `FILE *` or a `std::ostream`, with a function that escapes the message. The C test reads the lines back from `tmpfile()`.
- **Bash.** `read_ports` prints the good ports on the standard output and the log lines on the standard error, so that a caller can pipe the ports to another command and still see the log.

### The escape at work

The bad text here has a line feed and a double quote. The text handler of `slog` puts the value in quotes and escapes them, so the record stays on one line:

```go run id=go-slog-escape
package main

import (
	"fmt"
	"log/slog"
	"os"
)

func main() {
	removeTime := func(groups []string, a slog.Attr) slog.Attr {
		if a.Key == slog.TimeKey && len(groups) == 0 {
			return slog.Attr{}
		}
		return a
	}
	log := slog.New(slog.NewTextHandler(os.Stdout, &slog.HandlerOptions{ReplaceAttr: removeTime}))
	text := "80\nlevel=INFO msg=\"all good\""
	err := fmt.Errorf("not a number: '%s'", text)
	log.Error("read port failed", "line", 1, "err", err)
}
```

```output for=go-slog-escape
level=ERROR msg="read port failed" line=1 err="not a number: '80\nlevel=INFO msg=\"all good\"'"
```

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty-list`: no texts. The log has only the summary, with `ok=0 total=0`.
- `empty-text`: the message is `not a number: ''`, and the record still has a value in quotes.
- `boundary`: 1 and 65535 are good, and 0 is out of range.
- `newline`, `backslash`, `carriage-return` and `tab`: each special character is escaped, so each record is one line.

## Pitfalls

- **Log and return.** Dave Cheney writes: "you should only handle errors once. Handling an error means inspecting the error value, and making a decision"[^9]. When each level logs the error and also returns it, the log gets the same error many times:

```go wrong id=go-log-and-return
package main

import (
	"log"
	"os"
	"strconv"
)

func readPort(text string) (int, error) {
	port, err := strconv.Atoi(text)
	if err != nil {
		log.Println("read port failed:", err)
		return 0, err
	}
	return port, nil
}

func loadConfig(text string) error {
	if _, err := readPort(text); err != nil {
		log.Println("load config failed:", err)
		return err
	}
	return nil
}

func main() {
	log.SetFlags(0)
	log.SetOutput(os.Stdout)
	if err := loadConfig("x"); err != nil {
		log.Println("error:", err)
	}
}
```

```output for=go-log-and-return
read port failed: strconv.Atoi: parsing "x": invalid syntax
load config failed: strconv.Atoi: parsing "x": invalid syntax
error: strconv.Atoi: parsing "x": invalid syntax
```

Pick one: log the error and stop there, or wrap it and return it, as in [[wrap-an-error-with-context]]. The level that handles it writes the log record.

- **Log injection.** The OWASP cheat sheet says: "Perform sanitization on all event data to prevent log injection attacks e.g. carriage return (CR), line feed (LF) and delimiter characters"[^10]. Here the bad text goes into the record with no escape, and it makes a second record that looks real:

```python wrong id=python-log-injection
import logging
import sys

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format="level=%(levelname)s %(message)s")
log = logging.getLogger("ports")

text = '80\nlevel=INFO msg="all good"'
log.error("msg=\"read port failed\" line=1 err=not a number: '%s'", text)
```

```output for=python-log-injection
level=ERROR msg="read port failed" line=1 err=not a number: '80
level=INFO msg="all good"'
```

The solutions put each value in quotes and escape it.

- **Secrets in the log.** An error message can hold the input that caused it. The OWASP cheat sheet lists data that "should usually not be recorded directly in the logs", such as "Session identification values", "Access tokens" and "Authentication passwords"[^10]. Remove or mask such a value before it goes into the message.
- **A level that the logger drops.** In Python, "The default level is WARNING, which means that only events of this severity and higher will be tracked, unless the logging package is configured to do otherwise"[^1]. Without `setLevel(logging.INFO)`, the summary record of `read_ports` goes nowhere.

## Related tasks

- [[wrap-an-error-with-context]]: add context to an error and return it, when this level cannot handle it.
- [[define-a-custom-error-type]]: an error with fields, which a logger can write as fields.

## References
