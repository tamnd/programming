---
tag: 92KTZK
kind: pattern
slug: pessimistic-offline-lock
name: Pessimistic offline lock
aliases: [Pessimistic Offline Lock, pessimistic locking, check-out lock, lock-modify-unlock, reserved checkout]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A pessimistic offline lock makes a business transaction take a lock on the data before it uses the data, so only one business transaction at a time can edit it.
family: architectural
polarity: positive
intent: "Prevent conflicts between concurrent business transactions by allowing only one business transaction at a time to access data."
history:
  year: 2002
  people: [David Rice]
  note: David Rice wrote the pattern for Fowler's Patterns of Enterprise Application Architecture (2002), in the chapter on offline concurrency patterns.
see_also: [optimistic-offline-lock, coarse-grained-lock, implicit-lock, read-write-lock, mutual-exclusion, deadlock, unit-of-work, lock-a-file]
sources:
  - url: https://martinfowler.com/eaaCatalog/pessimisticOfflineLock.html
    title: David Rice, Catalog of Patterns of Enterprise Application Architecture, Pessimistic Offline Lock
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 16, Offline Concurrency Patterns, Pessimistic Offline Lock
  - url: https://www.rfc-editor.org/rfc/rfc4918
    title: RFC 4918, HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV), June 2007
    locator: section 1 Introduction; section 6.2 Exclusive vs. Shared Locks; section 6.6 Lock Timeout
  - url: https://api.rubyonrails.org/classes/ActiveRecord/Locking/Pessimistic.html
    title: Ruby on Rails API, ActiveRecord::Locking::Pessimistic
    locator: description
  - url: https://svnbook.red-bean.com/en/1.7/svn.advanced.locking.html
    title: Version Control with Subversion, for Subversion 1.7, Locking
    locator: introduction; Breaking and Stealing Locks
---

## Intent

A pessimistic offline lock "Prevents conflicts between concurrent business transactions by allowing only one business transaction at a time to access data."[^1] A business transaction takes a lock on a record before it starts to edit the record, and keeps the lock until it ends.

## Problem

A business transaction, such as the edit of a customer in a web form, often runs across many requests. One open system transaction of the database for the whole business transaction does not work well: "Sadly, however, this doesn't always work well because transaction systems aren't geared to work with long transactions."[^1] So the application uses many short system transactions, and it must control the concurrent access itself.

An [[optimistic-offline-lock]] finds a conflict when a user saves. If conflicts are frequent, this is a bad experience. The user does all the work of the edit and finds only at the end that the save fails. A long edit, such as an insurance claim or a large order, can waste much time.

## Solution

"Pessimistic Offline Lock prevents conflicts by avoiding them altogether."[^1] "It forces a business transaction to acquire a lock on a piece of data before it starts to use it, so that, most of the time, once you begin a business transaction you can be pretty sure you'll complete it without being bounced by concurrency control."[^1]

The parts are:

- **The lock type.** An exclusive write lock lets only one business transaction edit the record. An exclusive read lock also stops the others from reading it. A read/write lock lets many readers in, or one writer.[^2]
- **The lock manager.** One object that the application calls to acquire and to release the locks. It refuses a lock that another owner holds, and it does not wait, so a business transaction cannot wait for a lock forever.
- **The lock table.** The locks must last across requests, and often across the processes of the application, so the lock manager keeps them in a table of the database. The resource is the primary key, so two sessions cannot both insert the same lock.
- **The owner.** Usually the session or the business transaction. When it ends, the lock manager releases all its locks.
- **The timeout.** A user can close the browser and never come back. The lock manager frees a lock that is older than a limit.

```text
  time    session   call to the lock manager       lock table
  10:00   ann       acquire customer 1: granted    customer 1 -> ann
  10:05   bob       acquire customer 1: refused    customer 1 -> ann
  10:12   ann       release all                    (empty)
  10:15   bob       acquire customer 1: granted    customer 1 -> bob
```

In the following program, the check of the lock table and the take of the lock are one step, so only the first session gets the lock.

```python run id=lock-manager
locks = {}  # resource -> owner; in a real application, a table in the database


def acquire(resource, owner):
    holder = locks.setdefault(resource, owner)  # one step: check and take
    return "granted" if holder == owner else f"refused, held by {holder}"


def release(resource, owner):
    if locks.get(resource) == owner:
        del locks[resource]


print("ann:", acquire("customer 1", "ann"))
print("bob:", acquire("customer 1", "bob"))
release("customer 1", "ann")
print("bob:", acquire("customer 1", "bob"))
```

```output
ann: granted
bob: refused, held by ann
bob: granted
```

## Example

