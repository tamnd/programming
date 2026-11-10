---
tag: NEW-structured-logging
kind: concept
slug: structured-logging
name: Structured logging
aliases: [logging, log levels, log records, key-value logging, JSON logging, semantic logging]
area: debugging-and-observability/logging
level: core
summary: >-
  A structured log writes each event as a record with a level, a message and named fields, such as JSON or key=value pairs, so that a program can filter and search the log.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2009
  people: [Eric Allman, Rainer Gerhards]
  note: Allman wrote the syslog daemon and protocol for BSD. RFC 3164 described that protocol in 2001, and RFC 5424 replaced it in 2009 with a format that adds STRUCTURED-DATA, named parameters inside each message.
see_also: [sensitive-information-exposure, hard-coded-credentials, exception-handling, interactive-debugging, cpu-profiling, delta-debugging]
sources:
  - url: https://docs.python.org/3.14/library/logging.html
    title: Python documentation, logging, Logging facility for Python
    locator: Logger Objects (debug and its keyword argument extra, exception); Logging Levels; LoggerAdapter Objects
  - url: https://docs.python.org/3.14/howto/logging.html
    title: Python documentation, Logging HOWTO
    locator: Basic Logging Tutorial (the default level is WARNING); Optimization
  - url: https://pkg.go.dev/log/slog
    title: Go standard library, package log/slog
    locator: the overview; Levels; Customizing a type's logging behavior; Performance considerations; HandlerOptions; Logger.Log
  - url: https://pkg.go.dev/cmd/vet
    title: Go command vet
    locator: the list of checks (slog, check for invalid structured logging calls)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.Logger.Level.html
    title: Java SE 25 API, System.Logger.Level
    locator: the class description (System.Logger Severity Level Mapping)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.LoggerFinder.html
    title: Java SE 25 API, System.LoggerFinder
    locator: Default Implementation
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.logging/java/util/logging/Level.html
    title: Java SE 25 API, java.util.logging.Level
    locator: the class description (the levels in descending order)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.logging/java/util/logging/ConsoleHandler.html
    title: Java SE 25 API, java.util.logging.ConsoleHandler
    locator: the class description (publishes to System.err with SimpleFormatter)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/text/MessageFormat.html
    title: Java SE 25 API, java.text.MessageFormat
    locator: the table of FormatType and FormatStyle; the table of format(Object[], StringBuffer, FieldPosition) (an argument instanceof Number)
  - url: https://openjdk.org/jeps/264
    title: JEP 264, Platform Logging API and Service
    locator: the header (owner, created) and Summary
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/logging
    title: .NET documentation, Logging in C# and .NET
    locator: the introduction; Log level; Log message template
  - url: https://docs.rs/log/latest/log/
    title: Rust crate log, documentation
    locator: the crate description; Use; Structured logging
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Logging_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Logging Cheat Sheet
    locator: Event attributes; Data to exclude; Log Injection
  - url: https://cwe.mitre.org/data/definitions/117.html
    title: CWE-117, Improper Output Neutralization for Logs
    locator: Description; Alternate Terms
  - url: https://cwe.mitre.org/data/definitions/532.html
    title: CWE-532, Insertion of Sensitive Information into Log File
    locator: Description
  - url: https://www.rfc-editor.org/rfc/rfc3164
    title: C. Lonvick, RFC 3164, The BSD syslog Protocol, August 2001
    locator: Abstract; 4.1 syslog Message Parts; Acknowledgements
  - url: https://www.rfc-editor.org/rfc/rfc5424
    title: R. Gerhards, RFC 5424, The Syslog Protocol, March 2009
    locator: 6.2.1 PRI (Table 2, Syslog Message Severities); 6.3 STRUCTURED-DATA; 6.3.3 SD-PARAM; 6.3.5 Examples
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/syslog.html
    title: POSIX.1-2024, closelog, openlog, setlogmask, syslog
    locator: SYNOPSIS (XSI); DESCRIPTION
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/logger.html
    title: POSIX.1-2024, logger
    locator: DESCRIPTION
---

## Idea

A log is the record that a running program keeps of what it did. A developer reads it after a failure, when the program cannot be stopped in a debugger. An operator reads it to see what a server did last night. A program reads it to count errors and to raise an alert.

