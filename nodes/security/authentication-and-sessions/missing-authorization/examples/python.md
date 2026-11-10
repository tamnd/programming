```python wrong id=login-only
# Sessions and records stand in for a real session store and database.
SESSIONS = {"session-alice": "alice", "session-bob": "bob"}
INVOICES = {
    1: {"owner": "alice", "total": 120},
    2: {"owner": "bob", "total": 75},
}
USERS = {"alice", "bob"}


def get_invoice(session, invoice_id):
    user = SESSIONS.get(session)
    if user is None:
        return 401, "log in first"
    invoice = INVOICES.get(invoice_id)
    if invoice is None:
        return 404, "not found"
    # Missing: a check that the invoice belongs to user.
    return 200, invoice


def delete_user(session, name):
    if SESSIONS.get(session) is None:
        return 401, "log in first"
    # Missing: a check that the caller is an administrator.
    USERS.discard(name)
    return 200, f"deleted {name}"


# Alice opens her own invoice. Then she changes the id in the URL from 1 to 2.
print(get_invoice("session-alice", 1))
print(get_invoice("session-alice", 2))
# Alice is not an administrator, but the handler deletes bob.
print(delete_user("session-alice", "bob"))
```

```output for=login-only
(200, {'owner': 'alice', 'total': 120})
(200, {'owner': 'bob', 'total': 75})
(200, 'deleted bob')
```

The handlers check the session and nothing else. Alice gets Bob's invoice when she changes the id from 1 to 2, and a customer deletes a user.

```python run id=central-policy
SESSIONS = {"session-alice": "alice", "session-carol": "carol"}
ROLES = {"alice": "customer", "bob": "customer", "carol": "admin"}
# The policy lists what each role may do. Anything not listed is denied.
POLICY = {
    "customer": {"invoice:read"},
    "admin": {"invoice:read", "user:delete"},
}
INVOICES = {
    1: {"owner": "alice", "total": 120},
    2: {"owner": "bob", "total": 75},
}
USERS = {"alice", "bob", "carol"}
ROUTES = {}


def route(name, *, permission):
    """Register a handler. permission has no default value."""
    def register(handler):
        ROUTES[name] = (permission, handler)
        return handler
    return register


def allowed(user, permission):
    return permission in POLICY.get(ROLES.get(user), set())


def dispatch(name, session, *args):
    """Every request passes through this one check."""
    if name not in ROUTES:
        return 404, "not found"
    permission, handler = ROUTES[name]
    user = SESSIONS.get(session)
    if user is None:
        return 401, "log in first"
    if not allowed(user, permission):
        return 403, "forbidden"
    return handler(user, *args)


def invoices_of(user):
    # In SQL: SELECT ... FROM invoices WHERE owner = ?
    return {k: v for k, v in INVOICES.items() if v["owner"] == user}


@route("get_invoice", permission="invoice:read")
def get_invoice(user, invoice_id):
    invoice = invoices_of(user).get(invoice_id)
    if invoice is None:
        return 404, "not found"
    return 200, invoice


@route("delete_user", permission="user:delete")
def delete_user(user, name):
    USERS.discard(name)
    return 200, f"deleted {name}"


print(dispatch("get_invoice", "session-alice", 1))
print(dispatch("get_invoice", "session-alice", 2))
print(dispatch("delete_user", "session-alice", "bob"))
print(dispatch("delete_user", "session-carol", "bob"))
print(dispatch("get_invoice", "no-such-session", 1))

# A route without a permission does not register.
try:
    @route("export_all")
    def export_all(user):
        return 200, INVOICES
except TypeError as error:
    print("refused:", error)
```

```output for=central-policy
(200, {'owner': 'alice', 'total': 120})
(404, 'not found')
(403, 'forbidden')
(200, 'deleted bob')
(401, 'log in first')
refused: route() missing 1 required keyword-only argument: 'permission'
```

`dispatch` is the one place where every request is checked. `invoices_of` scopes the lookup to the caller, so another user's invoice gives the same 404 as a missing one.

- Verdict: stdlib. Python has no web authorization in the standard library. The check is a plain function, and web frameworks give the same hook as a decorator or a middleware.
- A required keyword-only argument, such as `permission` in `route`, makes Python refuse a route that has no permission. A default value such as `permission=None` would bring the gap back.
- With an SQL database, write the scope into the query: `SELECT ... FROM invoices WHERE id = ? AND owner = ?`. Pass both values as parameters, as [[sql-injection]] shows.
