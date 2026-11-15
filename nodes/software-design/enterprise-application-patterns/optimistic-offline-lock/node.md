---
tag: 2A1PFZ
kind: pattern
slug: optimistic-offline-lock
name: Optimistic offline lock
aliases: [Optimistic Offline Lock, optimistic locking, optimistic concurrency control, version check, lock_version]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  An optimistic offline lock gives each record a version. A save checks that the version did not change since the session read the record, and fails on a conflict.
family: architectural
polarity: positive
intent: "Prevent conflicts between concurrent business transactions by detecting a conflict and rolling back the transaction."
history:
  year: 2002
  people: [David Rice]
  note: David Rice wrote the pattern for Fowler's Patterns of Enterprise Application Architecture (2002), in the chapter on offline concurrency patterns.
see_also: [pessimistic-offline-lock, coarse-grained-lock, implicit-lock, unit-of-work, data-mapper, active-record, identity-field, enterprise-application]
sources:
  - url: https://martinfowler.com/eaaCatalog/optimisticOfflineLock.html
    title: David Rice, Catalog of Patterns of Enterprise Application Architecture, Optimistic Offline Lock
    locator: pattern summary
  - url: https://martinfowler.com/eaaCatalog/pessimisticOfflineLock.html
    title: David Rice, Catalog of Patterns of Enterprise Application Architecture, Pessimistic Offline Lock
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 16, Offline Concurrency Patterns, Optimistic Offline Lock
  - url: https://jakarta.ee/specifications/persistence/3.2/jakarta-persistence-spec-3.2
    title: Jakarta Persistence 3.2 specification
    locator: 3.5.1. Optimistic Locking
  - url: https://www.rfc-editor.org/rfc/rfc9110
    title: RFC 9110, HTTP Semantics, June 2022
    locator: section 13.1.1 If-Match
  - url: https://api.rubyonrails.org/classes/ActiveRecord/Locking/Optimistic.html
    title: Ruby on Rails API, ActiveRecord::Locking::Optimistic
    locator: Usage
---

## Intent

An optimistic offline lock "Prevents conflicts between concurrent business transactions by detecting a conflict and rolling back the transaction."[^1] Each record has a version. A session keeps the version that it read, and its save succeeds only if the version in the database is still the same.

## Problem

A business transaction, such as the edit of a customer in a web form, often runs across many requests. Each request is a short system transaction of the database. The database keeps each system transaction correct, but it does not know about the business transaction. "Data integrity is at risk once two sessions begin to work on the same records and lost updates are quite possible."[^1]

For example, Ann and Bob open the same customer. Bob changes the credit and saves. Then Ann changes the name and saves the whole record from the copy that Ann read. The database now has the old credit, and the change of Bob is lost. No error tells anyone about it.

An open system transaction for the whole business transaction is not a good solution. It holds the database locks while a user reads the form, and the user can leave the form open for a long time.

## Solution

"Optimistic Offline Lock solves this problem by validating that the changes about to be committed by one session don't conflict with the changes of another session."[^1] The usual way is a version number in each row:

- **The version column.** Each row has a number that each save increases by one.
- **The read.** The session reads the row and keeps the version with its copy of the data. In a web application, the version goes into the form, often as a hidden field.
- **The checked save.** The session writes the row only if the version is still the one that it read, and it increases the version in the same step. In SQL, this is `UPDATE ... SET version = version + 1 WHERE id = ? AND version = ?`. If no row changed, another session saved first.
- **The conflict.** The save fails, the business transaction rolls back, and the application tells the user. The user can reload the new data and do the edit again.

The check and the write must be one step. "So long as the validation and the updates occur within a single system transaction the business transaction will display consistency."[^1]

```text
  business transaction of bob            business transaction of ann
  request 1: read customer 1, version 1  request 1: read customer 1, version 1
  request 2: UPDATE ... WHERE version = 1
             1 row changed, now version 2
                                         request 2: UPDATE ... WHERE version = 1
                                                    0 rows changed: conflict
```

In the following program, both sessions read version 1. The save of Bob changes the version, so the save of Ann finds a conflict.

```python run id=version-check-on-save
table = {1: {"credit": 500, "version": 1}}


def save(customer_id, credit, read_version):
    row = table[customer_id]
    if row["version"] != read_version:  # someone saved since this session read the row
        return "conflict"
    table[customer_id] = {"credit": credit, "version": read_version + 1}
    return "ok"


ann_version = bob_version = table[1]["version"]  # both sessions read version 1
print("bob:", save(1, 800, bob_version))
print("ann:", save(1, 600, ann_version))
print(table[1])
```

```output
bob: ok
ann: conflict
{'credit': 800, 'version': 2}
```

