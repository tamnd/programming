---
tag: NEW-os-command-injection
kind: weakness
slug: os-command-injection
name: OS command injection
aliases: [command injection, shell injection, cwe-78]
area: security/injection
level: core
summary: >-
  OS command injection happens when a program puts outside input into a command line that a shell parses, so the shell reads part of the input as more commands or options.
cwe: 78
abstraction: base
languages_affected: [any-that-starts-processes]
owasp_top10_2025: A05
cwe_top25_2025: 9
history:
  year: 1996
  people: []
  note: AUSCERT advisory AA96.01 and CERT/CC advisory CA-1996-06 warned that escape_shell_cmd in the CGI example code of NCSA httpd and Apache failed to stop shell commands, and that the phf program was actively exploited.
sources:
  - url: https://cwe.mitre.org/data/definitions/78.html
    title: "CWE-78: Improper Neutralization of Special Elements used in an OS Command ('OS Command Injection')"
    locator: Description; Technical Nuance; Alternate Terms; Common Consequences; Potential Mitigations; Observed Examples
  - url: https://top10.owasp.org/2025/A05_2025-Injection/
    title: "OWASP Top 10:2025, A05:2025 Injection"
    locator: List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 9, CVEs in KEV
  - url: https://www.sei.cmu.edu/documents/503/1996_019_001_496172.pdf
    title: "Software Engineering Institute, 1996 CERT Advisories, CA-1996-06: Vulnerability in NCSA/Apache CGI example code"
    locator: chapter 6, the introduction and 1. Description
  - url: https://www.kb.cert.org/vuls/id/20276
    title: "CERT/CC Vulnerability Note VU#20276, phf CGI Script fails to guard against newline characters"
    locator: Overview; Description
  - url: https://cheatsheetseries.owasp.org/cheatsheets/OS_Command_Injection_Defense_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, OS Command Injection Defense Cheat Sheet
    locator: Argument Injection; Primary Defenses, options 1 to 3; Additional Defenses; Code examples, Java
  - url: https://docs.python.org/3.14/library/subprocess.html#security-considerations
    title: The Python Standard Library, subprocess, Python 3.14
    locator: Security Considerations
  - url: https://pkg.go.dev/os/exec
    title: Go standard library, package os/exec
    locator: Overview
  - url: https://nodejs.org/api/child_process.html
    title: Node.js documentation, Child process
    locator: child_process.exec; child_process.execFile
  - url: https://blog.rust-lang.org/2024/04/09/cve-2024-24576/
    title: "Rust Security Response WG, Security advisory for the standard library (CVE-2024-24576)"
    locator: Overview; Mitigations
  - url: https://codeql.github.com/codeql-query-help/python/py-command-line-injection/
    title: CodeQL query help, Uncontrolled command line (py/command-line-injection)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/subprocess-popen-with-shell-equals-true/
    title: Ruff rules, subprocess-popen-with-shell-equals-true (S602)
    locator: What it does
  - url: https://github.com/securego/gosec/blob/master/RULES.md
    title: gosec rules
    locator: G204 and G702
  - url: https://www.shellcheck.net/wiki/SC2086
    title: "ShellCheck wiki, SC2086: Double quote to prevent globbing and word splitting"
    locator: Problematic code; Correct code
  - url: https://www.cve.org/CVERecord?id=CVE-1999-0067
    title: CVE-1999-0067, phf CGI program
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2014-6271
    title: CVE-2014-6271, GNU Bash
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-3400
    title: CVE-2024-3400, Palo Alto Networks PAN-OS GlobalProtect
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-24576
    title: CVE-2024-24576, Rust standard library
    locator: the description and the publication date
---

## Description

A program that needs another program can start it in two ways. It can give the operating system the path of the program and a list of arguments. Or it can give a shell one line of text and let the shell parse it. The shell language has its own syntax: `;`, `&&`, `|` and a newline end a command, `$( )` and backquotes run a command, `>` writes a file, and spaces split arguments. OS command injection happens when the program builds the line from fixed text and input from outside. When the input holds one of these characters, the shell reads it as syntax. The program meant one command with one argument. The shell runs what the line says.

The root cause is again that code and data share one channel. The developer knows that the input is one argument, but the string does not keep this knowledge. The fix is to keep the list of arguments as a list all the way to the operating system (see Fixed code).

The new command runs with the rights of the program, so the attacker can read or change any data that the program can, or stop the system. The commands also appear to come from the program and its owner.[^1] On a web server, this often means control of the server.

CWE-78 names this weakness. It is a base weakness under CWE-77, command injection, and its alternate terms are shell injection and shell metacharacters.[^1] CWE-78 describes two forms. In the first, the program runs a fixed program and the input is an argument, as in `system("nslookup [HOSTNAME]")`. In the second, the input selects the program or the whole command.[^1] It is rank 9 of the 2025 CWE Top 25, and with 20 CVEs in the CISA catalogue of known exploited vulnerabilities, it has more of them than any other weakness on the list.[^3] In the OWASP Top 10:2025 it is part of A05:2025 Injection.[^2]

### History

On 14 March 1996, AUSCERT published advisory AA96.01 about the example CGI code of the NCSA and Apache web servers, and CERT/CC reprinted it on 20 March as CA-1996-06.[^4] The code had a function, `escape_shell_cmd`, that tried to make input safe for `system` and `popen`. The advisory said that any program that relied on it could be attacked, and that the phf program, which came with both servers, was being actively exploited.[^4] A later CERT/CC note explains the bug. phf built a command line with `strcat` and passed it to `popen`. `escape_shell_cmd` put a backslash before `&`, `;`, `|`, `$` and other characters, but not before a newline, and a newline also ends a shell command.[^5] The note says that phf was widely exploited in 1996 and 1997.[^5] The lesson is that a list of dangerous characters is never complete.

