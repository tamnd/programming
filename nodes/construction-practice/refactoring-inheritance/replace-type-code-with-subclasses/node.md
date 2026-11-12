---
tag: RCQY38
kind: technique
slug: replace-type-code-with-subclasses
name: Replace type code with subclasses
aliases: [extract subclass, replace type code with state/strategy]
area: construction-practice/refactoring-inheritance
level: core
summary: >-
  Replace a field that holds a type code, such as "engineer" or "salesman", with one subclass for each value of the code.
family: refactoring
polarity: positive
intent: "Let the class of an object say what kind of thing it is."
also_known_as: [Extract Subclass, Replace Type Code with State/Strategy]
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on dealing with inheritance. The catalogue lists Replace Type Code with State/Strategy and Extract Subclass as its aliases.
see_also: [replace-conditional-with-polymorphism, primitive-obsession, repeated-switches, replace-constructor-with-factory-function, state-pattern, strategy-pattern]
sources:
  - url: https://refactoring.com/catalog/replaceTypeCodeWithSubclasses.html
    title: Martin Fowler, Catalog of Refactorings, Replace Type Code with Subclasses
    locator: the entry, the createEmployee sketch; inverse of Remove Subclass; aliases Replace Type Code with State/Strategy, Extract Subclass
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry replaceTypeCodeWithSubclasses, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 12, Dealing with Inheritance, Replace Type Code with Subclasses
---

## Purpose

A type code is a field whose value says which kind of thing an object is, for example the type of an employee. Code that depends on the kind tests the field with `if` or `switch`. When the same test is in many places, each new kind needs a change in each of them. See [[repeated-switches|Repeated switches]] and [[primitive-obsession|Primitive obsession]].

Replace type code with subclasses makes one subclass for each value. The behavior for each kind can then move into its subclass, with [[replace-conditional-with-polymorphism|Replace Conditional with Polymorphism]]. The catalogue card changes `createEmployee(name, type)` from `return new Employee(name, type);` to a `switch` on the type that returns a `new Engineer(name)`, a `new Salesman(name)` or a `new Manager(name)`.[^1] The `switch` is now in one place, the factory function. The card names the inverse, Remove Subclass.[^1]

### Direct and indirect inheritance

- **Direct.** The class with the type code gets the subclasses, as in the card. This works when the kind of an object never changes, and when the class has no other subclasses yet.
- **Indirect.** The type code becomes an object of its own, such as `EmployeeType`, and that class gets the subclasses. The employee holds a type object. The type can then change at run time, and the employee class stays free for other subclasses. This form is the State or Strategy pattern, and the catalogue gives Replace Type Code with State/Strategy as an alias of the refactoring.[^1] See [[state-pattern|State]] and [[strategy-pattern|Strategy]].

## Steps

1. Encapsulate the type code field, so that all code reads it through one getter.
2. Pick one value of the code. Make a subclass for it, and override the getter of the type code to return the value.
3. Make a factory function, or change the one that exists, to return the subclass for that value. See [[replace-constructor-with-factory-function|Replace Constructor with Factory Function]].
4. Test. Repeat steps 2 and 3 for each value.
5. Remove the type code field from the superclass.
6. Move the behavior that depends on the type into the subclasses with Replace Conditional with Polymorphism. Test after each move.

## Example

Each employee had a `type` string, and the bonus was a `switch` on it. After the refactoring, the factory function has the only `switch`, and each subclass has its own bonus.

```javascript run id=type-code-to-subclasses
import assert from "node:assert/strict";

class EmployeeBefore {
  constructor(name, type) { this._name = name; this._type = type; }
  get bonus() {
    switch (this._type) {
      case "engineer": return 1000;
      case "salesman": return 3000;
      case "manager": return 5000;
      default: throw new Error(`unknown type ${this._type}`);
    }
  }
}

class Employee {
  constructor(name) { this._name = name; }
  get name() { return this._name; }
}
class Engineer extends Employee { get type() { return "engineer"; } get bonus() { return 1000; } }
class Salesman extends Employee { get type() { return "salesman"; } get bonus() { return 3000; } }
class Manager extends Employee { get type() { return "manager"; } get bonus() { return 5000; } }

function createEmployee(name, type) {
  switch (type) {
    case "engineer": return new Engineer(name);
    case "salesman": return new Salesman(name);
    case "manager": return new Manager(name);
    default: throw new Error(`unknown type ${type}`);
  }
}

for (const t of ["engineer", "salesman", "manager"]) {
  assert.equal(createEmployee("Ana", t).bonus, new EmployeeBefore("Ana", t).bonus);
}
const staff = [createEmployee("Ana", "salesman"), createEmployee("Bo", "manager")];
console.log(staff.map((e) => `${e.name} ${e.constructor.name} ${e.bonus}`));
```

```output
[ 'Ana Salesman 3000', 'Bo Manager 5000' ]
```

The bonus amounts are made up for the example.

| Language | What the example shows |
|---|---|
| JavaScript | a type string replaced with three subclasses and a factory function, as above |
| Python | the same change; the factory function looks up the class in a dictionary in place of a `switch` |
| Go | not inheritance: each kind becomes its own type, and an interface `Employee` gives the shared methods |

## When to use

- **The same test of a type code in many places.** One subclass for each kind removes the tests.
- **Fields or methods that only some kinds use.** They can move down into the subclass for those kinds. See [[push-down-field|Push Down Field]].
- **A kind that changes at run time.** Use the indirect form, with a type object.

## Pitfalls

**The kind changes.** An object cannot change its class in most languages. If an employee can become a manager, direct subclasses do not work. Use the indirect form.

**One axis only.** A class can have one set of subclasses. If the objects vary in two ways, such as the type and the contract of an employee, use subclasses for one and delegates for the other. See [[replace-subclass-with-delegate|Replace Subclass with Delegate]].

**Too little difference.** If the subclasses only differ in a constant, a field is simpler. That is the inverse, [[remove-subclass|Remove Subclass]].

**Go and Rust.** These languages have no class inheritance. The Go form is one type for each kind and an interface for the shared methods. The Rust form is an enum with one variant for each kind and a `match` in each method, or one struct for each kind and a trait.

## Tools that do it

No IDE has one action for the whole refactoring. The IDE can do some of the steps, such as the encapsulation of the field and the rename of the factory function.

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tag dealing-with-inheritance.[^2] In the second edition it is in Chapter 12, Dealing with Inheritance.[^3]

## References
