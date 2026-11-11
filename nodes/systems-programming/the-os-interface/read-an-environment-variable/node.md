---
tag: T0ZXFQ
kind: task
slug: read-an-environment-variable
name: Read an environment variable
aliases: [get an environment variable, read an env var with a default, getenv]
area: systems-programming/the-os-interface
level: core
summary: Read one environment variable of the process, return the value also when it is empty, and return a default only when the variable is not set.
contract: contract.yaml
category: processes-and-the-os
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap08.html
    title: The Open Group, POSIX.1-2024, Base Definitions, Chapter 8, Environment Variables
    locator: 8.1 Environment Variable Definition
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/getenv.html
    title: The Open Group, POSIX.1-2024, getenv, secure_getenv
    locator: Description, Return Value, Rationale, Change History
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/setenv.html
    title: The Open Group, POSIX.1-2024, setenv
    locator: Description, Errors
  - url: https://docs.python.org/3/library/os.html#os.environ
    title: The Python Standard Library, os, Miscellaneous operating system interfaces
    locator: os.environ, os.environb, os.reload_environ, os.getenv, os.putenv
  - url: https://docs.python.org/3/library/stdtypes.html#truth-value-testing
    title: The Python Standard Library, Built-in Types
    locator: Truth Value Testing
  - url: https://nodejs.org/api/process.html#processenv
    title: Node.js documentation, Process
    locator: process.env
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Logical_OR
    title: MDN Web Docs, Logical OR (||)
    locator: Description
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Nullish_coalescing
    title: MDN Web Docs, Nullish coalescing operator (??)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/System.html#getenv(java.lang.String)
    title: Java SE 25 API, java.lang.System
    locator: getenv(String), getenv()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.getenvironmentvariable?view=net-10.0
    title: .NET API reference, Environment.GetEnvironmentVariable
    locator: GetEnvironmentVariable(String), Returns, Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.environment.setenvironmentvariable?view=net-10.0
    title: .NET API reference, Environment.SetEnvironmentVariable
    locator: SetEnvironmentVariable(String, String), Remarks
  - url: https://pkg.go.dev/os#LookupEnv
    title: Go standard library, package os
    locator: Getenv, LookupEnv
  - url: https://go.dev/ref/spec#String_types
    title: The Go Programming Language Specification
    locator: String types
  - url: https://doc.rust-lang.org/std/env/fn.var.html
    title: The Rust Standard Library, std::env::var
    locator: Errors
  - url: https://doc.rust-lang.org/std/env/fn.var_os.html
    title: The Rust Standard Library, std::env::var_os
  - url: https://doc.rust-lang.org/std/env/fn.set_var.html
    title: The Rust Standard Library, std::env::set_var
    locator: Safety
  - url: https://en.cppreference.com/w/cpp/utility/program/getenv.html
    title: cppreference.com, std::getenv
  - url: https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-Expansion
    title: GNU Bash Reference Manual, Shell Parameter Expansion
    locator: 3.5.3, indirect expansion, ${parameter:-word}
  - url: https://www.gnu.org/software/bash/manual/bash.html#Environment
    title: GNU Bash Reference Manual, Environment
    locator: 3.7.4
---

## Task

Read the value of one environment variable of the process. If the variable is not set, use a default value.

Each solution is a function `get_env(name, default)` that returns a string. It returns the value when the variable is set. It also returns the value when the value is the empty string. It returns `default` only when the variable is not set.

The rules are those of POSIX. An environment variable is a string `name=value`, and a name has no `=`. Upper and lower case letters stay different, so `APP_PORT` and `app_port` are two variables. A value can hold any byte except NUL. The cases use UTF-8 values only.

A process gets its environment when it starts. A child process gets a copy of the environment of its parent, so a change in the child does not change the parent.

The tests control the input. Each test sets or removes the variables in its own process before the call. In Java, `System.getenv()` returns an unmodifiable map, so the Java test starts a child JVM with the environment of each case.

## Solutions

Each solution asks the environment for one name, and tells "not set" apart from "set to the empty string".

- **Python.** `os.environ.get(name, default)`. `os.getenv(name, default)` gives the same result. Python copies the environment into the `os.environ` mapping when it imports `os`. Assignment to `os.environ` changes both the mapping and the environment.
- **JavaScript and TypeScript (Node.js).** `process.env` is an object that holds the environment, and a missing variable gives `undefined`. The solution tests the name with `Object.hasOwn(process.env, name)`, because `process.env` also inherits properties such as `toString`. The TypeScript file adds types only.
- **Java.** `System.getenv(name)` returns `null` when the variable is not set, and `Objects.requireNonNullElse` puts the default in its place. `System.getenv()` returns a map of all the variables.
- **C#.** `Environment.GetEnvironmentVariable(name)` returns `null` when the variable is not found, and `??` gives the default.
- **Go.** `os.LookupEnv(name)` returns the value and a boolean that is true when the variable is present. `os.Getenv` returns `""` when the variable is not present, so it cannot tell missing from empty.
- **Rust.** `env::var(name)` returns `Err(VarError::NotPresent)` when the variable is not set, and `Err(VarError::NotUnicode)` when the value is not valid Unicode. The solution changes only `NotPresent` into the default. `env::var_os` returns an `Option<OsString>` and does not check the value.
- **C.** `getenv(name)` returns a null pointer when the variable is not found. The string belongs to the environment, so do not change it. A later `setenv`, `unsetenv` or `putenv` can overwrite or free it, so copy the value before you change the environment.
- **C++.** `std::getenv` from `<cstdlib>`, with the same rules. The solution copies the value into a `std::string` at once.
- **Bash.** `${!name-$default}`. The `!` reads the variable whose name is in `name`. The `-` without a colon gives the default only when the variable is unset. At startup Bash makes a shell variable from each environment variable, so this also sees shell variables that are not exported. With a fixed name, write `${APP_PORT-80}`.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each case gives the variables that the test sets (`env`), the name and the default.

