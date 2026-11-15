---
tag: 3ZYVBP
kind: pattern
slug: special-case
name: Special case
aliases: [Special Case, special case subclass]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A special case is a subclass, or another implementation of the same interface, that gives the behavior for a particular case, such as a missing customer, in place of null.
family: architectural
polarity: positive
intent: "Return an object with the special behavior in place of null or an odd value."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [null-object-pattern, introduce-special-case, null-references, value-object, domain-model, test-double]
sources:
  - url: https://martinfowler.com/eaaCatalog/specialCase.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Special Case
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Special Case
  - url: https://docs.djangoproject.com/en/5.2/ref/contrib/auth/
    title: Django documentation, django.contrib.auth
    locator: AnonymousUser object
---

## Intent

A special case is "A subclass that provides special behavior for particular cases."[^1] When a finder has no normal object to return, it returns a special case object. The special case has the same interface as the normal object, so the callers need no test for null.

## Problem

The catalog says: "Nulls are awkward things in object-oriented programs because they defeat polymorphism."[^1] A caller can call a method on any object of the right type. But if the variable can be null, the call fails when the program runs.

So each caller must test for null and decide what to do. Often each caller does the same thing, for example it shows a default name. The test and the default are then copied in many places, and one place that forgets the test fails. Null is not the only case. The catalog also names infinity, "which has special rules for things like addition that break the usual invariants of real numbers".[^1]

## Solution

The catalog says: "Instead of returning null, or some odd value, return a Special Case that has the same interface as what the caller expects."[^1] The parts are:

- **The normal type.** It is a class or an interface that the callers use, such as `Customer`.
- **The special case.** It is a subclass of the normal class, or another implementation of the interface, such as `UnknownCustomer`. It gives the behavior that the callers would otherwise write after a test for null: a default name, an empty list, zero.
- **The source.** A finder or a factory returns the special case in place of null.
- **A way to ask, if needed.** A method such as `is_known()` lets the few callers that must act differently find the special case without a type test.

```text
  find_customer(1) -> Customer(Ann)          .bill_line() -> "customer 1: Ann, ..."
  find_customer(9) -> UnknownCustomer(9)     .bill_line() -> "customer 9: current resident, ..."
  callers: c.bill_line()                     (no test for None)
```

In the following program, a visitor that has not signed in gets an `AnonymousUser`, which is a special case of `User`.

```python run id=anonymous-user
class User:
    is_authenticated = True

    def __init__(self, name):
        self.name = name

    def greeting(self):
        return f"hello, {self.name}"


class AnonymousUser(User):  # the special case for a visitor who has not signed in
    is_authenticated = False

    def __init__(self):
        super().__init__("")

    def greeting(self):
        return "hello, guest"


sessions = {"s1": User("Ann")}


def current_user(session_id):
    return sessions.get(session_id, AnonymousUser())


for s in ["s1", "s2"]:
    user = current_user(s)
    print(user.greeting(), "| signed in:", user.is_authenticated)
```

```output
hello, Ann | signed in: True
hello, guest | signed in: False
```

## Example

Each example has a normal `Customer` and an `UnknownCustomer` special case with the same methods. The finder returns the special case for an ID that has no record. The program finds customers 1, 2 and 9, and prints a bill line for each with no test for null. Customer 9 has no record, so its line shows "current resident" and the basic plan. Last, the program counts the known customers with `is_known`.

## Consequences

**Benefits.**

- **No repeated tests for null.** The default behavior is in one class, and not in each caller.
- **No errors from a missing test.** The finder never returns null, so no caller can forget the test.
- **Polymorphism works again.** The callers call the same methods on each object, and each class gives its own behavior.

**Liabilities.**

**With null, one caller that forgets the test fails.** In the following program, the finder returns `None` for a customer with no record. The code for the name tests for `None`, but the code for the plan forgets it.

```python wrong id=one-caller-forgets-the-none-test
class Customer:
    def __init__(self, name, plan):
        self.name, self.plan = name, plan


ROWS = {1: Customer("Ann", "premium")}


def find_customer(customer_id):
    return ROWS.get(customer_id)  # None when there is no record


def name_on_bill(customer_id):
    c = find_customer(customer_id)
    return "current resident" if c is None else c.name


def plan_on_bill(customer_id):
    return find_customer(customer_id).plan  # this place forgets the test for None


print("name:", name_on_bill(9))
try:
    print("plan:", plan_on_bill(9))
except AttributeError as e:
    print("AttributeError:", e)
```

```output
name: current resident
AttributeError: 'NoneType' object has no attribute 'plan'
```

Return an `UnknownCustomer` with the name "current resident" and the plan "basic", and remove the tests for `None` from the callers.

**Errors can hide.** If the missing object is an error, such as a broken foreign key, a special case lets the program continue with wrong data. The program must decide which missing cases are normal.

**One more class for each case.** Each special case is a class to keep in step with the normal class. When the normal class gets a new method, each special case needs it too.

**The callers can still need to know.** Some code must act differently for the special case, for example a report that counts only the known customers. The interface then needs a method such as `is_known`.

## When not to use it

**When the callers must handle the missing case themselves.** If each caller does something different, there is no default behavior to put in a class. An optional type, such as `Optional` in Java or `Option` in Rust, makes each caller handle the case.

**When the missing value is an error.** Raise an exception or return an error. A special case would hide the problem.

## Variants and related patterns

**Null object.** The [[null-object-pattern]] is a special case for a missing object whose methods do nothing. The book names Null Object as a kind of special case.[^2]

**Several special cases.** One type can have more than one special case, for example one for a customer with no record and one for a customer whose record is not yet loaded. Each one has its own behavior.

**Refactoring.** The refactoring [[introduce-special-case]] replaces the repeated tests for a special value with a special case object.

**Related ideas.** [[null-references]] and nullability describe the problem that the pattern solves. A special case with no state of its own can be one shared instance, because no caller can change it. A [[test-double]] is a different idea: it replaces an object in a test, not in the program.

## Known uses

- Django gives an `AnonymousUser` for a visitor that has not signed in: "django.contrib.auth.models.AnonymousUser is a class that implements the django.contrib.auth.models.User interface".[^3] For example, its `is_authenticated` is False and its `id` is always None.[^3]

## References
