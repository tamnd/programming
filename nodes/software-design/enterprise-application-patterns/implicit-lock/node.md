---
tag: XWFNBA
kind: pattern
slug: implicit-lock
name: Implicit lock
aliases: [Implicit Lock, automatic locking, locking mapper, framework-managed locks]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  An implicit lock puts the code that takes and checks the offline locks into the framework, the mappers or a layer supertype, so that no application script can forget a lock.
family: architectural
polarity: positive
intent: "Allow framework or layer supertype code to acquire offline locks."
history:
  year: 2002
  people: [David Rice]
  note: David Rice wrote the pattern for Fowler's Patterns of Enterprise Application Architecture (2002), in the chapter on offline concurrency patterns.
see_also: [optimistic-offline-lock, pessimistic-offline-lock, coarse-grained-lock, decorator-pattern, data-mapper, unit-of-work, active-record, inversion-of-control]
sources:
  - url: https://martinfowler.com/eaaCatalog/implicitLock.html
    title: David Rice, Catalog of Patterns of Enterprise Application Architecture, Implicit Lock
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 16, Offline Concurrency Patterns, Implicit Lock
  - url: https://jakarta.ee/specifications/persistence/3.2/jakarta-persistence-spec-3.2
    title: Jakarta Persistence 3.2 specification
    locator: 3.5.1. Optimistic Locking
  - url: https://api.rubyonrails.org/classes/ActiveRecord/Locking/Optimistic.html
    title: Ruby on Rails API, ActiveRecord::Locking::Optimistic
    locator: Usage
---

## Intent

An implicit lock "Allows framework or layer supertype code to acquire offline locks."[^1] The application scripts load and save the objects as usual. The shared code below them takes the locks, checks the versions and releases the locks.

## Problem

"The key to any locking scheme is that there are no gaps in its use."[^1] One script that forgets to take a lock, or to check a version, makes the whole scheme useless for the data that the script changes. "Generally, if an item might be locked anywhere it must be locked everywhere."[^1]

The other errors are as bad. A script that does not release its locks keeps the other users out: "Not releasing locks won't corrupt your record data, but it will eventually bring productivity to a halt."[^1] These errors are hard to find, because they show only when two users work at the same time. "Because offline concurrency management is difficult to test, such errors might go undetected by all of your test suites."[^1]

## Solution

"One solution is to not allow developers to make such a mistake. Locking tasks that cannot be overlooked should be handled not explicitly by developers but implicitly by the application."[^1] The places for this code are the framework, the [[layer-supertype]] of the domain objects or the mappers, and generated code.[^1]

The parts are:

- **The locking mapper.** A mapper with the same interface as the plain mapper. Its `find` takes the lock before it loads the object, and its `update` checks the version. It is a [[decorator-pattern]] around the plain mapper.[^2]
- **The business transaction.** The framework creates it for each business transaction. It gives the scripts only the locking mapper, and it releases all the locks at the end, also when the business transaction fails.
- **The lock manager or the version check.** The lock itself is a [[pessimistic-offline-lock]] or an [[optimistic-offline-lock]]. The implicit lock only decides where the calls are.

```text
  application script     customer = tx.customers.find(1)    (no lock call)
          |
  locking mapper         lock manager: acquire "customer 1"
          |              then the plain mapper: find(1)
  plain mapper           SELECT ... FROM customer WHERE id = 1
          |
  business transaction   at the end: lock manager: release all
```

In the following program, `find_for_edit` is the framework code. It takes the lock, so the script `raise_credit` gets a lock without a lock call of its own.

```python run id=mapper-takes-the-lock
locks = {}
table = {1: {"name": "Acme Ltd", "credit": 500}}


def find_for_edit(owner, customer_id):  # framework code: every find for edit takes the lock
    holder = locks.setdefault(customer_id, owner)
    if holder != owner:
        raise PermissionError(f"customer {customer_id} is locked by {holder}")
    return dict(table[customer_id])


def raise_credit(owner, amount):  # application code: it has no lock call
    try:
        row = find_for_edit(owner, 1)
    except PermissionError as e:
        return f"refused, {e}"
    row["credit"] += amount
    table[1] = row
    return "ok"


ann = find_for_edit("ann", 1)
print("ann edits:", ann["name"])
print("bob:", raise_credit("bob", 100))
```

