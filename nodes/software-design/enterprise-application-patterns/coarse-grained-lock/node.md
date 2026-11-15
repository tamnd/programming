---
tag: 85CQYF
kind: pattern
slug: coarse-grained-lock
name: Coarse-grained lock
aliases: [Coarse-Grained Lock, shared version, shared lock, root lock, group lock]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  A coarse-grained lock is one lock for a group of related objects, such as a customer and its addresses. A change to any member of the group takes the lock of the group.
family: architectural
polarity: positive
intent: "Lock a set of related objects with a single lock."
history:
  year: 2002
  people: [David Rice, Matt Foemmel]
  note: David Rice and Matt Foemmel wrote the pattern for Fowler's Patterns of Enterprise Application Architecture (2002), in the chapter on offline concurrency patterns.
see_also: [optimistic-offline-lock, pessimistic-offline-lock, implicit-lock, unit-of-work, dependent-mapping, foreign-key-mapping, lazy-load, data-mapper]
sources:
  - url: https://martinfowler.com/eaaCatalog/coarseGrainedLock.html
    title: David Rice and Matt Foemmel, Catalog of Patterns of Enterprise Application Architecture, Coarse-Grained Lock
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 16, Offline Concurrency Patterns, Coarse-Grained Lock
  - url: https://jakarta.ee/specifications/persistence/3.2/jakarta-persistence-spec-3.2
    title: Jakarta Persistence 3.2 specification
    locator: 3.5.4.1. OPTIMISTIC, OPTIMISTIC_FORCE_INCREMENT; 3.5.4.2. PESSIMISTIC_READ, PESSIMISTIC_WRITE, PESSIMISTIC_FORCE_INCREMENT
---

## Intent

A coarse-grained lock "Locks a set of related objects with a single lock."[^1] The objects of a group share one version or one lock. A session that changes any member of the group must take that one lock.

## Problem

"Objects can often be edited as a group. Perhaps you have a customer and its set of addresses. If so, when using the application it makes sense to lock all of these items if you want to lock any one of them."[^1]

A lock for each object makes this hard. "Having a separate lock for individual objects presents a number of challenges. First, anyone manipulating them has to write code that can find them all in order to lock them."[^1] With an [[optimistic-offline-lock]], the session must load each member to check its version. "And with Pessimistic Offline Lock a large lock set is a management headache and increases lock table contention."[^1]

A rule of the group can also break. For example, a customer can have at most one primary address. If each address has its own version, two sessions can each make a different address the primary one, and each save changes a different row. Each check passes, and the group breaks the rule.

## Solution

"A Coarse-Grained Lock is a single lock that covers many objects. It not only simplifies the locking action itself but also frees you from having to load all the members of a group in order to lock them."[^1]

There are two usual designs:[^2]

- **The shared version.** All the members of the group point to one version object, or to one version row in the database. For an optimistic lock, a save of any member checks and increases that one version. For a pessimistic lock, the lock is on the shared version.
- **The root lock.** The group has a root, such as the customer for its addresses. A session that changes a member finds the root and locks the root. The members have no lock of their own.

```text
  customer 1 ----------------+
  address 1 (customer 1) ----+---> version of the group "customer 1": 1
  address 2 (customer 1) ----+

  bob saves a change to customer 1: check version 1, set version 2
  ann saves a change to address 2:  check version 1, the version is 2: conflict
```

In the following program, the customer and an address share one version object. The save of the customer by Bob changes the version, so the save of the address by Ann fails.

```python run id=shared-version
class Version:
    def __init__(self):
        self.value = 1


shared = Version()  # one version object for the whole group
customer = {"name": "Acme Ltd", "version": shared}
address = {"street": "9 Oak Road", "version": shared}


def save(member, read_version):
    if member["version"].value != read_version:
        return "conflict"
    member["version"].value += 1  # a change to any member changes the version of all of them
    return "ok"


ann_read = bob_read = shared.value  # both sessions read version 1 of the group
print("bob saves the customer:", save(customer, bob_read))
print("ann saves an address:", save(address, ann_read))
print("version of the group:", shared.value)
```