## Example

Each example file has a table of customers with a `version` column. Ann and Bob load customer 1, and both copies have version 1. Bob saves a new credit of 800, and the version becomes 2. Ann then saves a new name, but the copy of Ann still has version 1, so the save fails with a conflict. The session of Ann reloads the customer, Ann does the edit again on the new data, and the save succeeds with version 3. At the end, the customer has the new name and the new credit, so no change is lost.

## Consequences

**Benefits.**

- **No lost updates.** A save that would write over a change of another session fails, and the user knows it.
- **Many users at the same time.** No lock stops a user from opening a record. "Whereas Pessimistic Offline Lock assumes that the chance of session conflict is high and therefore limits the system's concurrency, Optimistic Offline Lock assumes that the chance of conflict is low."[^1]
- **No lock management.** There is no lock table, and no lock is left behind when a user closes the browser.
- **Simple to add.** One column and one condition in each `UPDATE` and `DELETE`. Many tools do it for you.

**Liabilities.**

**A save with no check loses data.** In the following program, each session saves its whole copy of the record, and nothing checks what changed since the session read it.

```python wrong id=save-without-a-version-check
table = {1: {"name": "Acme Ltd", "credit": 500}}

# Two sessions read the same record, each in its own system transaction.
ann = dict(table[1])
bob = dict(table[1])

bob["credit"] = 800
table[1] = bob  # bob saves first

ann["name"] = "Acme Group"
table[1] = ann  # Wrong: ann's session saves its whole copy, with no check of what changed since it read the row

print("final:", table[1])
print("bob's credit change is lost:", table[1]["credit"] != 800)
```

```output
final: {'name': 'Acme Group', 'credit': 500}
bob's credit change is lost: True
```

Add a version to the record, keep it with each copy, and make the save fail when the version changed.

**The conflict comes at the end.** The user finds the conflict only when the work is done. About a business transaction with many conflicts, Rice writes that "the victims will do all the transaction work only to find at the last minute that the whole thing will fail and their time will have been wasted."[^2] When conflicts are frequent, a [[pessimistic-offline-lock]] is better.

**Every write must check.** One `UPDATE` without the version condition opens the hole again. An [[implicit-lock]] puts the check in shared code, such as the mapper, so that no script can forget it.

**Inconsistent reads.** The check covers only the rows that the session saves. "Also, with one session editing data that another is reading an inconsistent read becomes likely."[^1] To check a row that the session only read, the save must also check the version of that row.[^3]

## When not to use it

**When conflicts are frequent or costly.** If many users edit the same records, or if a business transaction takes a long time, many users will lose their work at the end. Use a [[pessimistic-offline-lock]].

**When one system transaction is enough.** If the read and the write happen in one short request, the locks and the isolation of the database are enough.

**When the change does not need the old copy.** Some changes, such as a counter that many sessions increase, are better as one `UPDATE` that changes the value in the database, such as `SET stock = stock - 1`.

## Variants and related patterns

**Version number or timestamp.** The Jakarta Persistence specification describes optimistic locking as a system "where each revision of an item of data is assigned a version number or timestamp."[^4] A number is safer than a timestamp, because two saves can happen in the same tick of the clock.

**Compare the old values.** In place of a version, the `WHERE` clause can contain the old values of all the fields that the session read. This needs no new column, but the statements are longer.

**Conditional requests in HTTP.** An HTTP client can send `If-Match` with the entity tag that it read. RFC 9110 says that `If-Match` is used "to prevent accidental overwrites when multiple user agents might be acting in parallel on the same resource".[^5] The server refuses the request when the tag changed.

**Related patterns.** A [[pessimistic-offline-lock]] prevents the conflict in place of finding it. A [[coarse-grained-lock]] puts one version on a group of objects, such as a customer and its addresses. An [[implicit-lock]] makes the shared code do the check. A [[unit-of-work]] or a [[data-mapper]] is a good place for the checked `UPDATE`, and the [[identity-field]] tells the save which row to check.

## Known uses

- Jakarta Persistence. "The persistence provider is required to perform optimistic locking automatically for every entity with a version".[^4] The version is a field with the `@Version` annotation, and a failed check throws an `OptimisticLockException`. The specification also warns that "Failure to make use of optimistic locking often leads to inconsistent entity state, lost updates, and other anomalies."[^4]
- Ruby on Rails. "Active Record supports optimistic locking if the lock_version field is present."[^6] A stale save raises `ActiveRecord::StaleObjectError`. To make the check work across web requests, "the recommended approach is to add lock_version as a hidden field to your form."[^6]
- HTTP servers that support `If-Match` with entity tags.[^5]

## References
