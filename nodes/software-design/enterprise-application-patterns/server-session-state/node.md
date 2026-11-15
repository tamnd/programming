---
tag: K194TS
kind: pattern
slug: server-session-state
name: Server session state
aliases: [Server Session State, server-side session, session store, in-memory session, HTTP session]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  Server session state keeps the data of each session on the server, in a serialized form under a session ID. The client holds only the ID.
family: architectural
polarity: positive
intent: "Keep the session state on a server system in a serialized form."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on session state patterns.
see_also: [client-session-state, database-session-state, object-serialization, serialized-lob, missing-authentication, pessimistic-offline-lock, enterprise-application, hash-table]
sources:
  - url: https://martinfowler.com/eaaCatalog/serverSessionState.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Server Session State
    locator: pattern summary
  - url: https://github.com/OWASP/CheatSheetSeries/blob/master/cheatsheets/Session_Management_Cheat_Sheet.md
    title: OWASP Cheat Sheet Series, Session Management Cheat Sheet
    locator: Session ID Content (or Value); Idle Timeout
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 6, Session State; Chapter 17, Session State Patterns, Server Session State
  - url: https://docs.djangoproject.com/en/5.2/topics/http/sessions/
    title: Django 5.2 documentation, How to use sessions
    locator: Using cached sessions; Using file-based sessions; Clearing the session store
  - url: https://www.php.net/manual/en/session.examples.basic.php
    title: PHP Manual, Sessions, Basic usage
    locator: introduction
---

## Intent

Server session state "Keeps the session state on a server system in a serialized form".[^1] The server keeps the data of each session, such as a cart, under a session ID. The client holds only the ID, usually in a cookie, and sends it with each request.

## Problem

HTTP is stateless, but a user works through many requests. The application must keep the state of each session from one request to the next.

The client can hold the state, as in [[client-session-state]], but then each request carries all of it, the client can read it, and the server must check it each time. Large or secret state does not fit there. The state should stay where the client cannot see or change it.

## Solution

Keep a store on the server from the session ID to the state of the session. The parts are:

- **The session ID.** A random value that no one can guess. The server makes it at the start of the session and sends it to the client in a cookie. OWASP says that "The meaning and business or application logic associated with the session ID must be stored on the server side, and specifically, in session objects or in a session management database or repository."[^2]
- **The store.** A map in the memory of the server process, a set of files, or a cache. The state is often serialized, so that the server can write it to a file or move it to another server.[^3]
- **The load and the save.** At the start of each request, the server finds the state from the ID. At the end, it saves the changed state.
- **The timeout.** A user can leave at any time and never come back, so the server ends a session after a time with no request. OWASP says that "All sessions should implement an idle or inactivity timeout."[^2]

```text
  client                        server a
  cookie: sid=a1  --------->    store: a1 -> "pen:2,ink:1"   (last use 10:30)
                                       a2 -> "ink:1"         (last use 10:01)
                  <---------    page with the cart of a1
  at 10:40 the server removes a2, which is idle for more than 30 minutes
```

In the following program, the store is a dictionary on the server. Each user has a random ID, and each ID has its own state.

```python run id=one-state-for-each-id
import secrets

sessions = {}  # on the server: session ID -> state; only the ID goes to the client


def start():
    sid = secrets.token_urlsafe(16)  # a random ID that no one can guess
    sessions[sid] = {"cart": []}
    return sid


ann, bob = start(), start()
sessions[ann]["cart"].append("pen")
sessions[bob]["cart"].append("ink")
print("ann:", sessions[ann]["cart"], "bob:", sessions[bob]["cart"])
print("length of the ID that the client keeps:", len(ann))
```

```output
ann: ['pen'] bob: ['ink']
length of the ID that the client keeps: 22
```

## Example

Each example file has two servers, `a` and `b`, each with its own store in memory, and a timeout of 30 minutes. Ann and Bob start sessions on server `a`, and their carts are separate. Then a load balancer sends one request of Ann to server `b`. Server `b` does not know the session `a1`, so it starts a new session with an empty cart. Ann's next request goes to server `a` again and finds the cart. At 10:40, server `a` removes the session of Bob, which has had no request for 39 minutes, and keeps the session of Ann.

## Consequences

**Benefits.**

- **Simple to program.** The state is a normal object on the server, and most web frameworks give it to each request.
- **Small requests.** The client sends only the ID.
- **The state stays on the server.** The client cannot read or change it, so the server can trust it.
- **The server can end a session.** A logout or a timeout removes the state, and the ID is then worth nothing.

**Liabilities.**

**State that is not keyed by the session is shared by all the users.** In the following program, the cart is one variable in the server process. Bob adds an item and sees the item of Ann.

```python wrong id=one-cart-for-all-users
cart = []  # Wrong: one variable in the server process, shared by all the users


def add(user, item):
    cart.append(item)
    return list(cart)


add("ann", "pen")
print("bob adds ink and sees:", add("bob", "ink"))
```

```output
bob adds ink and sees: ['pen', 'ink']
```

Keep the state of each user in the store under the session ID of that user, and never in a global variable.

**Many servers.** A store in the memory of one server does not help the other servers, as the example shows. The usual answers are session affinity, where the load balancer sends all the requests of a session to the same server, or a shared store, such as a cache or a database.[^3] Django warns that "You should only use cache-based sessions if you’re using the Memcached or Redis cache backend."[^4] It says of its local-memory cache that it "is NOT multi-process safe, therefore probably not a good choice for production environments."[^4]

**Lost state on a restart.** A store in memory loses all the sessions when the server stops. A store in files or in a cache survives a restart.

**Memory.** Each open session takes memory, also when its user has left. The timeout and a regular sweep keep the store small. Django notes that the cache backend does not grow without a limit, "because caches automatically delete stale data."[^4]

**A stolen ID gives the session.** Anyone who has the ID can use the session, so the ID must be random, sent only over HTTPS, and changed at login.[^2]

## When not to use it

**When the servers must be stateless.** If any server must be able to handle any request, and there is no shared store, use [[client-session-state]].

**When the state must survive for a long time.** State that must last for days, or across restarts and deployments, is better as [[database-session-state]].

## Variants and related patterns

**Memory, files or a cache.** PHP, by default, "saves session data on the server at the location specified by the session.save_path configuration directive."[^5] Django can keep sessions in the database, in files or in a cache.[^4]

**Serialized or live objects.** The store can hold live objects, which is fast but ties the session to one process, or serialized data, which can move between servers. The serialized form can be a [[serialized-lob]] in a shared table.[^3]

**Related patterns.** [[client-session-state]] holds the session ID. [[database-session-state]] keeps the state in the database as committed data. The serialization is [[object-serialization]].

## Known uses

- PHP sessions. "Sessions are a simple way to store data for individual users against a unique session ID."[^5] PHP serializes the session data at the end of each script and stores it with the session save handler.[^5]
- Django, with the `cache` and `file` session engines.[^4]

## References
