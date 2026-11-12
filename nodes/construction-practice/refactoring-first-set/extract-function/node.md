---
tag: EKDK97
kind: technique
slug: extract-function
name: Extract function
aliases: [Extract Method]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Move a fragment of code into a function of its own, named for what the fragment does, and replace the fragment with a call. The behaviour of the program does not change.
family: refactoring
polarity: positive
intent: "Give a fragment of code a name that says what it does, so that the reader can skip how it does it."
also_known_as: [Extract Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) named it Extract Method. The second edition (2018) renamed it Extract Function. Its inverse is Inline Function.
see_also: [unit-testing]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Extract Function (page 106), motivation and mechanics
  - url: https://refactoring.com/catalog/extractFunction.html
    title: Martin Fowler, Catalog of Refactorings, Extract Function
    locator: the entry; formerly Extract Method; inverse of Inline Function
  - url: https://martinfowler.com/bliki/FunctionLength.html
    title: Martin Fowler, Function Length, bliki, 30 November 2016
    locator: intention and implementation; the highlight and reverse example; the cost of a function call
  - url: https://www.jetbrains.com/help/idea/extract-method.html
    title: IntelliJ IDEA documentation, Extract method
    locator: the refactoring and its options
  - url: https://www.jetbrains.com/help/pycharm/extract-method.html
    title: PyCharm documentation, Extract method
    locator: the refactoring
  - url: https://learn.microsoft.com/en-us/visualstudio/ide/reference/extract-method
    title: Visual Studio documentation, Extract method refactoring
    locator: the refactoring for C# and Visual Basic
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: Extract to method or function
  - url: https://go.dev/gopls/features/transformation
    title: gopls documentation, Code transformation features
    locator: the code action kinds refactor.extract.function and refactor.extract.method
  - url: https://rust-analyzer.github.io/book/assists.html
    title: rust-analyzer manual, Assists
    locator: extract_function
  - url: https://github.com/llvm/llvm-project/blob/llvmorg-21.1.0/clang-tools-extra/clangd/refactor/tweaks/ExtractFunction.cpp#L9-L30
    title: LLVM project source at llvmorg-21.1.0, clangd tweak ExtractFunction
    locator: the file header comment, lines 9 to 30
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Extract Method
---

## Purpose

A long function mixes what it does with how it does it. The reader must run the code in the head to learn its purpose. Extract function moves a fragment into a function of its own and gives that function a name that states the purpose. The original function then reads as a list of steps, and the reader opens a step only when the detail matters.

Fowler gives this as his reason to extract: the separation between intention and implementation. When a reader needs effort to see what a fragment does, the fragment should become a function named for that "what".[^3] The rule does not depend on length. His example is a Smalltalk method `highlight`, whose body was only a call to `reverse`: the name was longer than the body, and that was the point.[^3] A comment above a fragment that says what it does is a common sign that the fragment wants a name.[^1]

The refactoring changes the structure and keeps the behaviour. Tests that pass before the change must pass after it. Its inverse is Inline Function.[^2] The first edition of the book called the same refactoring Extract Method.[^11]

## Steps

1. Run the tests and see them pass. Without tests that cover the fragment, the steps below have no check.
2. Choose the fragment, and choose a name that says what it does, not how it does it.
3. Make a function with that name, and copy the fragment into its body.
4. Look at each variable that the fragment uses and that the source function declares. A variable that the fragment only reads becomes a parameter. A variable that the fragment declares and uses only inside itself moves into the function. A variable that the fragment assigns and the source function reads later becomes the return value.
5. Compile, or run the type checker or linter, to find the names that the copy misses.
6. Replace the fragment in the source function with a call, and pass the parameters and use the return value.
7. Run the tests again.
8. Look for other copies of the same fragment, and replace them with calls to the function.

When step 4 finds more than one assigned variable, the fragment is often the wrong size. Choose a smaller fragment, or return a small record of the values.

## Example

The function `invoice` builds the text of an invoice. A comment marks the part that computes the total, and the expression that formats cents appears twice:

```python run id=before
def invoice(customer, items):
    lines = [f"Invoice for {customer}"]
    for name, price, quantity in items:
        lines.append(f"  {name}: {quantity} x {price // 100}.{price % 100:02d}")
    # compute the total
    total = 0
    for _, price, quantity in items:
        total += price * quantity
    lines.append(f"Total: {total // 100}.{total % 100:02d}")
    return "\n".join(lines)


def test_invoice():
    items = [("pen", 250, 2), ("pad", 1205, 1)]
    assert invoice("Ada", items) == (
        "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05"
    )
    assert invoice("Bob", []) == "Invoice for Bob\nTotal: 0.00"


test_invoice()
print("tests pass")
print(invoice("Ada", [("pen", 250, 2), ("pad", 1205, 1)]))
```