```output
ann edits: Acme Ltd
bob: refused, customer 1 is locked by ann
```

## Example

Each example file has a lock manager, a plain mapper for customers, and a locking mapper that wraps it. The framework gives each business transaction a locking mapper, and releases its locks when it ends. Two scripts use the mapper: one renames a customer across two requests, and one raises the credit. Neither script calls the lock manager. Ann opens customer 1 to rename it, so the mapper takes the lock for Ann. Bob runs the credit script on the same customer and is refused. Ann saves, and the end of the business transaction releases the lock. Bob runs the script again and it succeeds. At the end, both changes are in the database, the mapper took 2 locks, and no lock is left.

## Consequences

**Benefits.**

- **No gaps.** Each load through the mapper takes the lock, so a new script gets the locks with no extra code.
- **Locks are released.** The framework releases the locks at the end of each business transaction, so no script can forget it.
- **Simpler scripts.** The application code shows the business logic and not the locking.
- **One place to change.** A change of the locking strategy, such as a new timeout, goes into one place.

**Liabilities.**

**Explicit locks have gaps.** In the following program, each script takes its own locks. The script that raises the credit was written later, and it has no lock call. It runs while Ann holds the lock, and the save of Ann then writes over its change.

```python wrong id=a-script-forgets-the-lock
locks = {}
table = {1: {"name": "Acme Ltd", "credit": 500}}


def open_for_rename(owner):  # this script takes its lock
    if locks.setdefault(1, owner) != owner:
        raise PermissionError("customer 1 is locked")
    return dict(table[1])


def raise_credit(amount):  # Wrong: this script was written later, and it has no lock call
    row = dict(table[1])
    row["credit"] += amount
    table[1] = row


ann = open_for_rename("ann")  # ann holds the lock and edits the name
raise_credit(100)  # this runs while ann holds the lock
ann["name"] = "Acme Group"
table[1] = ann  # ann saves the copy that ann read
locks.clear()
print("final:", table[1])
print("the credit change is lost:", table[1]["credit"] == 500)
```

```output
final: {'name': 'Acme Group', 'credit': 500}
the credit change is lost: True
```

Put the lock into the mapper that each script uses, and do not give the scripts the plain mapper.

**Hidden locks can surprise.** A developer who does not know that a load takes a lock can load too much and lock out other users, or fail to handle a refused lock. The developers must still know which loads take locks and that a lock can fail.

**Not all locks fit a mapper.** A lock of a group, such as a [[coarse-grained-lock]], or a lock that depends on the use case, can need more knowledge than the mapper has.

## When not to use it

**When the application is small.** With a few scripts and one developer, explicit lock calls are easy to see and to review.

**When the framework already does it.** Many object-relational mappers already check a version column on each save. Use that, and do not add a second layer.

## Variants and related patterns

**Implicit optimistic lock.** The [[layer-supertype]] or the mapper keeps the version of each object that it loads, and puts the version check into each `UPDATE` and `DELETE`. This is the most common form, because the mapper writes all the SQL.[^2]

**Implicit pessimistic lock.** A locking mapper takes the lock in each `find`, as in the example. A pessimistic lock on each load can lock more than the business transaction needs, so the mapper may need a separate find for the cases that only read.

**Related patterns.** The lock itself is an [[optimistic-offline-lock]] or a [[pessimistic-offline-lock]]. The locking mapper is a [[decorator-pattern]] of a [[data-mapper]]. A [[unit-of-work]] is a good place to check the versions and to release the locks at commit. A framework that calls the application code, and not the other way round, is an example of [[inversion-of-control]].

## Known uses

- Jakarta Persistence. "The persistence provider is required to perform optimistic locking automatically for every entity with a version".[^3] The application marks the version field once, and the provider checks it on each update.
- Ruby on Rails. When the table has a `lock_version` column, Active Record checks it on each save: "Each update to the record increments the integer column lock_version and the locking facilities ensure that records instantiated twice will let the last one saved raise a StaleObjectError if the first was also updated."[^4] The application code does not call a lock.

## References
