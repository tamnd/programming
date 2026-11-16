---
tag: SNT975
kind: technique
slug: open-classes-and-monkey-patching
name: Open classes and monkey patching
aliases: [monkey patching, monkey patch, monkeypatch, open class, reopen a class, guerrilla patch, duck punching, runtime patching, hot fix]
area: objects-and-classes/classes
level: advanced
summary: Change a class or module that other code owns at run time, by adding or replacing its methods, and keep the change small, guarded, reversible and in one place.
see_also: [extension-methods, add-a-method-to-an-existing-type, prototype-based-programming, incomplete-library-class, decorator-pattern, test-double, dependency-injection, global-data]
sources:
  - url: https://en.wikipedia.org/wiki/Monkey_patch
    title: Monkey patch, Wikipedia
    locator: Introduction; Pitfalls
  - url: https://docs.ruby-lang.org/en/master/syntax/refinements_rdoc.html
    title: Refinements, Ruby documentation
    locator: Refinements; Scope
  - url: https://docs.ruby-lang.org/en/master/syntax/modules_and_classes_rdoc.html
    title: Modules and Classes, Ruby documentation
    locator: Module Definition
  - url: https://docs.python.org/3/library/unittest.mock.html
    title: unittest.mock, mock object library, The Python Standard Library
    locator: patch; patch.object; Where to patch
  - url: https://docs.python.org/3/reference/datamodel.html
    title: The standard type hierarchy, Data model, The Python Language Reference
    locator: 3.2.10 Custom classes
  - url: https://developer.mozilla.org/en-US/docs/Glossary/Polyfill
    title: Polyfill, MDN Glossary
    locator: Polyfill
  - url: https://www.typescriptlang.org/docs/handbook/declaration-merging.html
    title: Declaration Merging, TypeScript Handbook
    locator: Module Augmentation
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/extension-methods
    title: Extension members, C# programming guide
    locator: Binding extension members at compile time
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method sets
---

## Purpose

A library has a bug, or it lacks one method, and the code that uses it cannot wait for a new release. A fork of the library is a large cost: the team must merge each new upstream version into it. In a dynamic language there is a smaller way. Wikipedia describes a monkey patch as a change to "the runtime code (not the source code)" of a program, and says: "Modifying the runtime code allows for modifying the behavior of third-party software without maintaining a modified version of the source code."[^1]

An open class is a class that code can change after its definition. In Ruby, a second `class String ... end` block adds to the class `String` and does not make a new class. The Ruby documentation says: "Due to Ruby’s open classes you can redefine or add functionality to existing classes."[^2] Python and JavaScript do the same with an assignment to an attribute of a class or a prototype. Each change is visible to every user of the class, which makes the technique useful and also dangerous. This page gives the steps that keep a patch small and safe.

## Steps

1. **Look for a cleaner way first.** A subclass, a wrapper object, a hook or a setting of the library, an extension method in C# or Kotlin, or a plain function changes less. Report the bug upstream, so that the patch can go away later. Ruby gives this advice for its open classes: "it is best to only reopen modules you own."[^3]
2. **Find the name that the code looks up.** A patch changes one binding: an attribute of a class, a module or a prototype. Code that copied the old value before the patch does not see the change. For tests, the Python documentation says: "The key is to patch out SomeClass where it is used (or where it is looked up)."[^4]
3. **Keep the original.** Store the old function in a variable before the assignment. The replacement can then call it for the work that it does not change, and the program can restore it.
4. **Add a guard.** Patch only the versions that have the bug, and check that the name exists before a replacement, or that it does not exist before an addition. Wikipedia notes that "a monkey patch may be conditional and thus only applied if appropriate."[^1]
5. **Put all patches in one module and apply them once, early.** Import that module at the start of the program, before other code makes objects or copies references. Write the reason, the affected versions and the upstream issue next to each patch.
6. **In tests, use a tool that undoes the patch.** A patch in a test that stays in force breaks the tests that run after it. `unittest.mock.patch` restores the original at the end of the test: "When the function/with statement exits the patch is undone."[^4]
7. **Remove the patch when it is no longer necessary.** After an upgrade, check whether the library fixed the bug, and delete the patch and its guard.

## Example

In Python, a class is an object, and an assignment to one of its attributes changes the class itself: "Class attribute assignments update the class’s dictionary, never the dictionary of a base class."[^5] Here `Invoice` stands for a class from a library. The patch adds the method `as_text` and replaces `total` with a version that rounds the sum. The new `total` calls the original, which the code keeps in `_original_total`:

```python run id=patch-a-class-at-run-time
class Invoice:  # stands for a class in a library that the code does not own
    def __init__(self, amounts):
        self.amounts = amounts

    def total(self):
        return sum(self.amounts)


old = Invoice([0.1, 0.2])
print(old.total())


def as_text(self):
    return f"Invoice of {len(self.amounts)} items: {self.total()}"


_original_total = Invoice.total


def total(self):
    return round(_original_total(self), 2)  # the patch calls the original


Invoice.as_text = as_text  # add a method
Invoice.total = total  # replace a method

print(old.total())
print(old.as_text())
print(Invoice.total is _original_total)

try:
    str.as_text = as_text
except TypeError as e:
    print(e)
```