```output for=before
tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

Two extractions follow. The block under the comment reads `items` and assigns `total`, which the function reads later, so `items` becomes the parameter and `total` becomes the return value. The function is named for the comment, and the comment goes away. The formatting expression reads one value and has no assignments, so it becomes `format_cents(cents)`, and both copies become calls:

```python run id=after
def format_cents(cents):
    return f"{cents // 100}.{cents % 100:02d}"


def total_cents(items):
    return sum(price * quantity for _, price, quantity in items)


def invoice(customer, items):
    lines = [f"Invoice for {customer}"]
    for name, price, quantity in items:
        lines.append(f"  {name}: {quantity} x {format_cents(price)}")
    lines.append(f"Total: {format_cents(total_cents(items))}")
    return "\n".join(lines)


def test_invoice():
    items = [("pen", 250, 2), ("pad", 1205, 1)]
    assert invoice("Ada", items) == (
        "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05"
    )
    assert invoice("Bob", []) == "Invoice for Bob\nTotal: 0.00"


def test_total_cents():
    assert total_cents([]) == 0
    assert total_cents([("pen", 250, 2)]) == 500


test_invoice()
test_total_cents()
print("tests pass")
print(invoice("Ada", [("pen", 250, 2), ("pad", 1205, 1)]))
```

```output for=after
tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

The test of `invoice` is the same, and it still passes. The output is the same. The extracted function `total_cents` can now have a test of its own.

## When to use

- A reader needs effort to see what a fragment does, or a comment explains what a fragment does.
- The same fragment appears in more than one place.
- A function does several steps at different levels of detail, such as a computation, formatting and output in one body.
- Another refactoring needs a smaller function first, for example to move behaviour to another class or to replace a conditional.

Do not extract when no name says more than the code does. A function `add_one(x)` with the body `x + 1` adds a jump and no meaning. Fowler notes that short functions work only when the names are good.[^3]

## Pitfalls

**A lost assignment.** If the fragment assigns a local variable that the source function reads later, the extracted function must return the value. In Python, Java, JavaScript and Go, an assignment to a parameter changes the parameter only, not the variable of the caller:

```python wrong id=lost-update
def summary(items):
    count = 0
    total = 0
    for _, price, quantity in items:
        count += quantity
        total += price * quantity
    return f"{count} items, {total} cents"


def add_up(items, count, total):  # extracted by hand, with a mistake
    for _, price, quantity in items:
        count += quantity
        total += price * quantity


def summary_after(items):
    count = 0
    total = 0
    add_up(items, count, total)
    return f"{count} items, {total} cents"


items = [("pen", 250, 2), ("pad", 1205, 1)]
print("before:", summary(items))
print("after: ", summary_after(items))
```

```output for=lost-update
before: 3 items, 1705 cents
after:  0 items, 0 cents
```

The fix is to return the values: `count, total = add_up(items)`. A test of `summary_after` finds the mistake at once. In C++ and Rust, the same fragment can take a reference (`int&`, `&mut i64`), but a return value keeps the data flow visible at the call.

**Control flow that leaves the fragment.** A `return`, `break` or `continue` inside the fragment leaves the source function or its loop. After the extraction, it leaves only the extracted function. Move such a statement out of the fragment first, or make the function return a value that the caller tests. Refactoring tools often refuse such a fragment.

**Too many parameters.** A fragment that reads seven local variables gives a function with seven parameters. Other refactorings of the catalog help first: Replace Temp with Query removes a local variable, Introduce Parameter Object groups values that travel together, and Replace Function with Command turns the function into an object with fields.[^1]

**A name that says how.** A name such as `loop_over_items` restates the code. The reader still has to read the body. Name the result or the purpose: `total_cents`.

**Fear of the cost of the call.** Fowler writes that the cost of a function call was occasionally a factor in his early career and is now very rare.[^3] Compilers and JIT compilers often inline small functions. Measure before you keep a long function for speed.

## Tools that do it

Many editors and language servers do this refactoring as one command. The tool finds the parameters and the return value, makes the function and replaces the fragment, and it refuses a fragment that it cannot extract safely.

- IntelliJ IDEA (for Java) and PyCharm (for Python) have the refactoring Extract Method.[^4][^5]
- Visual Studio has Extract Method for C#.[^6]
- Visual Studio Code, through the TypeScript language service, has the code action "Extract to method or function" for JavaScript and TypeScript.[^7]
- gopls, the Go language server, has the code actions `refactor.extract.function` and `refactor.extract.method`.[^8]
- rust-analyzer has the assist `extract_function`.[^9]
- clangd, the C and C++ language server, has an ExtractFunction tweak. It extracts statements from a function that is not a template, and it passes a variable that the fragment changes by non-const reference.[^10]

A tool can prove that the extraction keeps the behaviour only for the facts it checks. Run the tests after the tool too.

## References
