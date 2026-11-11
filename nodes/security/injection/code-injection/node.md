---
tag: MSMSQV
kind: weakness
slug: code-injection
name: Code injection
aliases: [eval injection, dynamic code evaluation, server-side template injection, SSTI, cwe-94]
area: security/injection
level: core
summary: >-
  Code injection happens when a program builds code from outside input and then runs it, so the input can change what the code does.
cwe: 94
abstraction: base
languages_affected: [python, javascript, php, ruby, perl, powershell, bash, any-with-eval]
owasp_top10_2025: A05
cwe_top25_2025: 10
history:
  year: 2006
  people: []
  note: CWE-94 entered CWE in Draft 3 on 2006-07-19, from the PLOVER list of vulnerability types, with the name Code Injection.
same_as:
  wikidata: Q2920753
  wikipedia: Code_injection
sources:
  - url: https://cwe.mitre.org/data/definitions/94.html
    title: "CWE-94: Improper Control of Generation of Code"
    locator: Description; Relationships; Common Consequences; Potential Mitigations; Detection Methods; Observed Examples; Content History
  - url: https://top10.owasp.org/2025/A05_2025-Injection/
    title: "OWASP Top 10:2025, A05:2025 Injection"
    locator: List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 10, CVEs in KEV, Rank Last Year
  - url: https://docs.python.org/3/library/functions.html#eval
    title: The Python Standard Library, Built-in Functions, Python 3.15
    locator: eval; exec
  - url: https://docs.python.org/3/library/ast.html
    title: The Python Standard Library, ast, Python 3.15
    locator: ast.parse, the warning; ast.literal_eval, the warning
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/eval
    title: MDN Web Docs, eval()
    locator: Never use direct eval()!; Security considerations
  - url: https://nodejs.org/api/vm.html
    title: Node.js documentation, VM (executing JavaScript)
    locator: the warning at the top of the page
  - url: https://portswigger.net/research/server-side-template-injection
    title: "James Kettle, Server-Side Template Injection, PortSwigger Research, 5 August 2015"
    locator: the introduction; Detect
  - url: https://docs.python.org/3/library/string.html
    title: The Python Standard Library, string, Python 3.15
    locator: Format String Syntax; Template strings, safe_substitute
  - url: https://pkg.go.dev/html/template
    title: Go standard library, package html/template
    locator: Security Model
  - url: https://codeql.github.com/codeql-query-help/python/py-code-injection/
    title: CodeQL query help, Code injection (py/code-injection)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/suspicious-eval-usage/
    title: Ruff rules, suspicious-eval-usage (S307)
    locator: What it does
  - url: https://eslint.org/docs/latest/rules/no-eval
    title: ESLint rules, no-eval
    locator: the introduction; Rule Details
  - url: https://www.cve.org/CVERecord?id=CVE-2003-0395
    title: CVE-2003-0395, Ultimate PHP Board
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2008-5305
    title: CVE-2008-5305, TWiki
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2020-8218
    title: CVE-2020-8218, Pulse Connect Secure
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2022-26134
    title: CVE-2022-26134, Atlassian Confluence Server and Data Center
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2023-29374
    title: CVE-2023-29374, LangChain
    locator: the description and the publication date
---

## Description

Some languages can run code that the program makes at run time. Python has `eval` and `exec`. JavaScript has `eval` and the `Function` constructor. PHP, Ruby and Perl have `eval`, and shells and PowerShell have their own forms. These functions take text and run it with all the rights of the program. Code injection happens when part of that text comes from outside input. The program meant the input to be a value, such as a number in a formula. The interpreter reads it as code.[^1]

The root cause is the same as in [[sql-injection]] and [[os-command-injection]]: code and data share one string. The difference is the interpreter. Here it is the language of the program itself, so the input can do anything that the program can do. It can read and change data, read files, start processes and make network requests.[^1]