```output
bob saves the customer: ok
ann saves an address: conflict
version of the group: 2
```

## Example

Each example file has a customer with two addresses. The customer row holds the name of its lock, and one table holds the version of each lock. Each session loads the whole group with its version. Ann makes 1 Elm Street the primary address, and Bob makes 9 Oak Road the primary address. Each session changes both addresses, because the rule of the group allows at most one primary address. The save of Ann checks the version of the group and succeeds, and the version becomes 2. The save of Bob checks the version and finds a conflict. At the end, the database has one primary address, and each save needed only one check.

## Consequences

**Benefits.**

- **One lock to take.** The code locks the group with one call, and does not need to find all the members.
- **Fewer loads.** An optimistic check reads one version, not the version of each member.
- **Rules of the group stay true.** Two sessions cannot change two members of the same group at the same time, so a rule across the members, such as one primary address, holds.
- **A smaller lock table.** A pessimistic lock manager keeps one lock for each group.

**Liabilities.**

**Locks on single objects miss a conflict of the group.** In the following program, each address has its own version. Each session changes one row, and each check passes, so the customer gets two primary addresses.

```python wrong id=a-version-for-each-member
# Wrong: each address has its own version, so nothing locks the group.
addresses = {1: {"primary": False, "version": 1}, 2: {"primary": False, "version": 1}}


def save(address_id, primary, read_version):
    row = addresses[address_id]
    if row["version"] != read_version:
        return "conflict"
    addresses[address_id] = {"primary": primary, "version": read_version + 1}
    return "ok"


# Both sessions read both addresses, and both are not primary.
# Each session sets one address as primary and saves only the row that it changed.
print("ann sets address 1:", save(1, True, 1))
print("bob sets address 2:", save(2, True, 1))
print("primary addresses:", [a for a, r in addresses.items() if r["primary"]])
```

```output
ann sets address 1: ok
bob sets address 2: ok
primary addresses: [1, 2]
```

Give the customer and its addresses one shared version, and make each save of a member check it.

**More conflicts.** Two users who change different members of the same group now conflict, even when their changes do not touch each other. A group that is too large makes the users wait or fail often.

**Finding the root.** With a root lock, the code must go from a member to its root before it can lock. In a [[data-mapper]], this can mean more loads, or a [[lazy-load]] of the root.[^2]

**Every save must use the shared lock.** A save of a member that skips the version of the group gives back the bug of the wrong program. An [[implicit-lock]] puts the check in shared code.

## When not to use it

**When the objects change on their own.** If the members have no rule in common and users often edit different members, separate locks give more concurrency.

**When the group is not clear.** A coarse-grained lock needs a clear owner for each member, such as the customer of an address. Objects that many groups share do not fit one group.

## Variants and related patterns

**Shared optimistic version.** The members point to one version object, as in the example.[^2]

**Shared pessimistic lock.** The lock manager locks the shared version, so one lock covers the group.[^2]

**Root optimistic lock.** The session increases the version of the root when it changes a member. Jakarta Persistence has the lock mode `OPTIMISTIC_FORCE_INCREMENT`, where the entity manager "must also force an update (increment) to the entity’s version column."[^3] A lock with this mode on the root changes its version when only a member changed.

**Related patterns.** The group is often an [[aggregate]], with the root as its entry point. A [[dependent-mapping]] or a [[foreign-key-mapping]] holds the link from each member to its root. The lock itself is an [[optimistic-offline-lock]] or a [[pessimistic-offline-lock]]. A [[unit-of-work]] can check the version of the group once in each commit.

## Known uses

- Jakarta Persistence, where `OPTIMISTIC_FORCE_INCREMENT` and `PESSIMISTIC_FORCE_INCREMENT` on the root entity force a change of its version.[^3]

## References