### Argument injection

An argument list stops a second command. It does not stop a value from becoming an option of the program. A value that starts with `-` can turn on a feature that the program did not mean to use. The OWASP cheat sheet shows `curl` with the value `--help`.[^6] CWE-88 names this weakness. For commands that support it, put `--` before the values to end option parsing, and check that a value does not start with `-`.[^6]

## Vulnerable code

The function below builds a shell command line with `+`. The second call uses the classic demonstration input `report.txt; echo INJECTED`. The `;` ends the first command, and the shell runs `echo INJECTED` as a second command.

```python wrong id=shell-true
import subprocess


def look_up(name):
    subprocess.run("echo looking up " + name, shell=True)


look_up("report.txt")
look_up("report.txt; echo INJECTED")
```

```output for=shell-true
looking up report.txt
looking up report.txt
INJECTED
```

The same weakness appears with `os.system`, `system` and `popen` in C, `exec` in Node.js, `sh -c` with a built string in any language, and `eval` in a shell script. The example files show it in each language.

## Fixed code

The program passes a list. The first item names the program, and each other item is one argument. No shell starts, so no character in an argument has a meaning.[^7]

```python run id=argv
import subprocess


def look_up(name):
    subprocess.run(["echo", "looking up", name])


look_up("report.txt")
look_up("report.txt; echo INJECTED")
```

```output for=argv
looking up report.txt
looking up report.txt; echo INJECTED
```

The input is now one argument, and `echo` prints it. Go's `os/exec` "intentionally does not invoke the system shell".[^8] `execFile` in Node.js does not spawn a shell by default.[^9] `ProcessBuilder` in Java, `Command` in Rust, `ArgumentList` in .NET and `posix_spawnp` in C work in the same way. The example files show each one.

### Why escaping is not enough

When a shell is necessary, the program must quote each value for that shell. `shlex.quote` in Python puts a value in single quotes for a POSIX shell, and the Python documentation says that this works only on some platforms.[^7] The rules differ between shells, and `cmd.exe` on Windows has its own. The phf bug shows what happens when one character is missing from the list.[^5] CVE-2024-24576 shows that the problem is hard even for a standard library. On Windows, a process gets one command line and splits it itself, and `cmd.exe` splits the arguments of a batch file with its own rules. Rust's `Command` did not escape arguments to `.bat` and `.cmd` files correctly, and the fix makes it return an `InvalidInput` error when it cannot escape an argument safely.[^10][^18]

### Avoid the command

The OWASP cheat sheet puts first the defence of not calling an operating system command at all.[^6] A library call cannot run a second command. Use `mkdir` from the standard library and not `system("mkdir ...")`. Copy, move and delete files, make archives and send HTTP requests with library functions too. CWE-78 gives the same advice.[^1]

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input to a process call. Its queries are `py/command-line-injection`, `js/command-line-injection`, `java/command-line-injection`, `go/command-injection` and `cs/command-line-injection`.[^11]
- **Pattern linters.** Ruff rule S602 reports `subprocess` calls with `shell=True`.[^12] gosec rule G204 reports `exec.Command` with arguments that are not constants, and G702 follows input to it.[^13] ShellCheck rule SC2086 reports unquoted variables in shell scripts, which split into many arguments.[^14]
- **Code review.** Search for `system`, `popen`, `shell=True`, `exec(`, `sh -c`, `cmd /c`, `eval` and `Invoke-Expression`. For each call, check if any part of the string comes from input.
- **Tests.** Put a value with `;`, a space, a quote and a leading `-` through each input that reaches a command. The command must get it as one argument, as it is.

## How to prevent it

- **Use a library call in place of a command.** This removes the weakness.[^6]
- **Pass an argument list and start no shell.** Use the process API that takes the program and the arguments apart.[^1][^6]
- **Choose the program from a fixed set.** Never let input name the program or the subcommand.[^6]
- **End option parsing.** Put `--` before values for commands that support it, and reject values that start with `-`.[^6]
- **Validate input as a second layer.** Check each argument against an allow-list or a strict pattern, such as `^[a-z0-9]{3,10}$`.[^6] Do not use a denylist of shell characters. It will miss one.[^1][^5]
- **Use least privilege.** Run the program with only the rights it needs, and in a separate account for each task where possible.[^6] A sandbox such as a chroot jail, AppArmor or SELinux limits the damage.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CA-1996-06 | 1996-03-20 | The phf example program of NCSA httpd and Apache passed input to `popen` after an escape function that missed the newline character. It was actively exploited.[^4][^5] |
| CVE-1999-0067 | published 1999-09-29 | The CVE record for phf: remote command execution through shell metacharacters. CWE-78 calls it the canonical example.[^15][^1] |
| CVE-2014-6271 | published 2014-09-24 | Shellshock. GNU Bash ran the trailing strings after a function definition in the value of an environment variable. The CVE record names vectors where the environment is set across a privilege boundary, such as the `mod_cgi` module of the Apache HTTP Server and the `ForceCommand` feature of OpenSSH.[^16] |
| CVE-2024-3400 | published 2024-04-12 | Command injection in the GlobalProtect feature of Palo Alto Networks PAN-OS let an unauthenticated attacker run code with root rights on the firewall.[^17] |
| CVE-2024-24576 | published 2024-04-09 | Rust's `Command` did not escape arguments correctly for batch files on Windows, so an attacker who controlled an argument could run shell commands.[^18] |

## References