Each example file has a lock manager with a lock table. Each lock has a resource, an owner and the time when the owner took it, and the timeout is 30 minutes. Ann opens customer 1 for edit at 10:00 and gets the lock. Bob tries at 10:05 and is refused. Ann saves at 10:12, and that releases the lock of Ann. Bob then gets the locks on customer 1 and on order 9, but Bob closes the browser and does not come back. Carol is refused at 10:40, because the lock of Bob is 25 minutes old. At 10:50, the lock is 35 minutes old, so the lock manager deletes it, and Carol gets the lock.

## Consequences

**Benefits.**

- **No lost work.** A user who has the lock can finish the edit, and the save does not fail because of another session.
- **Early refusal.** A user who cannot get the lock knows it at the start, before any work is done.
- **Simple saves.** The save does not need a version check for the data that the lock covers.

**Liabilities.**

**The check and the take must be one step.** If the lock manager first checks that a lock is free and then takes it in a later step, two sessions can both see the lock as free. In the following program, both sessions think that they hold the lock.

```python wrong id=check-then-take
locks = {}


def is_free(resource):  # step 1, in one system transaction
    return resource not in locks


def take(resource, owner):  # step 2, in a later system transaction
    locks[resource] = owner


# Wrong: the check and the take are two steps, and two sessions run them at the same time.
ann_sees_free = is_free("customer 1")
bob_sees_free = is_free("customer 1")
if ann_sees_free:
    take("customer 1", "ann")
if bob_sees_free:
    take("customer 1", "bob")

print("ann's session thinks it holds the lock:", ann_sees_free)
print("bob's session thinks it holds the lock:", bob_sees_free)
print("the lock table says:", locks["customer 1"])
```

```output
ann's session thinks it holds the lock: True
bob's session thinks it holds the lock: True
the lock table says: bob
```

Make the take one atomic step, such as an insert into a table with the resource as its primary key, or one `setdefault` on a shared map that a single process owns.

**Less concurrency.** Only one user at a time can edit a record. If the users often edit the same data, they must wait for each other.

**Abandoned locks.** A user can leave a lock in place for hours. A timeout frees it, but the timeout is a balance: a short timeout takes the lock from a slow user, and a long timeout keeps the others out. WebDAV has the same problem, and RFC 4918 says that "A lock MAY have a limited lifetime."[^3]

**Lock management.** The application must know the owner of each lock, release the locks at the end of each business transaction, and give the users a way to see and to break the locks.

**Deadlock.** If a business transaction waits for a lock, two of them can each wait for a lock that the other holds. A lock manager that refuses at once, and does not wait, avoids this [[deadlock]].[^2]

## When not to use it

**When conflicts are rare.** An [[optimistic-offline-lock]] gives more concurrency and needs no lock table. Rice suggests trying it first, and using the pessimistic lock when the conflicts of the optimistic lock cost too much.[^1]

**When the edit is short.** If the read and the write happen in one request, the locks of the database, in one system transaction, are enough.

**When the users cannot be trusted to release the locks.** Public applications, where many users open records and leave, fill the lock table with abandoned locks.

## Variants and related patterns

**Shared and exclusive locks.** RFC 4918 for WebDAV has exclusive locks and shared locks: "The most basic form of lock is an exclusive lock."[^3] "A shared lock allows multiple principals to receive a lock."[^3] A [[read-write-lock]] is the same idea inside one process.

**Database row locks.** A database can lock a row in one system transaction with `SELECT ... FOR UPDATE`. In Rails, "Locking::Pessimistic provides support for row-level locking using SELECT … FOR UPDATE and other lock types."[^4] Such a lock ends with the system transaction, so it is not an offline lock. It is useful for the short step that takes an offline lock.

**Related patterns.** An [[optimistic-offline-lock]] finds conflicts in place of preventing them. A [[coarse-grained-lock]] takes one lock for a group of objects. An [[implicit-lock]] makes the shared code take the locks, so that no script forgets one. Inside one process, the same idea is [[mutual-exclusion]].

## Known uses

- WebDAV. The `LOCK` method of RFC 4918 gives a client a write lock on a resource. The RFC describes locking as the ability to "keep more than one person from working on a document at the same time", which prevents lost updates.[^3] Each lock has a timeout: "Timeout is measured in seconds remaining until lock expiration."[^3]
- Subversion. The `svn lock` command gives the lock-modify-unlock model for files that cannot be merged, such as images. "Subversion's locking feature exists ultimately to minimize wasted time and effort."[^5] It works "By allowing a user to programmatically claim the exclusive right to change a file in the repository".[^5] The locks can be broken: "in Subversion's default configuration state, locks can be released not only by the person who created them, but by anyone."[^5]
- File locks of the operating system, such as the locks of [[lock-a-file]], which keep one writer on a file.

## References
