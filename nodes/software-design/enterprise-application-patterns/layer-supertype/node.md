---
tag: B6CMAT
kind: pattern
slug: layer-supertype
name: Layer supertype
aliases: [Layer Supertype, base class of a layer, domain object base class]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A layer supertype is one base type for all the types of a layer, such as all the domain objects, so that their shared behaviour, like the identity and the equality, is written once.
family: architectural
polarity: positive
intent: "Give all the types of a layer one supertype that holds the behaviour they share."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [identity-field, domain-model, active-record, template-method-pattern, service-layer, value-equality-and-identity, enterprise-application]
sources:
  - url: https://martinfowler.com/eaaCatalog/layerSupertype.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Layer Supertype
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Layer Supertype
  - url: https://guides.rubyonrails.org/active_record_basics.html
    title: Rails Guides, Active Record Basics
    locator: Creating Active Record Models
  - url: https://docs.djangoproject.com/en/5.2/topics/db/models/
    title: Django 5.2 documentation, Models
    locator: Abstract base classes
---

## Intent

A layer supertype is "A type that acts as the supertype for all types in its layer."[^1] The behaviour that all the types of the layer share is written once, in the supertype.

## Problem

The types of one layer often need the same code. All the domain objects have an ID and compare by it. All the controllers check the user. All the data mappers open a connection in the same way. The catalog says: "It's not uncommon for all the objects in a layer to have methods you don't want to have duplicated throughout the system."[^1]

If each class writes this code again, the copies drift apart. One class forgets a method, another class does it in a slightly different way, and a fix must be made in each copy.

## Solution

"You can move all of this behavior into a common Layer Supertype."[^1] The parts are:

- **The supertype.** One abstract base class, or in languages without inheritance, a trait or an embedded struct, for one layer. It holds the shared fields and methods.
- **The types of the layer.** Each one extends the supertype and adds only what is its own.
- **Hooks.** Where the shared code needs a part that differs for each type, the supertype calls a method that each subclass overrides, as in the [[template-method-pattern]].

A common use is a supertype for the domain objects that holds the [[identity-field]].[^2] It can also define the equality by ID. Each layer has its own supertype, so the domain objects and the data mappers do not share one.

```text
                 DomainObject           id, equality by kind and id, check() calls rules()
                 /          \
          Customer          Product     each gives only its own fields and rules()
```

In the following program, the supertype defines the equality and the hash once. Two objects for the same row are equal, and objects of two types with the same ID are not.

```python run id=one-equality-for-the-layer
class DomainObject:  # the layer supertype
    def __init__(self, id):
        self.id = id

    def __eq__(self, other):
        return type(self) is type(other) and self.id == other.id

    def __hash__(self):
        return hash((type(self), self.id))


class Customer(DomainObject):
    pass


class Invoice(DomainObject):
    pass


print("same customer row, two objects:", Customer(1) == Customer(1))
print("in one set:", len({Customer(1), Customer(1), Invoice(1)}))
```

```output
same customer row, two objects: True
in one set: 2
```

## Example

Each example file has a supertype for the domain objects, with an ID, the equality by the type and the ID, and a `check` method that collects the broken rules. `Customer` and `Product` give only their fields and their rules. The program checks a valid customer, a valid product and a product with an empty name and a negative price. Then it compares two objects for the same customer row, which are equal, and a customer and a product with the same ID, which are not. In Go and Rust, which have no inheritance, the supertype is an embedded struct with functions on an interface, and a trait with default methods.

## Consequences

**Benefits.**

- **No duplicated code.** The shared behaviour is in one place, so a fix or a change is made once.
- **A uniform layer.** All the types of the layer behave in the same way, and code that works on the layer can use the supertype.
- **A place for the rules of the layer.** New behaviour for all the domain objects, such as a change log, goes into the supertype.

**Liabilities.**

**Without a supertype, a class forgets the shared code.** In the following program, each class writes its own equality, and `Invoice` forgot it. Two objects for the same invoice row are not equal.

```python wrong id=each-class-writes-its-own-equality
class Customer:
    def __init__(self, id):
        self.id = id

    def __eq__(self, other):  # each class writes its own equality
        return isinstance(other, Customer) and self.id == other.id

    def __hash__(self):
        return hash(self.id)


class Invoice:  # this class forgot it
    def __init__(self, id):
        self.id = id


print("same customer row loaded twice is equal:", Customer(1) == Customer(1))
print("same invoice row loaded twice is equal:", Invoice(1) == Invoice(1))
```

```output
same customer row loaded twice is equal: True
same invoice row loaded twice is equal: False
```

Move the equality into one supertype, and let `Customer` and `Invoice` extend it.

**A base class that grows too much.** It is easy to put into the supertype code that only some of the types need. Keep only the behaviour that all the types of the layer share.

**One base class only.** In a language with single inheritance, the types of the layer cannot extend another class. A framework base class can take that place, and then the supertype must extend it.

## When not to use it

**When the types share only a little code.** A helper function, or a small mixin, is simpler than a base class for all the types.

**When composition fits better.** If the shared code is a service, such as logging or access to the database, give the objects that service and do not make it a base class.

## Variants and related patterns

**Framework base classes.** Many frameworks give the application a supertype for one layer. In Rails, "ApplicationRecord is the base class for all Active Record models in your app."[^3] It inherits from `ActiveRecord::Base`, so each model of the application gets the behaviour of [[active-record]].

**Abstract models.** In Django, "Abstract base classes are useful when you want to put some common information into a number of other models."[^4] An abstract model gives its fields to each subclass and has no table of its own.

**Related patterns.** The supertype often uses the [[template-method-pattern]] for its hooks. In a [[domain-model]], the supertype of the domain objects holds the [[identity-field]].

## Known uses

- Rails applications have an `ApplicationRecord` class that all their models extend.[^3]
- Django models extend `django.db.models.Model`, and abstract models add a supertype of the application.[^4]

## References