CWE-94 names this weakness. It is a base weakness. Its children include CWE-95, eval injection, and CWE-1336, improper neutralization of special elements used in a template engine.[^1] It is rank 10 of the 2025 CWE Top 25, up from rank 11 in 2024. The list counts 7 CVEs of this weakness in the Known Exploited Vulnerabilities catalog of CISA.[^3] In the OWASP Top 10:2025, it is part of A05:2025 Injection.[^2] Code injection through a deserializer is a separate weakness, [[deserialization-of-untrusted-data]].

### History

CWE-94 was one of the first CWE entries. It came from PLOVER, an earlier list of vulnerability types, in CWE Draft 3 in July 2006, with the name Code Injection.[^1] CWE renamed it four times. The fourth name, from October 2026, is Improper Control of Generation of Code.[^1]

### Why a restricted eval is not a sandbox

A common idea is to call `eval` with an empty set of names, so that the input cannot reach dangerous functions. The Python documentation says that overriding `__builtins__` is not a security mechanism, because the code can still access all builtins.[^4] The Node.js documentation says that the `node:vm` module is not a security mechanism, and that it must not run untrusted code.[^7] MDN says that `eval` runs code with the privileges of the caller. A direct call to `eval` can also read and change the local variables of the caller.[^6] A language runtime is too large to make safe by removing names. Do not try to filter the input either. A denylist of words such as `import` misses other spellings of the same code.

### Server-side template injection

A template engine is a small interpreter. It reads template text, and it runs the expressions inside it. When users can write the template text itself, they can write expressions too. This is server-side template injection, CWE-1336.[^1] James Kettle described it in 2015.[^8] His classic demonstration input is `{{7*7}}`: an engine that evaluates the expression shows `49` where the user expected the text.[^8] Engines that can call methods or reach the classes of the language can give the user the rights of the program.[^8]

Template engines with fewer features still leak data. In Python, a format string can read any attribute of its arguments with `.name`, and any item with `[index]`.[^9] Go templates can read the fields of the data and call its methods. The Go documentation says that the package assumes that template authors are trusted and the data is not.[^10] The Python and Go example files show both cases. The fix is the same in each engine. The template text comes from the program, and the user input is only data that the template prints.

## Vulnerable code

The function below is a calculator. It passes the formula that the user typed to `eval`. The classic demonstration input is a call to `__import__`, which shows that the input can load any module. Here it only asks the module `platform` for the name of the interpreter.

```python wrong id=eval
def calculate(expression):
    # The feature: a calculator for the formulas that users type.
    return eval(expression)


print(calculate("6 * 7"))
print(calculate("__import__('platform').python_implementation()"))
```

```output for=eval
42
CPython
```

The second call ran a function that the calculator never offered. The same input could call any function of any module, such as one that starts a process or deletes a file.

## Fixed code

The fix is to stop running input as code. The program parses the input with a grammar that has only the features it needs, and it evaluates the result itself.[^1] The code below parses the formula with `ast.parse` and walks the tree. It accepts numbers and four operators, and it refuses every other kind of node.

```python run id=ast-walk
import ast
import operator

OPERATORS = {
    ast.Add: operator.add,
    ast.Sub: operator.sub,
    ast.Mult: operator.mul,
    ast.Div: operator.truediv,
    ast.USub: operator.neg,
}


def evaluate(node):
    if isinstance(node, ast.Constant) and type(node.value) in (int, float):
        return node.value
    if isinstance(node, ast.BinOp) and type(node.op) in OPERATORS:
        return OPERATORS[type(node.op)](evaluate(node.left), evaluate(node.right))
    if isinstance(node, ast.UnaryOp) and type(node.op) in OPERATORS:
        return OPERATORS[type(node.op)](evaluate(node.operand))
    raise ValueError(f"not allowed: {type(getattr(node, 'op', node)).__name__}")


def calculate(expression):
    if len(expression) > 100:
        raise ValueError("expression too long")
    tree = ast.parse(expression, mode="eval")
    return evaluate(tree.body)


for text in ["6 * 7", "(1 + 2) / 4", "__import__('platform').python_implementation()", "2 ** 10"]:
    try:
        print(text, "=", calculate(text))
    except (ValueError, SyntaxError, ZeroDivisionError) as error:
        print(text, "->", error)
```

