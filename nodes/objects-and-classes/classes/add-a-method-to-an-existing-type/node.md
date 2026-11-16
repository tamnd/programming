---
tag: 089KSM
kind: task
slug: add-a-method-to-an-existing-type
name: Add a method to an existing type
aliases: [add a method to a built-in type, extend a type, extend String, extension method, extension trait, add a method to a class you do not own, retroactive method]
area: objects-and-classes/classes
level: core
summary: Give the string type of the language a word_count method that it does not have, with an extension method, an extension trait, a prototype property, a subclass, a new type or a free function.
contract: contract.yaml
category: objects
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [extension-methods, open-classes-and-monkey-patching, define-a-method-on-a-type, partial-classes, method-concept, incomplete-library-class]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/extension-methods
    title: Extension members, C# programming guide
    locator: Introduction; Binding extension members at compile time
  - url: https://doc.rust-lang.org/book/ch10-02-traits.html
    title: Defining Shared Behavior with Traits, The Rust Programming Language
    locator: Implementing a Trait on a Type
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Method declarations
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty
    title: Object.defineProperty(), JavaScript reference, MDN
    locator: Description
  - url: https://www.typescriptlang.org/docs/handbook/declaration-merging.html
    title: Declaration Merging, TypeScript Handbook
    locator: Global augmentation
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/String.html
    title: String (Java SE 21 & JDK 21), API documentation
    locator: Class String, declaration
  - url: https://kotlinlang.org/docs/extensions.html
    title: Extensions, Kotlin documentation
    locator: Introduction; Nullable receivers
  - url: https://en.wikipedia.org/wiki/Monkey_patch
    title: Monkey patch, Wikipedia
    locator: Introduction; Pitfalls
---

## Task

Add a method `word_count` to the string type of the language, so that code can write `text.word_count()` for any string `text`. The method returns the number of words, where a word is a run of one or more characters that are not whitespace. The string type comes from the language or its standard library, so the code cannot change its source.

Then use it. The function `word_count(text)` returns the count, through the method when the language allows one. For example, `word_count("the quick fox")` is 3, and `word_count("  two\twords\r\nand three  ")` is 4. The input has 0 to 100,000 characters, and its only whitespace characters are space, tab, newline and carriage return. The tests also check that the method does not break other code that uses strings.

Languages give very different answers. Some add a method that is only a static function with a receiver, some change the type for the whole program, and some do not allow either. Each solution uses the nearest form that its language allows.

## Solutions

| Language | Form | Does the type change? | Called as |
|---|---|---|---|
| Python | a subclass `Text(str)` | no; only `Text` values have the method | `Text(s).word_count()` |
| JavaScript | a property on `String.prototype` | yes, for all code in the realm | `s.wordCount()` |
| TypeScript | `declare global` plus the prototype property | yes, for all code in the realm | `s.wordCount()` |
| Java | a static method in the utility class `Words` | no | `Words.wordCount(s)` |
| C# | an extension method | no | `s.WordCount()` |
| Go | a new type `Text` with `string` as its underlying type | no | `Text(s).WordCount()` |
| Rust | an extension trait `WordCount` implemented for `str` | no | `s.word_count()` |
| C | a function | no | `word_count(s)` |
| C++ | a free function in the namespace `words` | no | `words::word_count(s)` |

