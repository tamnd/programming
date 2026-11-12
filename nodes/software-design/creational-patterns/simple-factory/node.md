---
tag: DCC5WZ
kind: pattern
slug: simple-factory
name: Simple factory
aliases: [simple factory pattern, factory function, parameterized factory, static factory with a switch]
area: software-design/creational-patterns
level: intro
summary: >-
  A simple factory is one function that takes a parameter, picks a concrete class from it and returns a new object. Callers use the object through an interface and do not name the class.
family: creational-design
polarity: positive
intent: "Put the choice of a concrete class in one function, which takes a parameter and returns a new object through a common interface, so that callers do not create the concrete classes themselves."
also_known_as: [Factory, Factory function]
history:
  year: 2004
  people: [Eric Freeman, Elisabeth Freeman, Kathy Sierra, Bert Bates]
  note: Head First Design Patterns (2004) defined the Simple Factory in its chapter on factories, as a first step before Factory Method and Abstract Factory. Design Patterns (1994) has no pattern of this name.
see_also: [factory-method-pattern, abstract-factory-pattern, static-factory-method, dependency-injection]
sources:
  - url: https://refactoring.guru/design-patterns/factory-comparison
    title: Factory Comparison, Refactoring.Guru
    locator: 4. Simple factory pattern
  - url: https://en.wikipedia.org/wiki/Factory_(object-oriented_programming)
    title: Factory (object-oriented programming), Wikipedia
    locator: Terminology; Use; Applications
  - book: isbn:9780596007126
    title: Eric Freeman, Elisabeth Freeman, Kathy Sierra and Bert Bates, Head First Design Patterns, O'Reilly, 2004
    locator: Chapter 4, The Factory Pattern, The Simple Factory defined
  - url: https://docs.python.org/3.14/library/functions.html#open
    title: Built-in Functions, The Python Standard Library, Python 3.14
    locator: open
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/_pyio.py#L232-L268
    title: CPython v3.14.0 source, Lib/_pyio.py
    locator: open, lines 232 to 268
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Calendar.html
    title: Calendar, Java SE 25 API
    locator: Class description; getInstance()
  - url: https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Calendar.java#L1688-L1732
    title: OpenJDK jdk-25+36 source, java/util/Calendar.java
    locator: createCalendar, lines 1688 to 1732
---

## Intent

Put the choice of a concrete class in one function. The function takes a parameter, picks a class from it, and returns a new object. The callers use the object through a common interface, so they do not need to know the concrete classes.[^1]

## Problem

A shop sends a message to a user when an order ships. The user chooses the channel: email, SMS or a push message to the app. Each channel has its own class, and all of them have a `send` method.

The code that sends the message must create an object of the right class. If each caller does this with its own `if` statement, the same conditional is in many places. When the shop adds a channel, each copy must change, and a copy that is not changed fails at run time. The forces are these:

- The concrete class depends on a value that is known only at run time, such as a setting or an input.
- Many callers need such an object, and they need only its interface.
- A constructor cannot make this choice. A constructor creates an object of its own class, and when it runs there is no object yet to dispatch on.[^2]

## Solution

Write one function, the **simple factory**, that takes the parameter and returns the object. The function holds the only conditional that names the concrete classes. Refactoring.Guru describes it as one creation method with a large conditional that, based on the parameters, chooses which product class to instantiate and then returns it.[^1]

```text
 Client                       make_notifier(channel)             products
 ------                       ----------------------             --------
 n = make_notifier("sms") --> "email"? -> new EmailNotifier       EmailNotifier
                              "sms"?   -> new SmsNotifier  -----> SmsNotifier
                              "push"?  -> new PushNotifier        PushNotifier
                              else     -> error
 n.send(user, text)  ------------------------------------------> SmsNotifier.send
```

The participants are the **factory**, the **product** interface, and the **concrete products**. The factory can be a free function, a static method or a method of a small factory object. In a language where classes are values, such as Python or JavaScript, a table from the parameter to the class can replace the conditional.

**Not a GoF pattern.** Design Patterns has no pattern named "factory". It has two patterns that use factories, Factory Method and Abstract Factory. Some sources call the general idea the factory pattern, and others consider it a programming idiom and call it a simple factory.[^2] The name comes from Head First Design Patterns.[^1][^3]

## Example

The example files show one simple factory in each language. `make_notifier` takes a channel name and returns an email, SMS or push notifier. The program asks for each of the three channels and for an unknown channel, `fax`, which gives an error:

| Language | Factory | Unknown channel |
|---|---|---|
| Python | a function with an `if` chain | `ValueError` |
| JavaScript | a `Map` from the name to the class | `RangeError` |
| TypeScript | a `switch` on a union type, with a `never` check | rejected by a type guard |
| Java | a `switch` expression | `IllegalArgumentException` |
| C# | a `switch` expression | `ArgumentException` |
| C++ | a function that returns `std::unique_ptr<Notifier>` | `std::invalid_argument` |
| Go | `NewNotifier` returns an interface and an `error` | an `error` value |
| Rust | a `match` that returns `Box<dyn Notifier>` | `Err` |

## Consequences

The benefits:

- **One place to change.** To add a channel, add a class and one case in the factory. Wikipedia notes that a change of the factory is typically one line in one file, and that it is easier than to change every place in the code where a new object is created.[^2]
- **Callers know only the interface.** The client code deals only with the abstract type, and does not need the header files or the declarations of the concrete classes.[^2]
- **Validation in one place.** The factory decides what an unknown parameter means: an error, a default, or a null object.

The costs:

- **The factory knows every class.** Each new kind changes the factory. A factory that a library ships cannot be extended by its users without a change to the library.
- **The conditional grows.** Refactoring.Guru notes that the method can become too big over time. If you then move parts of it to subclasses, the result becomes the Factory Method pattern.[^1]

**The conditional must be in the factory only.** The benefit is lost when callers keep their own copy of the choice. Here the shop adds the push channel, and one of two copies is not changed:

```python wrong id=scattered-conditional
class Email:
    name = "email"


class Sms:
    name = "sms"


class Push:
    name = "push"


def welcome(channel):
    if channel == "email":
        n = Email()
    elif channel == "sms":
        n = Sms()
    elif channel == "push":  # added when push came
        n = Push()
    else:
        raise ValueError(f"unknown channel: {channel}")
    return f"welcome sent by {n.name}"


def reset_password(channel):
    if channel == "email":
        n = Email()
    elif channel == "sms":
        n = Sms()
    else:  # this copy was not changed
        raise ValueError(f"unknown channel: {channel}")
    return f"reset link sent by {n.name}"


for task in (welcome, reset_password):
    try:
        print(task("push"))
    except ValueError as e:
        print(f"{task.__name__} failed: {e}")
```

```output for=scattered-conditional
welcome sent by push
reset_password failed: unknown channel: push
```

The fix is to move the conditional into one `make_notifier` function, and to call it from both places.

## When not to use it

Do not write a factory when there is only one concrete class. A constructor call is clearer, and the factory adds a name to learn.

Do not use a simple factory when users of a library must add their own kinds. A registry that users can add to, a [[factory-method-pattern]] that subclasses override, or a factory object that the caller passes in lets new kinds come without a change to the factory.

Do not use it to hide a choice that the caller already knows. If the caller always wants an SMS notifier, it can create one, or receive one by [[dependency-injection]].

## Variants and related patterns

**Static factory method.** A [[static-factory-method]] is a static method that returns an instance of its class, often of a subclass. A simple factory is often written as one, with the parameter that picks the class.

**Factory Method.** In the [[factory-method-pattern]], a creator class declares a creation method, and each subclass overrides it to create its own product. The choice moves from a conditional to the class hierarchy. A simple factory that has grown too big can become a Factory Method.[^1]

**Abstract Factory.** An [[abstract-factory-pattern]] creates a family of related products through one interface. A simple factory creates one kind of product, and it is not an abstract factory even when it is declared abstract.[^1]

**Registry.** A table from the parameter to a constructor, which other code can add to at start-up. The factory looks up the parameter in the table in place of a fixed conditional, so a plug-in can add a kind without a change to the factory.

## Known uses

- Python: the built-in `open()` returns a different class for each mode. In text mode it returns an `io.TextIOWrapper`. In binary mode with buffering it returns an `io.BufferedReader`, an `io.BufferedWriter` or an `io.BufferedRandom`, by the mode. With buffering disabled it returns an `io.FileIO`.[^4] The pure-Python version in [Lib/_pyio.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/_pyio.py#L232-L268) shows the choice: it creates a `FileIO`, then wraps it in a buffered class picked by the mode, then in a `TextIOWrapper` unless the mode is binary.[^5]
- Java: `Calendar.getInstance()` returns a `Calendar` whose fields are set to the current date and time.[^6] The private method `createCalendar` in [java/util/Calendar.java at jdk-25+36](https://github.com/openjdk/jdk/blob/jdk-25%2B36/src/java.base/share/classes/java/util/Calendar.java#L1688-L1732) picks the class from the locale: a `BuddhistCalendar` for the `th_TH` locale, a `JapaneseImperialCalendar` for the `ja_JP_JP` locale, and a `GregorianCalendar` for other locales.[^7] The caller sees only the abstract class `Calendar`.

## References