```output for=ast-walk
6 * 7 = 42
(1 + 2) / 4 = 0.75
__import__('platform').python_implementation() -> not allowed: Call
2 ** 10 -> not allowed: Pow
```

The tree of the demonstration input has a `Call` node, and the walk refuses it. The power operator is also refused, because a short input such as `9 ** 9 ** 9` takes a very long time. The length limit protects the parser: the documentation of `ast.parse` warns that a large or complex string can crash the interpreter.[^5]

### Allow-list the operations

The tree walk is an allow-list. It names the node types and operators that are valid, and everything else fails. This is the opposite of a filter that looks for dangerous words. When the calculator needs a function such as `sqrt`, add it to the allow-list by name.

### Prefer data formats and lookups

Many uses of `eval` do not need code at all. A program that reads a list or a dictionary from text can use `json.loads`. The Python documentation does not recommend `ast.literal_eval` for untrusted data, because a small input can exhaust memory or the C stack.[^5] A program that chooses a function from a name in the input can use a dictionary from names to functions. A program that reads a field by a name in the input can check the name against a fixed set.

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input to calls that run code. Its queries include `py/code-injection`, `js/code-injection` and `cs/code-injection`, and for Java the queries for SpEL, OGNL and other expression languages. The queries `py/template-injection` and `java/server-side-template-injection` find template text from input.[^11]
- **Pattern linters.** Ruff rule S307 flags `eval`, and S102 flags `exec`.[^12] ESLint rules `no-eval`, `no-implied-eval` and `no-new-func` flag `eval`, strings passed to `setTimeout` and `setInterval`, and the `Function` constructor.[^13] Treat every hit as a finding until a reviewer shows that the text is a constant.
- **Code review.** Search for `eval`, `exec`, `compile`, `Function(`, `Invoke-Expression`, and for template engines that receive text from a request or the database.
- **Tests.** Send `{{7*7}}` to inputs that end up in a template, and look for `49` in the output.[^8] Send a formula with a function call to inputs that reach a calculator. The feature must refuse it.

## How to prevent it

- **Do not run input as code.** Refactor the feature so that it does not generate code. CWE lists this first.[^1]
- **Parse a small grammar.** When users must write formulas or filters, parse them with a grammar that has only the needed operations. Evaluate the tree in the program.
- **Use data formats for data.** Use JSON and the parser of the format, not `eval`.
- **Keep template text in the program.** Pass user input to the template as data. When users must write templates, use a logic-less engine, or one with a sandbox that the vendor supports and patches.
- **Do not trust restricted builtins or `vm` contexts.** Their documentation says that they are not security mechanisms.[^4][^7]
- **Run with least privilege.** Run the process with few rights, in a sandbox or a container, so that injected code can do less.[^1]
- **Watch for language models.** Code that a language model writes from a prompt is outside input too. Do not pass it to `eval` or `exec` without a sandbox.[^18]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2003-0395 | published 2003-06-10 | Ultimate PHP Board 1.9 ran PHP code from the `User-Agent` header of a request. The code ran with administrator rights when the administrator opened `admin_iplog.php`.[^14] |
| CVE-2008-5305 | published 2008-12-10 | Eval injection in TWiki before 4.2.4 let remote attackers run Perl code through the `%SEARCH{}%` variable.[^15] |
| CVE-2020-8218 | published 2020-07-30 | Code injection in Pulse Connect Secure before 9.1R8 let an attacker run code through a crafted URI to the admin web interface.[^16] |
| CVE-2022-26134 | published 2022-06-03 | OGNL injection in Confluence Server and Data Center let an attacker with no login run code.[^17] |
| CVE-2023-29374 | published 2023-04-05 | In LangChain through 0.0.131, the `LLMMathChain` chain passed text from a prompt to the Python `exec` method. Prompt injection could run any code.[^18] |

## References