- **C# uses an extension method.** `WordCount(this string text)` is a static method, and the `this` modifier lets code call it as `text.WordCount()`. The guide says that extension members add methods "without creating a new derived type, recompiling, or otherwise modifying the original type."[^1] The test checks that `string` itself has no `WordCount` method.
- **Rust uses an extension trait.** The crate defines the trait `WordCount` and implements it for `str`. The orphan rule allows this, because "we can implement a trait on a type only if either the trait or the type, or both, are local to our crate."[^2] Here the trait is local. The method works on a `String` too, because a `String` dereferences to `str`, and only code that imports the trait can call it.
- **Go uses a new type.** A method needs a receiver type that is declared in the same package: "A receiver base type cannot be a pointer or interface type and it must be declared in the same package as the method."[^3] So the package declares `type Text string` and puts `WordCount` on it. A conversion `Text(s)` costs nothing, but a `Text` is a different type from `string`.
- **JavaScript changes the prototype.** Every string gets its methods from `String.prototype`, so a function on it is a method of every string in the program. The code uses `Object.defineProperty`, because "properties added using Object.defineProperty() are not writable, not enumerable, and not configurable"[^4] by default; the code sets `writable` and `configurable` to true, and keeps the method out of `for...in` loops like the built-in methods. It also refuses to replace a `wordCount` that is already there.
- **TypeScript adds a type declaration.** The run-time code is the JavaScript code. `declare global { interface String { wordCount(): number } }` tells the compiler about the method, because interface declarations merge, and "You can also add declarations to the global scope from inside a module".[^5]
- **Python uses a subclass.** A built-in type such as `str` does not accept new attributes, so `str.word_count = ...` fails with a `TypeError`. A subclass `Text(str)` adds the method and keeps all the methods of `str`. Only values made as `Text` have the method.
- **Java uses a static method.** Java has no extension methods, and `String` is declared as `public final class String`,[^6] so a subclass is not possible either. `Words.wordCount(text)` takes the string as its first parameter.
- **C and C++ use a function.** C has no methods. C++ cannot add a member function to `std::string`, so a free function in a namespace takes a `std::string_view`, which accepts a `std::string`, a string literal and a part of a string.

### Alternatives

- **Kotlin extension functions.** `fun String.wordCount() = ...` adds a function that code calls as a method. Kotlin says that "extensions don't modify the classes or interfaces they extend",[^7] in the same way as C#.
- **Monkey patching a user-defined class.** In Python, Ruby and JavaScript, code can assign a function to an attribute of a class that it did not write, and every instance gets the method. See [open classes and monkey patching](/open-classes-and-monkey-patching/) for the risks.
- **A wrapper or decorator type.** A class that holds a string and adds methods works in every language with classes, as Go's `Text` does. It is the safest form, but code must wrap and unwrap the values.
- **A free function.** The plain function `word_count(text)` is the answer in C, and it is a good answer in every language. A method form is only shorter to call.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the empty string has no words, so the result is 0.
- `blank`: a string of only whitespace also has no words. A count of the separators, plus one, gives a wrong answer here and in the `empty` case.
- `mixed-whitespace`: tabs, newlines, a carriage return and runs of more than one space separate words, and the string starts and ends with spaces. A split on a single space gives empty words.
- `non-ascii`: `naïve café 東京` has 3 words. The counts are of runs, so it does not matter whether a language reads bytes, UTF-16 code units or code points.

## Pitfalls

- **A changed prototype is global.** The JavaScript method is on every string in the realm, for every library. Two libraries that add `wordCount` with different rules break each other. Wikipedia lists this risk: "If the same method is patched multiple times, then only the last one is used; the other patches have no effect".[^8] A new version of the language can also add a method with the same name. The solution checks for an existing `wordCount` first.
- **An enumerable property shows in loops.** An assignment such as `String.prototype.wordCount = function () {...}` makes an enumerable property, so `for (const k in s)` lists it next to the indexes. `Object.defineProperty` with `enumerable: false` does not.

```javascript run id=enumerable-prototype-method
String.prototype.shout = function () { return this.toUpperCase() + "!"; };
Object.defineProperty(String.prototype, "whisper", {
  value: function () { return this.toLowerCase() + "..."; },
  writable: true,
  configurable: true,
});

const keys = [];
for (const k in "hi") keys.push(k);
console.log(keys.join(" "));
console.log("Hi".shout(), "Hi".whisper());
```

```output for=enumerable-prototype-method
0 1 shout
HI! hi...
```
- **A member wins over an extension.** A C# extension method or a Kotlin extension function with the same signature as a member is never called. If a later version of the type adds `WordCount`, the code calls the new member, which can count words in a different way.
- **Extensions use the declared type.** A C# extension method is chosen at compile time from the declared type of the expression, so it does not act like a virtual method. A Rust trait method works only where the trait is imported.
- **A new type does not mix with the old type.** A Go `Text` is not a `string`, and a Python `Text` loses its type after most `str` methods: `Text("a").upper()` returns a plain `str`.

## Related tasks

- [[define-a-method-on-a-type]] defines a method on a type that the code owns.
- [[extension-methods]] explains how extension methods work and how the compiler picks them.
- [[open-classes-and-monkey-patching]] explains how dynamic languages change a class at run time.
- [[partial-classes]] splits one type that the code owns into parts.

## References
