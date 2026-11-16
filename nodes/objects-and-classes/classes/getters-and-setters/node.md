---
tag: SPX9Q8
kind: technique
slug: getters-and-setters
name: Getters and setters
aliases: [accessor methods, accessors, mutator methods, mutators, get and set methods, encapsulate field, JavaBeans properties]
area: objects-and-classes/classes
level: core
summary: Make a field private and reach it through a method that reads it and a method that checks and stores a new value, so the class controls every change.
see_also: [field-and-property, encapsulation-and-visibility, define-a-class, method-concept, record-type, immutability, data-class]
sources:
  - url: https://en.wikipedia.org/wiki/Mutator_method
    title: Mutator method, Wikipedia
    locator: Introduction
  - url: https://docs.oracle.com/javase/tutorial/javabeans/writing/properties.html
    title: Properties, The Java Tutorials, JavaBeans
    locator: Properties
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Names, Getters
  - url: https://rust-lang.github.io/api-guidelines/naming.html
    title: Naming, Rust API Guidelines
    locator: Getter names follow Rust convention (C-GETTER)
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, The Python Standard Library
    locator: class property
  - url: https://kotlinlang.org/docs/properties.html
    title: Properties, Kotlin documentation
    locator: Custom getters and setters; Backing fields
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/set
    title: set, JavaScript reference, MDN
    locator: Description
  - url: https://projectlombok.org/features/GetterSetter
    title: "@Getter and @Setter, Project Lombok"
    locator: Overview; Supported configuration keys
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/properties
    title: Properties, C# programming guide
    locator: Auto-implemented properties; Field backed properties
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Functions/get
    title: get, JavaScript reference, MDN
    locator: Description
---

## Purpose

A public field lets any code store any value in it. If a value must follow a rule, for example an email address that must contain `@` or an age that cannot be negative, the rule must then be checked at every place that writes the field, and one missed place stores a bad value. The error shows later, often far from the code that caused it.

Getters and setters give the class one place for each read and each write. The field becomes private. A setter "takes the desired new value as a parameter, optionally validates it, and modifies the private member variable",[^1] and a getter "returns the value of the private member variable". Wikipedia adds: "They are also known collectively as accessors." The class can then check, clean or convert each value, and it can later change how it stores the value without a change to the code that uses it.

## Steps

1. **Make the field private.** Use `private` in Java, C#, C++ and Kotlin, a name with a lower-case first letter in Go, a field without `pub` in Rust, a `#` field in JavaScript, and a name that starts with `_` in Python, which is a convention and not a rule.
2. **Add a getter.** Name it by the convention of the language:

   | Language | Getter | Setter |
   |---|---|---|
   | Java (JavaBeans) | `getEmail()`, or `isActive()` for a `boolean` | `setEmail(value)` |
   | Go | `Email()` | `SetEmail(value)` |
   | Rust | `email()` | `set_email(value)` |
   | C# and Kotlin | property `Email` / `email` with `get` | the `set` accessor of the same property |
   | Python | `@property def email(self)` | `@email.setter` |
   | JavaScript | `get email()` | `set email(value)` |

   JavaBeans says: "To define a property in a bean class, supply public getter and setter methods",[^2] and adds: "A special case for boolean properties allows the accessor method to be defined using is instead of get." Go does not use the prefix: "it's neither idiomatic nor necessary to put Get into the getter's name".[^3] Rust says the same: "With a few exceptions, the get_ prefix is not used for getters in Rust code."[^4] C++ has no fixed rule; many projects use `email()` and `set_email()`.

3. **Add a setter only if other code must change the value.** A field with a getter and no setter is read-only to other code: "A read-only property, for example, has a getter method but no setter."[^2] In the setter, first clean the value, then check it, and store it only if the check passes. On a bad value, raise an exception or return an error, and leave the old value in the field.
4. **Use the same check in the constructor.** Call the setter from the constructor, or call the same check function. Otherwise the constructor can make an object with a bad value that no setter would accept.
5. **Change the code that used the field.** In Java, Go, Rust and C++, each `e.email = x` becomes `e.setEmail(x)` and each read becomes a call. In Python, C#, Kotlin and JavaScript, the getter and setter have the syntax of a field, so the callers do not change.

## Example

In Python. Before the change, `email` is a plain attribute, and the class stores any value:

```python run id=public-attribute
class Employee:
    def __init__(self, name, email):
        self.name = name
        self.email = email  # a public attribute: any code can store any value


e = Employee("Ana", "ana@example.com")
e.email = "  Ana@Example.COM "  # stored as it is
print(repr(e.email))
e.email = "not an address"  # also stored; the error shows later, far from here
print(repr(e.email))
```

```output for=public-attribute
'  Ana@Example.COM '
'not an address'
```

After the change, `email` is a property. The built-in `property` returns "a property attribute",[^5] and its getter and setter run on each read and each assignment. The setter cleans the value with `strip` and `lower`, checks for `@`, and stores the value in `_email`. The constructor assigns `self.email`, so it uses the setter too:

