---
tag: 67ATSC
kind: pattern
slug: client-session-state
name: Client session state
aliases: [Client Session State, cookie session, signed cookie session, cookie store, hidden field state]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  Client session state keeps the data of a session on the client, in a cookie, a hidden field or the URL, so the server keeps nothing between requests.
family: architectural
polarity: positive
intent: "Store session state on the client."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on session state patterns.
see_also: [server-session-state, database-session-state, object-serialization, deserialization-of-untrusted-data, compute-a-sha-256-hash, data-transfer-object, remote-facade, enterprise-application]
sources:
  - url: https://martinfowler.com/eaaCatalog/clientSessionState.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Client Session State
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 6, Session State; Chapter 17, Session State Patterns, Client Session State
  - url: https://www.rfc-editor.org/rfc/rfc6265
    title: RFC 6265, HTTP State Management Mechanism, April 2011
    locator: section 6.1 Limits
  - url: https://docs.djangoproject.com/en/5.2/topics/http/sessions/
    title: Django 5.2 documentation, How to use sessions
    locator: Using cookie-based sessions
  - url: https://api.rubyonrails.org/classes/ActionDispatch/Session/CookieStore.html
    title: Ruby on Rails API, ActionDispatch::Session::CookieStore
    locator: description
  - url: https://flask.palletsprojects.com/en/stable/quickstart/
    title: Flask documentation, Quickstart
    locator: Sessions
---

## Intent

Client session state "Stores session state on the client."[^1] The server serializes the data of the session, such as the items of a cart or the step of a wizard, and sends it to the client with each response. The client sends it back with the next request. The server keeps nothing between the requests.

## Problem

HTTP is stateless. Each request comes alone, but a user works through many requests: a cart grows, a form has many pages, a search has filters. The application must keep this state from one request to the next.

The server can keep the state in its memory or in a database, but then each server must find the state of each session. When many servers share the load, or when a server restarts, this is hard. A server that keeps nothing can serve any request, and a new server can join at once.

## Solution

Send the state to the client and let the client hold it. There are three usual places:[^2]

- **The URL.** A small state, such as a session ID or a page number, can be a parameter of each link.
- **A hidden field.** A form can carry the state in a hidden input field, and the server reads it when the form comes back.
- **A cookie.** The browser sends the cookie back with each request to the site, with no work from the page.

The state that comes back is input from the client, so the server must not trust it. The parts of a safe design are:

- **The serialization.** The server writes the state as text, such as JSON or a short list of fields, and reads it again on the next request.
- **The signature.** The server signs the serialized state with a key that only the server knows, for example with an HMAC. On the next request, it checks the signature and refuses state that the client changed.
- **The encryption, when needed.** A signature stops changes, but the client can still read the data. Data that the user must not see must also be encrypted.
- **The size limit.** A browser must store only "At least 4096 bytes per cookie",[^3] so the state must be small.

```text
  request 1   client --------------------------------------> server
              server <- cart "pen:1" + signature ------------  (keeps nothing)
  request 2   client -- cookie "pen:1" + signature --------> server: check, add an item
              server <- cart "pen:1,ink:1" + new signature --
```

In the following program, the server signs the state and gives all of it to the client. When the client changes the state, the signature does not match, and the server refuses it.

```python run id=signed-state
import hashlib
import hmac

SECRET = b"kept on the server"


def to_cookie(state):  # the server signs the state and sends all of it to the client
    return state + "|" + hmac.new(SECRET, state.encode(), hashlib.sha256).hexdigest()


def from_cookie(cookie):  # on the next request, the client sends it back
    state, _, tag = cookie.rpartition("|")
    good = hmac.new(SECRET, state.encode(), hashlib.sha256).hexdigest()
    return state if hmac.compare_digest(tag, good) else None


cookie = to_cookie("step=2;plan=basic")
print("state back:", from_cookie(cookie))
print("changed by the client:", from_cookie(cookie.replace("basic", "gold")))
```

