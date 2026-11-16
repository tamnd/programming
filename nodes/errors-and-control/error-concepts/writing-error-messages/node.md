---
tag: 3QKH8F
kind: practice
slug: writing-error-messages
name: Writing error messages
aliases: [error message guidelines, good error messages, error message style, helpful error messages]
area: errors-and-control/error-concepts
level: core
summary: >-
  Write each error message so that its reader learns what went wrong, where it went wrong and how to fix it, in words that this reader knows.
see_also: [log-an-error, wrap-an-error-with-context, define-a-custom-error-type, error-handling-strategies]
sources:
- url: https://developers.google.com/tech-writing/error-messages
  title: Writing Helpful Error Messages, Technical Writing, Google for Developers
  locator: the introduction
- url: https://learn.microsoft.com/en-us/windows/win32/uxguide/mess-error
  title: Error Messages, Windows design guidelines, Microsoft Learn
  locator: Design concepts
- url: https://developers.google.com/tech-writing/error-messages/identify-the-cause
  title: Identify the cause, Writing Helpful Error Messages, Google for Developers
  locator: the whole page
- url: https://developers.google.com/tech-writing/error-messages/invalid-inputs
  title: Identify the user's invalid inputs, Writing Helpful Error Messages, Google for Developers
  locator: the whole page
- url: https://www.gnu.org/prep/standards/html_node/Errors.html
  title: GNU Coding Standards, 4.4 Formatting Error Messages
  locator: the whole section
- url: https://developers.google.com/tech-writing/error-messages/show-fix
  title: Explain how to fix the problem, Writing Helpful Error Messages, Google for Developers
  locator: the whole page
- url: https://developers.google.com/tech-writing/error-messages/target-audience
  title: Write for the target audience, Writing Helpful Error Messages, Google for Developers
  locator: the whole page
- url: https://www.nngroup.com/articles/error-message-guidelines/
  title: Tim Neusesser and Evan Sunwall, Error-Message Guidelines, Nielsen Norman Group, 14 May 2023
  locator: Communication Guidelines
- url: https://cheatsheetseries.owasp.org/cheatsheets/Error_Handling_Cheat_Sheet.html
  title: Error Handling Cheat Sheet, OWASP Cheat Sheet Series
  locator: Objective
- url: https://rustc-dev-guide.rust-lang.org/diagnostics.html
  title: Errors and lints, Rust Compiler Development Guide
  locator: Diagnostic output style guide
- url: https://go.dev/wiki/CodeReviewComments
  title: Go Code Review Comments, The Go Wiki
  locator: Error Strings
- url: https://doc.rust-lang.org/std/error/trait.Error.html
  title: std::error::Error, Rust standard library
  locator: the description
---

## Statement

The Google course on error messages puts the aim in one sentence: "Great error messages answer two questions as clearly and concisely as possible: What went wrong? How does the user fix that problem?"[^1] The Windows guidelines split the same aim into three parts. Good error messages have "A problem. States that a problem occurred. A cause. Explains why the problem occurred. A solution. Provides a solution so that users can fix the problem."[^2]

## Meaning

**Say what went wrong.** "Tell users exactly what went wrong."[^3] A message such as `Invalid port.` names the problem but not the value. "If the error involves values that the user can enter or modify (for example, text, settings, command-line parameters), then the error message should identify the offending value(s)."[^4] Show the bad value and the rule that it breaks. The Google course gives this example: "The postal code for the US must consist of either five or nine digits. The specified postal code (4872953) contained seven digits."[^4]

**Say where.** A message about a file names the file and the line. The GNU Coding Standards give the form "sourcefile:lineno: message" for compilers, and "program:sourcefile:lineno: message" for other programs that are not interactive[^5]. Editors and other tools can then find the place. [[wrap-an-error-with-context]] shows how each level of a program adds its part of the place.

**Say how to fix it.** "Create actionable error messages. That is, after explaining the cause of the problem, explain how to fix the problem."[^6] When the fix is to use a different value, give the range or an example of a good value.

