---
tag: YC32KZ
kind: technique
slug: replace-constructor-with-factory-function
name: Replace constructor with factory function
aliases: [replace constructor with factory method]
area: construction-practice/refactoring-apis
level: core
summary: >-
  Replace direct calls of a constructor with calls of an ordinary function that makes the object, so that the creation can have a name and can choose what to return.
family: refactoring
polarity: positive
intent: "Give object creation a name and free it from the limits of a constructor."
also_known_as: [Replace Constructor with Factory Method]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has it as Replace Constructor with Factory Method. The second edition (2018) calls it Replace Constructor with Factory Function, in the chapter on refactoring APIs.
see_also: [static-factory-method, simple-factory, factory-method-pattern, remove-setting-method, change-function-declaration]
sources:
  - url: https://refactoring.com/catalog/replaceConstructorWithFactoryFunction.html
    title: Martin Fowler, Catalog of Refactorings, Replace Constructor with Factory Function
    locator: the entry, the createEngineer sketch; aliases Replace Constructor with Factory Method
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceConstructorWithFactoryFunction, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Replace Constructor with Factory Function
  - url: https://www.jetbrains.com/help/idea/replace-constructor-with-factory-method.html
    title: JetBrains, IntelliJ IDEA documentation, Replace constructor with factory method
    locator: the first paragraph
  - url: https://rust-lang.github.io/api-guidelines/predictability.html
    title: Rust API Guidelines, Predictability
    locator: Constructors are static, inherent methods (C-CTOR)
---

## Purpose

In many languages a constructor has limits that an ordinary function does not have:

- Its name is the name of the class, so it cannot say which kind of object the caller wants.
- It must return a new instance of exactly that class. It cannot return a subclass, a cached object or a proxy.
- It needs a special call, such as `new` in JavaScript, Java and C#.

Replace constructor with factory function puts a normal function in front of the constructor. The catalogue card turns `new Employee(document.leadEngineer, 'E')` into `createEngineer(document.leadEngineer)`.[^1] The code `'E'` is gone from the call, and the name says what the caller makes.

## Steps

1. Make a factory function. Its body calls the constructor.
2. Replace each call of the constructor with a call of the factory function. Test after each change.
3. If the language allows it, limit the visibility of the constructor, so that new code must use the factory function.

## Example

An employee has a type code. A caller that makes an engineer must know that the code of an engineer is `'E'`. After the refactoring, the caller calls `createEngineer`.

```javascript run id=factory-function
import assert from "node:assert/strict";

class Employee {
  constructor(name, typeCode) {
    this._name = name;
    this._typeCode = typeCode;
  }
  get name() { return this._name; }
  get type() { return Employee.legalTypeCodes[this._typeCode]; }
  static get legalTypeCodes() { return { E: "Engineer", M: "Manager", S: "Salesperson" }; }
}

function createEmployee(name, typeCode) {
  return new Employee(name, typeCode);
}

function createEngineer(name) {
  return new Employee(name, "E");
}

const before = new Employee("Ana", "E");
const after = createEngineer("Ana");
assert.deepEqual(after, before);
assert.equal(createEmployee("Bo", "M").type, "Manager");
console.log(`${after.name}: ${after.type}`);
```

```output
Ana: Engineer
```

| Language | What the example shows |
|---|---|
| JavaScript | `new Employee(name, 'E')` replaced with `createEngineer(name)`, as above |
| Python | a class method `Employee.engineer(name)`; Python has no `new`, and the call of a class already looks like a call of a function |
| Go | `NewEngineer(name)`, a plain function next to `NewEmployee(name, code)` |

## When to use

- **The call needs a name.** A code, a flag or a list of literals in a constructor call says less than a function name such as `createEngineer`. See [[boolean-parameter-smell|Boolean parameter]].
- **The creation must choose the class.** A factory function can return a subclass or another implementation of an interface. This is the start of [[simple-factory|Simple factory]].
- **The creation must reuse objects.** A factory function can return a cached instance. In Java and C#, `new` always makes a new object.
- **The constructor must stay hidden.** With a private constructor, the factory function is the only way in.

## Pitfalls

**Constructors in other languages.** The refactoring matters most in languages where `new` calls the constructor. In Rust, the API Guidelines say that constructors "are just a convention". They define them as "static (no self) inherent methods for the type that they construct."[^5] There, and in other languages where a plain function makes the value, the factory function is already the normal style, and the refactoring is only a rename, for example from `new` to `engineer`.

**Too many factory functions.** One factory function for each combination of arguments makes the API large. Use them for the cases that callers use often.

**Not the same as the Factory Method pattern.** A factory function is a plain function. The Factory Method design pattern uses a method that subclasses override to choose the class. See [[factory-method-pattern|Factory method]] and [[static-factory-method|Static factory method]].

## Tools that do it

IntelliJ IDEA has the action Replace Constructor with Factory Method. Its documentation says that it "lets you hide a constructor and replace it with a static method which returns a new instance of a class."[^4]

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tags refactoring-apis and grouping-function.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