```output
state back: step=2;plan=basic
changed by the client: None
```

## Example

Each example file is a cart that lives only in a cookie. Each request reads the cookie, checks its HMAC-SHA256 signature, adds one item and sends a new cookie with a new signature. The output shows the data and the first characters of the signature after each of three requests. Then a client changes `ink:1` to `ink:9` in the cookie, and the server refuses the request because the signature is bad. The server has no table and no map of sessions: the whole cart is in the cookie.

## Consequences

**Benefits.**

- **A stateless server.** Any server can handle any request, so load balancing, fail-over and restarts are simple.
- **No storage on the server.** There is no session table to grow and nothing to clean up. The Django documentation notes that the cookie backend does not have this problem, "because the session data is stored by the users’ browsers."[^4]
- **Fast.** The Rails documentation says that its cookie-based store "is dramatically faster than the alternatives."[^5]
- **The state survives a restart of the server.**

**Liabilities.**

**State with no signature can be changed by the client.** In the following program, the cookie holds the role of the user as plain text. The user edits the cookie, and the server believes the new role.

```python wrong id=unsigned-state
cookie = "user=ann;role=customer"  # Wrong: the state goes to the client with no signature


def role(cookie):
    return dict(part.split("=") for part in cookie.split(";"))["role"]


print("role before:", role(cookie))
cookie = cookie.replace("customer", "admin")  # the client can edit its own cookie
print("role after the client edits the cookie:", role(cookie))
```

```output
role before: customer
role after the client edits the cookie: admin
```

Sign the state with a key that only the server knows, and check the signature on each request. Better still, keep data such as a role on the server, and send only a signed ID.

**The client can read the state.** In Django, "The session data is signed but not encrypted"[^4] and "When using the cookies backend the session data can be read by the client."[^4] Flask says the same of its sessions: "the user could look at the contents of your cookie but not modify it, unless they know the secret key used for signing."[^6]

**Size.** Each request carries the whole state. RFC 6265 asks that "Servers SHOULD use as few and as small cookies as possible to avoid reaching these implementation limits and to minimize network bandwidth due to the Cookie header being included in every request."[^3]

**No freshness and no logout.** The server cannot tell that a cookie is old, so a client can send back an older state. Django warns that the MAC "cannot guarantee freshness i.e. that you are being sent back the last thing you sent to the client."[^4] Because the server keeps no record, "cookie-based sessions are not invalidated when a user logs out."[^4]

**Deserialization of untrusted data.** Never read the state with a deserializer that can make arbitrary objects, such as `pickle`. A signature helps only while the key stays secret. See [[deserialization-of-untrusted-data]].

## When not to use it

**When the state is large.** A cart with hundreds of lines, or a document that the user edits, does not fit in a cookie. Use [[server-session-state]] or [[database-session-state]].

**When the state is secret or sensitive.** Data that the user must not see or keep needs encryption, and data that must be removed at logout needs a record on the server.

**When the server must be able to end a session.** To log a user out at once, or to block a stolen session, the server must keep a record of the session.

## Variants and related patterns

**Signed cookies.** The state is readable, but a signature stops changes. This is the cookie backend of Django and the session of Flask.[^4][^6]

**Encrypted cookies.** The state is encrypted and signed. In Rails, "encrypted cookies cannot be altered or read by users."[^5]

**Only a session ID on the client.** The client holds only a random ID, and the state is on the server. This is the usual mix: client session state for the ID, and [[server-session-state]] or [[database-session-state]] for the data.

**Related patterns.** The serialized state is a kind of [[data-transfer-object]] for the session. The serialization itself is [[object-serialization]].

## Known uses

- Ruby on Rails. "This cookie-based session store is the Rails default."[^5] A `CookieOverflow` exception is raised when the session is larger than 4096 bytes.[^5]
- Django, with the `signed_cookies` session engine.[^4]
- Flask, whose `session` object "is implemented on top of cookies for you and signs the cookies cryptographically."[^6]

## References