**Write for the reader.** "Tailor the error message to the target audience."[^7] A user of an app and a developer who reads a log need different words. The Nielsen Norman Group says: "Avoid technical jargon and use language familiar to your users instead", and "Hide or minimize the use of obscure error codes or abbreviations; show them for technical diagnostic purposes only."[^8] So a program often has two messages for one error: a short one for the user, and a full one with the cause for the log. The OWASP cheat sheet asks for this split in web apps: "a generic response is returned by the application but the error details are logged server side for investigation, and not returned to the user"[^9]. [[log-an-error]] writes the full message to the log.

**Do not blame the reader.** "Don't use phrasing that blames users or implies they are doing something wrong, such as invalid, illegal, or incorrect."[^8] The Rust compiler guide says the same in a short way: "The word “illegal” is illegal."[^10]

**Follow the style of the language.** A message inside a program is often joined to other messages. The Go guide says: "Error strings should not be capitalized (unless beginning with proper nouns or acronyms) or end with punctuation, since they are usually printed following other context."[^11] The Rust documentation says: "Error messages are typically concise lowercase sentences without trailing punctuation"[^12]. The GNU rule has the same reason: the message "should not begin with a capital letter when it follows a program name and/or file name, because that isn’t the beginning of a sentence"[^5].

## Examples

The first function gives a message that only names the problem. The second gives the place, the bad value, the rule and a fix:

```python run id=port-messages
def check_port_vague(text):
    if not text.isdigit() or not 1 <= int(text) <= 65535:
        raise ValueError("Invalid port.")
    return int(text)


def check_port(where, text):
    if not (text.isascii() and text.isdigit()):
        raise ValueError(f"{where}: port {text!r} is not a number; use a whole number from 1 to 65535, such as 8080")
    port = int(text)
    if not 1 <= port <= 65535:
        raise ValueError(f"{where}: port {port} is out of range; use a number from 1 to 65535")
    return port


for check in (lambda: check_port_vague("80a"), lambda: check_port("app.conf:3", "80a"), lambda: check_port("app.conf:7", "70000")):
    try:
        check()
    except ValueError as e:
        print(e)
```

```output for=port-messages
Invalid port.
app.conf:3: port '80a' is not a number; use a whole number from 1 to 65535, such as 8080
app.conf:7: port 70000 is out of range; use a number from 1 to 65535
```

In Go, each level adds its context in front of the message of the level below. A message with a capital letter and a period then looks wrong in the middle of the line:

```go run id=message-in-a-chain
package main

import (
	"errors"
	"fmt"
)

func main() {
	good := fmt.Errorf("app.conf:3: %w", errors.New("port \"80a\" is not a number"))
	bad := fmt.Errorf("app.conf:3: %w", errors.New("Port is not a number."))
	fmt.Println("server:", good)
	fmt.Println("server:", bad)
}
```

```output for=message-in-a-chain
server: app.conf:3: port "80a" is not a number
server: app.conf:3: Port is not a number.
```

## Criticism and limits

- **Words cannot fix bad error handling.** The Windows guidelines say: "Good error messages aren't just a UI problem, they are a software design problem."[^2] A program that does not know the cause of an error cannot report it. Errors that keep their cause, as in [[define-a-custom-error-type]] and [[wrap-an-error-with-context]], make good messages possible.
- **Short and complete pull in different directions.** The Rust compiler guide says that "Error messages should be succinct", and adds: "That said, don’t make it so terse that it’s hard to understand."[^10] The Windows guidelines ask for a message that "is as short as possible, but no shorter"[^2]. The Rust compiler puts the long text behind the `--explain` flag[^10].
- **The style guides do not agree on capitals.** Go and Rust ask for a lowercase message with no period[^11][^12]. The GNU standards ask for the same after a program name or a file name, but "Error messages from interactive programs, and other messages such as usage messages, should start with a capital letter"[^5]. The difference comes from where the message appears. A message that is joined to other text starts in lowercase, and a message that starts the line starts with a capital letter.
- **Detail can help an attacker.** The detail that helps a developer, such as a stack trace or a query, can also show the inside of a system. The OWASP cheat sheet keeps the details in the server log and sends a generic response[^9].

## References
