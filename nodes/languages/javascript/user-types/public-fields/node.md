---
tag: XSTH0B
kind: feature
owner: javascript
slug: public-fields
name: Public fields
aliases: [public class fields, class fields, instance fields, field declarations, field initializers, arrow function fields]
area: languages/javascript/user-types
level: core
summary: >-
  A public field in a JavaScript class body defines an own, enumerable property on each new instance. Its initializer runs once per instance, before the constructor body or right after super() returns.
grammar: null
spec: {standard: "ECMA-262, 17th edition (ECMAScript 2026)", url: "https://tc39.es/ecma262/2026/multipage/abstract-operations.html#sec-definefield", locator: "7.3.32 DefineField; 7.3.33 InitializeInstanceElements"}
see_also: [javascript/classes, javascript/constructor, javascript/instance-methods, javascript/property-descriptors, javascript/defining-getters-and-setters]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Public_class_fields
  title: MDN Web Docs, JavaScript Reference, Public class fields
  locator: the introduction; Syntax; Description; Examples
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/abstract-operations.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 7
  locator: 7.3.32 DefineField; 7.3.33 InitializeInstanceElements
  accessed: 2026-11-18
- url: https://tc39.es/ecma262/2026/multipage/ordinary-and-exotic-objects-behaviours.html
  title: ECMAScript 2026 Language Specification (ECMA-262, 17th edition), clause 10
  locator: 10.2.2 [[Construct]]
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/this
  title: MDN Web Docs, JavaScript Reference, this
  locator: Class context
  accessed: 2026-11-18
- url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
  title: TC39, Finished Proposals
  locator: Class fields, Expected Publication Year
  accessed: 2026-11-18
- url: https://docs.python.org/3.14/reference/datamodel.html
  title: The Python Language Reference, Data model, Python 3.14
  locator: 3.2.11 Class instances
  accessed: 2026-11-18
---

## Synopsis

```javascript run id=basics
class Counter {
  count = 0;
  label;
  step = 1;
  increment() {
    this.count += this.step;
    return this.count;
  }
}
const c = new Counter();
console.log(c.increment(), c.label, Object.keys(c));
const d = Object.getOwnPropertyDescriptor(c, "count");
console.log(d.writable, d.enumerable, d.configurable);
```

```output for=basics
1 undefined [ 'count', 'label', 'step' ]
true true true
```

| Form | Meaning |
|---|---|
| `name;` | own property `name`, value undefined |
| `name = expr;` | own property, `expr` evaluated for each instance with `this` the instance |
| `[keyExpr] = expr;` | computed name, `keyExpr` evaluated once when the class is defined |
| `name = () => this.x;` | an arrow function per instance, `this` fixed to that instance |

## Semantics

**r1.** "Public fields are writable, enumerable, and configurable properties defined on each class instance or class constructor."[^1] ECMA-262 creates each one as a data property of the new object: "Perform ? CreateDataPropertyOrThrow(receiver, fieldName, initValue)."[^2] With no initializer, "Let initValue be undefined."[^2] Example: [`basics`](#basics).

**r2.** "Public instance fields are added to the instance either at construction time in the base class (before the constructor body runs), or just after super() returns in a subclass."[^1] In [[Construct]], a base class runs "Let initializeResult be Completion(InitializeInstanceElements(thisArgument, F))." and only then "Let result be Completion(OrdinaryCallEvaluateBody(F, argumentsList))."[^3] So the constructor can read the fields. Example: [`timing`](#timing).

**r3.** "Fields are added one-by-one. Field initializers can refer to field values above them, but not below them."[^1] ECMA-262: "For each element fieldRecord of fields, doPerform ? DefineField(O, fieldRecord)."[^2] Example: [`timing`](#timing).

**r4.** "The field initializer expression is evaluated each time a new instance is created."[^1] "In the field initializer, this refers to the class instance under construction".[^1] So an object or array initializer gives each instance its own value, and "arrow functions in field initializers are bound to the instance for instance fields".[^4] Example: [`perinstance`](#perinstance).

**r5.** "Computed field names are only evaluated once, at class definition time. This means that each class always has a fixed set of field names, and two instances cannot have different field names via computed names."[^1] Example: [`computed`](#computed).

**r6.** "Because class fields are added using the [[DefineOwnProperty]] semantic (which is essentially Object.defineProperty() ), field declarations in derived classes do not invoke setters in the base class. This behavior differs from using this.field = … in the constructor."[^1] Example: [`setter`](#setter).

**r7.** A field cannot read constructor arguments: "Class fields cannot depend on arguments of the constructor".[^1] Set such values in the constructor. Also, "The name of a class field (static or instance) cannot be constructor ."[^1]

## Examples

When fields exist (rules r2 and r3).

```javascript run id=timing
class C {
  a = 1;
  b = this.c;
  c = this.a + 1;
  constructor() {
    console.log("in constructor:", this.a, this.b, this.c);
  }
}
new C();
```

```output for=timing
in constructor: 1 undefined 2
```

`b` reads `this.c` before `c` is defined, so it gets undefined.

One initializer run per instance (rule r4).

```javascript run id=perinstance
let made = 0;
class Item {
  id = ++made;
  tags = [];
  getId = () => this.id;
}
const a = new Item();
const b = new Item();
a.tags.push("red");
console.log(a.id, b.id, b.tags, a.tags === b.tags);
const loose = a.getId;
console.log(loose(), Object.hasOwn(a, "getId"));
```

```output for=perinstance
1 2 [] false
1 true
```

The arrow function keeps its instance even when called on its own, but each instance gets its own copy.

A computed field name (rule r5).

```javascript run id=computed
let n = 0;
const key = () => `field${++n}`;
class C {
  [key()] = "x";
}
console.log(Object.keys(new C()), Object.keys(new C()), n);
```

```output for=computed
[ 'field1' ] [ 'field1' ] 1
```

## Versions

| Feature | Edition |
|---|---|
| public instance fields | ECMAScript 2022[^5] |

The TC39 list of finished proposals gives 2022 for Class fields.[^5]

## Pitfalls

### Expecting a field to call an inherited setter

```javascript wrong id=setter
class Base {
  set value(v) {
    console.log("setter got", v);
  }
}
class WithField extends Base {
  value = 1;
}
class WithAssignment extends Base {
  constructor() {
    super();
    this.value = 2;
  }
}
const f = new WithField();
const g = new WithAssignment();
console.log(Object.hasOwn(f, "value"), Object.hasOwn(g, "value"));
```

```output for=setter
setter got 2
true false
```

The field defines an own property and skips the setter; the assignment calls the setter (rule r6). Assign in the constructor when the setter must run.

### Reading a subclass field in the base constructor

```javascript wrong id=basector
class Base {
  constructor() {
    console.log("Base sees", this.size);
  }
}
class Box extends Base {
  size = 10;
}
console.log(new Box().size);
```

```output for=basector
Base sees undefined
10
```

MDN: "because instance fields of a derived class are defined after super() returns, the base class's constructor does not have access to the derived class's fields."[^1] Pass the value to `super()` as an argument instead (rule r2).

## Comparison

In Python, a name assigned in the class body is a class attribute, stored once. An instance looks in its own dictionary first: "When an attribute is not found there, and the instance’s class has an attribute by that name, the search continues with the class attributes."[^6] So `tags = []` in a Python class body is one list for all instances. In JavaScript, `tags = [];` in a class body is evaluated again for each instance and gives each its own array (rule r4).

## References