A plain log line is text for a human:

```text
order 1042 placed by bob, total $19.99
```

A program that must find all orders over $10 has to parse this sentence with a regular expression. The next developer who changes the sentence breaks the parser. A structured log writes the same event as a record with named fields:

```text
{"level":"INFO","msg":"order placed","order":1042,"user":"bob","total_cents":1999}
level=INFO msg="order placed" order=1042 user=bob total_cents=1999
```

The message is a fixed name for the kind of event. The values that change go in fields. A tool can then select `msg="order placed" and total_cents > 1000` without a parser for each sentence.[^3]

Each record also has a level, which says how important the event is. The program sets a minimum level, and the logger drops the records below it. A developer can leave debug records in the code and turn them on when a failure needs them.[^1] [^3]

## How it works

### Records, loggers, handlers

Most logging libraries have the same parts:

- **The record.** One event: a time, a level, a message and a set of key-value pairs. Go's `log/slog` defines a record in these words.[^3]
- **The logger.** The object that the code calls, often with a name such as `shop.orders`. In Python, the names form a hierarchy: `shop.orders` is a child of `shop`. A logger with no level of its own uses the level of its nearest ancestor that has one.[^1]
- **The handler.** It receives the records that pass the level check and writes them to a destination: standard output, a file, the system log or a network service.[^1] [^3]
- **The formatter.** It turns a record into bytes: a line of text, `key=value` pairs or JSON.[^1]

The call site creates the record. The configuration decides where it goes and how it looks. A library logs to a named logger and leaves the configuration to the application.

### Levels

A level is a number. A higher number is more severe. The names differ between systems, but the order is the same:

| System | Levels, from low to high |
|---|---|
| Syslog (RFC 5424) | Debug 7, Informational 6, Notice 5, Warning 4, Error 3, Critical 2, Alert 1, Emergency 0 [^17] |
| Python `logging` | DEBUG 10, INFO 20, WARNING 30, ERROR 40, CRITICAL 50 [^1] |
| Go `log/slog` | Debug −4, Info 0, Warn 4, Error 8 [^3] |
| Java `System.Logger` | TRACE, DEBUG, INFO, WARNING, ERROR [^5] |
| `java.util.logging` | FINEST, FINER, FINE, CONFIG, INFO, WARNING, SEVERE [^7] |
| .NET `LogLevel` | Trace 0, Debug 1, Information 2, Warning 3, Error 4, Critical 5 [^11] |
| Rust `log` | trace, debug, info, warn, error [^12] |

Syslog numbers the levels in the other direction: 0 is the most severe.[^17] Go leaves a gap of 4 between its levels, so that other schemes can map named levels between them. Info is 0, the zero value of an integer, so it is the default.[^3]

A level check must be cheap, because a program calls the logger often and drops most records. Python formats the message only when a handler will write the record. The arguments of the call are still computed first.[^2] Go's documentation gives the same warning: "The arguments to a log call are always evaluated, even if the log event is discarded."[^3]

### Fields

A field has a name and a value. The libraries add fields in three ways:

- **At the call.** Go takes pairs after the message: `log.Info("order placed", "order", 1042)`.[^3] Python takes a dictionary in `extra=`, which becomes attributes of the record.[^1] .NET takes a message template such as `"Order {OrderId} placed"`, and the names in braces become field names.[^11]
- **On a child logger.** Go's `Logger.With("request", id)` returns a logger that adds the field to every record.[^3] Python's `LoggerAdapter` does the same.[^1] All records of one request can then be found by its id.
- **From the value.** In Go, a type with a `LogValue` method decides how it appears in a record. The documentation suggests this to redact a password.[^3]

### Formats