```output for=patch-a-class-at-run-time
0.30000000000000004
0.3
Invoice of 2 items: 0.3
False
cannot set 'as_text' attribute of immutable type 'str'
```

The object `old` existed before the patch, but it uses the new methods, because Python looks up a method on the class at each call. The last line shows a limit of Python: the built-in types such as `str` refuse new attributes, so a patch can change classes written in Python but not the built-in types.

In a test, the patch must end with the test. `mock.patch.object` replaces the method in a `with` block and puts the original back at the end of the block, also when the block raises an exception:

```python run id=patch-only-in-a-block
from unittest import mock


class Rates:
    def euro_to_dollar(self):
        raise ConnectionError("no network in tests")


def price_in_dollars(euros):
    return round(euros * Rates().euro_to_dollar(), 2)


with mock.patch.object(Rates, "euro_to_dollar", return_value=1.1):
    print(price_in_dollars(10))
    print(type(Rates.euro_to_dollar).__name__)

try:
    price_in_dollars(10)
except ConnectionError as e:
    print("after the block:", e)
```

```output for=patch-only-in-a-block
11.0
MagicMock
after the block: no network in tests
```

The example files do the same in JavaScript, with a helper that patches a prototype method and returns a function that undoes the patch, and in TypeScript, where an interface tells the compiler about the new method. The Go file shows what a language without open classes uses instead: a variable of a function type that a test can replace.

## When to use

- **A hot fix for a bug in a dependency.** The patch fixes the program now, and it goes away when the upstream release has the fix. Wikipedia describes the main use as a "workaround to a bug or feature which does not act as desired."[^1]
- **A polyfill.** A polyfill adds a standard method to an older run-time that does not have it: "A polyfill is a piece of code (usually JavaScript on the Web) used to provide modern functionality on older browsers that do not natively support it."[^6] A good polyfill changes nothing when the method is already there, and it follows the standard exactly.
- **A test that must replace a dependency.** A clock, a network call or a random number generator that the code under test reaches through a class or a module. A patch that ends with the test is common in Python, Ruby and JavaScript. Dependency injection is often cleaner when the code is yours.
- **A plugin system that the host designed for it.** Wikipedia notes that "a monkey patch is sometimes the official method of extending a program", and gives Firefox add-ons as an example.[^1]

Do not use a monkey patch in a library that other programs use. Its patch changes the classes of every program that imports it, and those programs did not ask for the change. Do not use it to add convenience methods to the built-in types of a large program; an extension method, a function or a wrapper does the same work without a global change.

## Pitfalls

- **The change is global.** Every user of the class sees the patch, also code that expects the original. The Ruby documentation says: "Unfortunately the scope of such changes is global. All users of the monkey-patched class see the same changes. This can cause unintended side-effects or breakage of programs."[^2]
- **Two patches of the same method.** "If the same method is patched multiple times, then only the last one is used; the other patches have no effect".[^1] A patch that keeps and calls the original, as in the example, lets two patches work together, but only if both do it.
- **A new version breaks the patch.** "A new release of the patched software may break the patch."[^1] The method can get a new name, new parameters or a new caller. A guard on the version and a test of the patched behaviour find this early. A new version of the language can also add a method with the name that the patch added, and the two then differ.
- **The source code no longer tells the truth.** "A monkey patch creates a discrepancy between the source code and actual behavior that can confuse developers."[^1] A developer who reads the library source sees code that does not run. Keep all patches in one module, and write why each one exists.
- **A patch in the wrong place has no effect.** In Python, `from a import SomeClass` copies the reference into the importing module, so a patch of `a.SomeClass` after the import does not change the copy. Patch the name where the code looks it up.
- **A test patch that stays.** A patch that a test applies with a plain assignment stays in force for the tests that run after it, so the results depend on the order of the tests. Use a tool that undoes the patch, or restore the original in a `finally` block.

## Tools that do it

| Language | Change a class at run time | Limit the change | In tests |
|---|---|---|---|
| Ruby | reopen the class with `class C ... end`; `define_method` | refinements: `refine` and `using` | test doubles of the test library |
| Python | assign to a class attribute; `setattr` | none; built-in types refuse | `unittest.mock.patch`, `patch.object` |
| JavaScript | assign to `prototype`; `Object.defineProperty` | none | the mock functions of the test library |
| TypeScript | the JavaScript forms | `declare module` and `declare global` declare the new members | as JavaScript |
| C#, Kotlin, Swift | not possible; extension members add methods but do not replace them | the extension is visible where its namespace is imported | interfaces and test doubles |
| Java, Go, Rust, C++ | not possible in the language | not applicable | interfaces, traits, function variables |

Ruby refinements limit a patch to the code that asks for it: "Refinements provide a way to extend a class locally."[^2] A file activates them with `using`, and "Refinements are lexical in scope." Code in other files sees the original class.

TypeScript needs a declaration for each patch. The handbook says that "you can patch existing objects by importing and then updating them",[^7] and a `declare module` block tells the compiler about the new member.

C# extension methods cannot replace a method: "You can use extension members to extend a class or interface, but not to override behavior defined in a class."[^8] Go fixes the methods of each type in its declaration, because "The method set of a type determines the methods that can be called on an operand of that type."[^9] In these languages, code that must be replaceable takes an interface, a trait or a function as a parameter, which is [dependency injection](/dependency-injection/).

## References