- `missing`: no variable, so the result is the default.
- `empty-value`: `APP_MODE` is set to `""`. The result is `""`, not the default.
- `empty-default`: the default can also be the empty string.
- `spaces`, `equals` and `newline`: the value comes back with no change. A name has no `=`, so each `=` after the name is part of the value.
- `non-ascii`: a UTF-8 value with characters of 2, 3 and 4 bytes.
- `case`: `APP_PORT` is set, and the call asks for `app_port`. On POSIX the result is the default. The .NET and Node.js documentation says that on Windows names are not case-sensitive. The Java documentation says that on Windows case is usually not significant. Python on Windows changes the keys of `os.environ` to upper case.
- `inherited-name`: the name `toString` is not set. In JavaScript, `process.env[name] ?? defaultValue` returns a function for this name.
- Values that are not UTF-8: JSON cannot hold such bytes, so no case file has them. Rust `env::var` gives `NotUnicode`. On Unix, Python decodes the bytes with the `surrogateescape` error handler. A Go string holds bytes, so Go returns them with no change. The Rust, Python, Go, C and C++ tests check this.

## Pitfalls

- **An empty value that counts as missing.** Some code uses a second rule: an empty value also gives the default. Bash `${VAR:-default}` with a colon follows this rule:

```bash run id=bash-colon
export APP_MODE=
echo "with colon: [${APP_MODE:-production}]"
echo "without colon: [${APP_MODE-production}]"
```

```output
with colon: [production]
without colon: []
```

The JavaScript `||` operator also follows it, because `""` is falsy. `??` replaces only `null` and `undefined`. The string `"0"` is not falsy, so `||` keeps it. The last line shows the inherited `toString`:

```javascript run id=js-or
process.env.APP_MODE = "";
process.env.APP_RETRIES = "0";
console.log(JSON.stringify(process.env.APP_MODE || "production"));
console.log(JSON.stringify(process.env.APP_MODE ?? "production"));
console.log(JSON.stringify(process.env.APP_RETRIES || "3"));
console.log(typeof process.env.toString);
```

```output
"production"
""
"0"
function
```

In Python, `os.environ.get(name) or default` follows the second rule, because `""` is false. The second rule is correct when an empty value has no use, for example a port number. It is wrong when the empty string is a valid setting, for example an empty prefix. Choose one rule for each variable and write it down.

- **`os.Getenv` in Go.** It returns `""` for an empty value and for a missing variable:

```go run id=go-getenv
package main

import (
	"fmt"
	"os"
)

func main() {
	os.Setenv("APP_MODE", "")
	os.Unsetenv("APP_TOKEN")
	fmt.Printf("Getenv: %q %q\n", os.Getenv("APP_MODE"), os.Getenv("APP_TOKEN"))
	_, modeSet := os.LookupEnv("APP_MODE")
	_, tokenSet := os.LookupEnv("APP_TOKEN")
	fmt.Println("LookupEnv:", modeSet, tokenSet)
}
```

```output
Getenv: "" ""
LookupEnv: true false
```

- **A change in a child process.** The child changes its own copy of the environment. The parent does not see the change:

```bash run id=bash-child
unset APP_MODE
bash -c 'export APP_MODE=debug; echo "child: $APP_MODE"'
echo "parent: ${APP_MODE-not set}"
```

```output
child: debug
parent: not set
```

- **A stale copy in Python.** `os.putenv` changes the environment of the process, but not `os.environ`. `os.reload_environ` reads the environment again. Assign to `os.environ` instead:

```python run id=python-putenv
import os

os.environ.pop("APP_MODE", None)
os.putenv("APP_MODE", "debug")
print(os.environ.get("APP_MODE"))
os.environ["APP_MODE"] = "debug"
print(os.environ.get("APP_MODE"))
```

```output
None
debug
```

.NET has a similar gap: a change that a native library makes is not seen by managed code.

- **A value that is not UTF-8 in Rust.** `env::var` gives `NotUnicode`, not `NotPresent`. So `env::var(name).unwrap_or(default)` hides a bad value behind the default:

```rust run id=rust-not-unicode
use std::env;
use std::ffi::OsStr;
use std::os::unix::ffi::OsStrExt;

fn main() {
    // SAFETY: this program has one thread.
    unsafe { env::set_var("APP_NAME", OsStr::from_bytes(b"caf\xe9")) };
    println!("{:?}", env::var("APP_NAME"));
    println!("{:?}", env::var("APP_NAME").unwrap_or("guest".to_string()));
    println!("{:?}", env::var_os("APP_NAME"));
}
```

```output
Err(NotUnicode("caf\xE9"))
"guest"
Some("caf\xE9")
```

- **Changes from other threads.** POSIX does not require `setenv` to be thread-safe. `getenv` is thread-safe only while no other thread changes the environment. For this reason, Rust `set_var` and `remove_var` are `unsafe`. Read the variables once, before the program starts threads, and pass the values on. To give a variable to a child process, set it on the child, for example with Rust `Command::env`.

## Related tasks

- [[parse-command-line-arguments]]: the other input that a program gets when it starts.
- [[split-a-string]]: split a value such as a list of paths into its parts.

## References