A structured format must keep each record on one line and each value inside its field. JSON escapes a newline in a string as `\n` and a quote as `\"`. A `key=value` format must quote a value that has a space and escape the quotes and newlines in it. RFC 5424 defines a format for syslog. A message can carry STRUCTURED-DATA elements, each with an id and named parameters, and the characters `"`, `\` and `]` must be escaped in a value:[^17]

```text
[exampleSDID@32473 iut="3" eventSource="Application" eventID="1011"]
```

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `logging`: levels, named loggers, handlers, formatters; `extra=` for fields; JSON needs a small `Formatter` |
| Go | stdlib | `log/slog`: key-value pairs, `JSONHandler` and `TextHandler`, `With`, `LogValuer` |
| Java | stdlib | `System.Logger` and `java.util.logging`; no key-value pairs, so fields go in the message |
| C# | library | `ILogger` in `Microsoft.Extensions.Logging`, with message templates; `System.Text.Json` for a hand-made record |
| Rust | library | the `log` facade with a logger crate; `{:?}` quotes and escapes a value |
| JavaScript | idiom | `console.log` with `JSON.stringify` |
| TypeScript | idiom | as JavaScript, with a union type for the levels |
| C | idiom | `fprintf` and a function that escapes values; `syslog()` on POSIX systems with the XSI option |
| C++ | idiom | `std::print` with `{:?}` and `std::source_location` |
| Kotlin | stdlib | the JDK's `System.Logger` and `java.util.logging` on the JVM |
| Bash | idiom | `printf` to standard error; the `logger` command for the system log |
| PowerShell | idiom | the Debug, Verbose, Warning and Error streams; `ConvertTo-Json` for a record |
| SQL, Regex, HTML, CSS | n/a | the language does not write log records |

### Python

The `logging` module has five named levels with the numbers 10 to 50.[^1] The root logger has the level WARNING by default, so a program that calls `logging.info` without any configuration sees nothing.[^2] The module has no JSON formatter. The example subclasses `Formatter` and reads the fields that `extra=` put on the record. `log.exception` logs at ERROR and adds the active exception.[^1]

### Go

`log/slog` was designed for key-value records. `JSONHandler` writes one JSON object per record and `TextHandler` writes `key=value` pairs. `HandlerOptions.Level` sets the minimum level, and the default is Info. `HandlerOptions.ReplaceAttr` can rename, change or remove any attribute. The examples remove the time, so the output is the same on each run.[^3] A call with a missing key does not fail: slog writes the value under the key `!BADKEY`.[^3] `go vet` has a `slog` check that reports these calls.[^4]

### Java

Java has two APIs in the JDK. `java.util.logging` has the levels SEVERE, WARNING, INFO, CONFIG, FINE, FINER and FINEST.[^7] `System.Logger`, from JEP 264, is a small API that the JDK's own classes use. A service can plug in any logging backend.[^10] By default, `System.Logger` sends records to `java.util.logging` when the `java.logging` module is present. Its levels map to that backend: TRACE to FINER, DEBUG to FINE, ERROR to SEVERE.[^5] [^6]

The default `ConsoleHandler` writes to `System.err` with `SimpleFormatter`.[^8] The output of `SimpleFormatter` starts with the date and time. The example replaces it with a handler that writes to `System.out` with a fixed format. Neither API has key-value pairs. A program puts the fields in the message, and `MessageFormat` formats a number argument by the locale, so `{0}` turns 1042 into `1,042`.[^9]

### C# and .NET

The .NET logging API, `ILogger`, supports structured logging with message templates. In `"Order {OrderId} placed"`, `OrderId` becomes the name of a field. The order of the placeholders, not their names, decides which argument fills each one. The documentation warns that string interpolation in a log call can cause performance issues.[^11] `ILogger` is in the package `Microsoft.Extensions.Logging`, so the example writes JSON with `Utf8JsonWriter` from the base library instead.

### Rust

The standard library has no logger. The `log` crate is a facade: libraries call its macros `error!`, `warn!`, `info!`, `debug!` and `trace!`, and the application selects one implementation. If none is selected, the facade falls back to a logger that does nothing. The crate's `kv` feature adds structured key-value pairs.[^12] The example uses no crate and writes `key=value` pairs with `{:?}`, which quotes and escapes each string.

### JavaScript, TypeScript, C and C++

These languages have no logger with levels in the standard library. A logger is a few lines: a level check and a function that writes one escaped record. In JavaScript, `JSON.stringify` does the escaping. In C, the program must escape quotes and newlines itself. C++ `std::print` with `{:?}` writes an escaped string.

## Variations and related ideas

### The system log

On Unix systems, syslog collects the logs of many programs in one place. It started on BSD, and Eric Allman wrote the daemon and the protocol.[^16] Each message has a facility, which names the part of the system, and a severity from 0 to 7.[^17] On POSIX systems with the XSI option, a C program calls `syslog()`, and a shell script calls the `logger` utility. Both send a message to a logging facility that the implementation defines.[^18] [^19]

### Debug output and logging

A `print` added for one debug session is not a log. It has no level, and it is removed after the bug is fixed. A log record stays in the code, and the level decides when it appears. [[interactive-debugging]] stops a program to look at its state. A log is often the only view of a program that runs in production.

## Pitfalls

### Secrets and personal data in the log

A log goes to more places than the data it describes: files, backups, a search service, the screens of support staff. CWE-532 describes the weakness: "The product writes sensitive information to a log file."[^15] The OWASP Logging Cheat Sheet lists data that should not be recorded directly, but be removed, masked, hashed or encrypted: session ids, access tokens, passwords, database connection strings, encryption keys, payment card data and sensitive personal data.[^13] See [[sensitive-information-exposure]] and [[hard-coded-credentials]].

The safe place for redaction is the type of the value, not each call. In Go, a `Password` type with a `LogValue` method prints `REDACTED` in every record.[^3] A whole request or a whole user object in a log record often holds a secret that nobody meant to log.

### Log injection

A log that writes user text as it is lets the user write records. A newline in a user name ends the record, and the rest of the name becomes a forged record. CWE-117 names this weakness, also called log forging: the program "constructs a log message from external input, but it does not neutralize" the special elements in it.[^14] OWASP says: "Do not use unstructured text log formats", and recommends structured formats such as JSON.[^13] The Python and C examples show a forged record. The JSON and `{:?}` examples keep the same text inside one field.

### The record is dropped and nobody sees it

A level set too high hides the records that would explain a failure. In Python, the default level is WARNING, so `logging.info` prints nothing until the program sets a level.[^2] In Java, a `ConsoleHandler` has the level INFO by default, so it does not write FINE records.[^8]

### Work done for a dropped record

The arguments of a log call are computed before the level check. An f-string in Python or string interpolation in C# builds the whole message even when the record is dropped.[^2] [^11] Pass the values as arguments, check the level first, or use a type that computes its value only when the record is written, such as Go's `LogValuer`.[^2] [^3]

### Fields that a machine cannot read

A field must have one type and one format. `1,042` in one record and `1042` in the next break a query. In Java, `MessageFormat` formats numbers by the locale.[^9] An error object can also turn into nothing: `JSON.stringify` writes only enumerable properties, and the `message` of an `Error` is not one, so the record gets `{}`. Copy the name and the message of an error into fields.

### Timestamps in tests

A record usually has the time of the event. A test that compares log output must remove the time or set a fixed clock. The examples on this page remove it: Go with `ReplaceAttr`, the other languages with a formatter that does not write it.

## Cost

A log record costs a level check, the formatting of the record and a write. The level check is a comparison of two numbers. The formatting allocates strings. The write is a system call or a copy into a buffer. Go's documentation lists ways to reduce the cost: `Logger.With` formats common fields once, and a `LogValuer` defers expensive work until the record is enabled.[^3] Python's documentation recommends `isEnabledFor` before expensive arguments.[^2]

The larger cost is often storage. A server that writes a record for each request writes millions of records a day. Debug records are usually off in production for this reason.

## History

- **BSD syslog.** Eric Allman wrote the syslog daemon and protocol. The protocol started on the BSD TCP/IP system and was then ported to many other systems and devices.[^16]
- **2001.** RFC 3164 described the BSD syslog protocol as it was observed in use. It had a priority and free text, with no fields.[^16]
- **2009.** RFC 5424, by Rainer Gerhards, replaced RFC 3164. It defined the severities 0 to 7 and STRUCTURED-DATA, named parameters inside a syslog message.[^17]
- **2014.** Daniel Fuchs created JEP 264, the Platform Logging API and Service, which became `System.Logger` in Java.[^10]

## References