```python run id=property-with-check
class Employee:
    def __init__(self, name, email):
        self.name = name
        self.email = email  # goes through the setter below

    @property
    def email(self):  # the getter
        return self._email

    @email.setter
    def email(self, value):  # the setter: clean the value, then check it
        value = value.strip().lower()
        if "@" not in value:
            raise ValueError(f"not an email address: {value!r}")
        self._email = value


e = Employee("Ana", "ana@example.com")
e.email = "  Ana@Example.COM "  # the same assignment as before
print(repr(e.email))
try:
    e.email = "not an address"
except ValueError as err:
    print("ValueError:", err)
print(repr(e.email))  # the old value is still there
```

```output for=property-with-check
'ana@example.com'
ValueError: not an email address: 'not an address'
'ana@example.com'
```

The assignment `e.email = "  Ana@Example.COM "` is the same in both versions, but the second version stores a clean address and refuses the bad one. The old value stays in the object after the error.

The example files do the same in Java, C#, JavaScript, Go and Rust, with the names of each language: `getEmail` and `isActive` in Java, `Email` and `SetEmail` in Go, `email` and `set_email` in Rust, and properties in C# and JavaScript.

## When to use

- **The value has a rule.** A range, a format, a value that must be cleaned, or a field that other fields depend on. The setter is the only place that must check the rule.
- **The value is computed or stored in another form.** A getter can compute `full_name` from two fields, or return a temperature in Celsius that the class stores in Kelvin. The callers do not see the difference.
- **Other code must not change the value.** A getter with no setter, or a setter that is private, as in C# `{ get; private set; }`.
- **A library must keep its interface stable.** In Java, Go, Rust and C++, a change from a public field to a method changes every caller. A library that may need a check later starts with methods.

Do not add accessors to plain data. If a type only carries values from one place to another, a Java or C# record, a Python dataclass, a Go struct with exported fields or a Rust struct with `pub` fields is shorter, and a getter and setter that only read and write the field add code but no protection. Go says: "There's nothing wrong with providing getters and setters yourself, and it's often appropriate to do so",[^3] which means that they are a choice for each type, not a rule for each field. In Python, C# and Kotlin, start with a plain field or an auto-implemented property, and change it to a property with code only when a rule appears: the callers do not change.

## Pitfalls

- **A pair of accessors for every field.** A class whose fields all have a public getter and a public setter that do no check is a public struct with more code. Add a setter only where other code must change the value, and prefer methods that do one job, such as `deposit(amount)`, to `setBalance(balance)`.
- **A getter that returns a mutable object.** If `getTags()` returns the class's own list, the caller can add to the list without the setter. Return a copy or a read-only view, for example `List.copyOf` in Java, or `&[String]` in Rust.
- **A setter that calls itself.** In Python, a setter that assigns `self.email = value` calls the setter again, until `RecursionError`. Store the value under another name, such as `_email`. In Kotlin, a custom setter uses the `field` keyword for the stored value: "You can access backing fields by using the field keyword in a custom get() or set() function."[^6]
- **A constructor that skips the check.** A constructor that assigns the field directly can make an object that the setter would refuse. Step 4 avoids this.
- **A rule across two fields.** A setter checks one value. If `start` must be before `end`, two setters cannot keep the rule when the caller moves both values; use one method that sets both, or make the type immutable and check in the constructor.
- **A setter without a getter in JavaScript.** MDN says that the `set` syntax "binds an object property to a function to be called when there is an attempt to set that property".[^7] If the class has only the setter, a read of the property returns `undefined`.
- **Work hidden in a property.** In Python, C#, Kotlin and JavaScript, a getter looks like a field read, so callers expect it to be fast and to have no side effects. Do not put a network call or a slow computation in a getter; use a method with a verb in its name.

## Tools that do it

| Language | Generated accessors | By hand |
|---|---|---|
| Java | records; Lombok `@Getter` and `@Setter` | `getX`, `isX`, `setX` methods |
| C# | auto-implemented properties `{ get; set; }` | a property with a backing field |
| Kotlin | every `val` and `var` property | a custom `get()` or `set(value)` |
| Python | none needed: attributes are public | `property` |
| JavaScript | none | `get` and `set` in a class or object literal |
| Go, Rust, C++ | none | methods |

Lombok is an annotation processor for Java: "You can annotate any field with @Getter and/or @Setter, to let lombok generate the default getter/setter automatically."[^8] It follows the JavaBeans names, and the `lombok.accessors.fluent` setting drops the prefixes. The C# guide says of an auto-implemented property: "The compiler generates a hidden backing field for the property."[^9] Kotlin does the same for each property: "By default, Kotlin automatically generates getters and setters."[^6] MDN describes the JavaScript form: "The get syntax binds an object property to a function that will be called when that property is looked up."[^10]

## References
