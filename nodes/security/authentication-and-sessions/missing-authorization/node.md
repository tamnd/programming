---
tag: 3A0889
kind: weakness
slug: missing-authorization
name: Missing authorization
aliases: [missing authorization check, missing access control, insecure direct object reference, IDOR, broken object level authorization, BOLA, missing function level access control, cwe-862]
area: security/authentication-and-sessions
level: core
summary: >-
  Missing authorization happens when a program checks who the user is but not whether that user may do the action or reach the object, so a changed id or URL gives access.
cwe: 862
abstraction: class
languages_affected: [any]
owasp_top10_2025: A01
cwe_top25_2025: 4
history:
  year: 1975
  people: [Jerome H. Saltzer, Michael D. Schroeder]
  note: In The Protection of Information in Computer Systems, Saltzer and Schroeder stated complete mediation, that every access to every object must be checked for authority, and fail-safe defaults.
sources:
  - url: https://cwe.mitre.org/data/definitions/862.html
    title: "CWE-862: Missing Authorization"
    locator: Description; Alternate Terms; Common Consequences; Potential Mitigations; Relationships; Modes Of Introduction; Demonstrative Examples, Examples 1 and 2; Observed Examples; Detection Methods; Content History
  - url: https://cwe.mitre.org/data/definitions/639.html
    title: "CWE-639: Authorization Bypass Through User-Controlled Key"
    locator: Description; Alternate Terms; Observed Examples; Memberships
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 4, Rank Last Year; ranks 17 and 24
  - url: https://top10.owasp.org/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: Description; How to prevent; List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Authorization_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Authorization Cheat Sheet
    locator: Deny by Default; Validate the Permissions on Every Request; Verify that Authorization Checks are Performed in the Right Location; Create Unit and Integration Test Cases for Authorization Logic
  - url: https://web.mit.edu/Saltzer/www/publications/protection/Basic.html
    title: "Jerome H. Saltzer and Michael D. Schroeder, The Protection of Information in Computer Systems, Proceedings of the IEEE 63(9), 1975, pages 1278 to 1308"
    locator: Section I.A.3, Design Principles, b) Fail-safe defaults and c) Complete mediation
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Insecure_Direct_Object_Reference_Prevention_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Insecure Direct Object Reference Prevention Cheat Sheet
    locator: Identifier complexity; Verifying access controls; Mitigation
  - url: https://api-security.owasp.org/editions/2023/en/0xa1-broken-object-level-authorization/
    title: "OWASP API Security Top 10 2023, API1:2023 Broken Object Level Authorization"
    locator: How To Prevent
  - url: https://codeql.github.com/codeql-query-help/csharp/cs-web-missing-function-level-access-control/
    title: CodeQL query help for C#, Missing function level access control
    locator: the query ID; Recommendation
  - url: https://codeql.github.com/codeql-query-help/csharp/cs-web-insecure-direct-object-reference/
    title: CodeQL query help for C#, Insecure Direct Object Reference
    locator: the query ID; Recommendation
  - url: https://www.cve.org/CVERecord?id=CVE-2009-2213
    title: CVE-2009-2213, Citrix NetScaler Access Gateway
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2009-3168
    title: CVE-2009-3168, Basic PHP Events Lister
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-36539
    title: CVE-2021-36539, Instructure Canvas LMS
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2024-6845
    title: CVE-2024-6845, Chatbot with ChatGPT WordPress plugin
    locator: the description and the publication date
---

## Description

Authentication answers the question "who is this user?". Authorization answers the question "may this user do this action on this object?". CWE calls the two AuthN and AuthZ. It discourages the short form "Auth", because it could mean either one.[^1] Missing authorization happens when the product does not perform an authorization check when an actor attempts to access a resource or perform an action.[^1] The user is often logged in. The program knows who the user is, but it never asks whether that user may do what the request asks.

The weakness has two common forms:

- **Function level.** A normal user calls a function for administrators, such as a page that resets a password. The page may have no link for normal users, but the server answers a direct request. CWE-425, Direct Request, is a child of CWE-862 for this case.[^1]
- **Object level.** A user changes the identifier of a record in a request, and the server returns the record of another user. CWE-639 describes this case. The product does not prevent one user from gaining access to another user's data by modifying the key value.[^2] Its alternate terms are insecure direct object reference (IDOR), broken object level authorization (BOLA) and horizontal authorization.[^2]

CWE gives two examples. In the first, a function uses a prepared statement, so it is safe from [[sql-injection]]. But it never checks that the user may run the query.[^1] In the second, a message board checks the password, and then it shows any private message whose id the user sends.[^1] An attacker can read or change data, gain privileges, or use up resources until the service stops.[^1]

CWE notes two ways that the weakness starts. A single-user application moves to a multi-user environment. Or a developer assumes that attackers cannot change inputs such as headers or cookies.[^1] The OWASP Authorization Cheat Sheet says that permission should be validated correctly on every request. It adds that an attacker only needs to find one way in.[^5]

CWE-862 is a class weakness. It is a child of CWE-285, Improper Authorization. Its children include CWE-425 and CWE-638, Not Using Complete Mediation.[^1] It is rank 4 on the 2025 CWE Top 25, up from rank 9 the year before.[^3] On the same list, CWE-863, Incorrect Authorization, is rank 17, and CWE-639 is rank 24.[^3] The OWASP Top 10:2025 maps all three to A01:2025 Broken Access Control. That category lists IDOR as viewing or editing someone else's account by providing its unique identifier.[^4]

[[cross-site-request-forgery]] is a different weakness. There the check is present, and the attacker makes the browser of an authorized user send the request.

### History

Saltzer and Schroeder stated the two principles behind the fix in 1975.[^6] Complete mediation says: "Every access to every object must be checked for authority."[^6] Fail-safe defaults says: "Base access decisions on permission rather than exclusion."[^6] They credit the second principle to E. Glaser in 1965. They warn that a mechanism that lists exclusions tends to fail by allowing access, and that this failure may go unnoticed in normal use.[^6]

The OWASP Top Ten 2007 had a category A4, Insecure Direct Object Reference. The 2019 OWASP API Security Top 10 used the name BOLA.[^2] CWE-862 entered CWE in version 1.13, released on 2011-06-01.[^1]

## Vulnerable code

The handlers below check that the caller is logged in. They never check what the caller may do. Dictionaries stand for the session store and the database. The classic demonstration is to change the id in the URL from 1 to 2.

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

The second call gives Bob's invoice to Alice. The third call lets a customer delete a user. Both handlers authenticate the caller, but neither one authorizes the action.

## Fixed code

The fix puts one check in front of every handler. Each route names the permission that it needs, and a policy lists the permissions of each role. The invoice lookup also includes the owner.

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

Alice reads invoice 1 and gets 404 for invoice 2. She gets 403 when she tries to delete a user, and Carol, an administrator, can do it. A request without a session gets 401. The last line shows that a route without a permission does not register, because `permission` has no default value.

### Deny by default

The policy lists only what each role may do. A user without a role, a role that is not in the policy, and a permission that no role has all get 403. This is the deny-by-default rule. The OWASP Top 10:2025 says: "Except for public resources, deny by default."[^4] The Authorization Cheat Sheet asks developers to justify each permission, and not to assume that access is the default.[^5]

A list of forbidden actions fails in the other direction. A developer adds a route and forgets to add it to the list, and the route is open to everyone.

### Check the object, not only the role

A role check does not stop one customer from reading the invoice of another customer, because both have the same role. CWE says that role-based access control may not protect against horizontal authorization.[^1] So the fixed code scopes the lookup to the user. The OWASP IDOR Prevention Cheat Sheet says to look up objects in datasets that the user has access to.[^7] In SQL, put the owner in the `WHERE` clause, as in `WHERE id = ? AND owner = ?`. The OWASP API Security Top 10 asks for the check in every function that reads a record with an input from the client.[^8]

The fixed handler returns 404 for another user's invoice. This is the same answer as for an invoice that does not exist, so the response does not tell the caller which ids exist.

Random identifiers, such as UUIDs, make ids hard to guess. They do not replace the check. The IDOR cheat sheet says that access control checks are essential even with complex identifiers. If attackers obtain the URL of an object, the application must still block them.[^7]

### Where the check goes

Put the check on the server. The Authorization Cheat Sheet says that developers must never rely on client-side access control checks.[^5] A hidden button does not stop a direct request. Put the check in one place that every request passes through, such as a middleware or a dispatcher. The OWASP Top 10:2025 says to implement access control mechanisms once and reuse them throughout the application.[^4] Frameworks have such a place. In ASP.NET MVC, the `Authorize` attribute requires authorization on an action method.[^9] The cheat sheet also prefers explicit configuration, even when a framework denies by default.[^5]

## How to detect it

- **Tests with two users.** The IDOR cheat sheet says to create User A and User B, and objects for each one. Then log in as User A and try to reach the objects of User B.[^7] Test reads, creates, updates, deletes, exports and administrative actions.[^7] The API Security Top 10 says not to deploy changes that make these tests fail.[^8] The Authorization Cheat Sheet also asks for unit and integration tests of the authorization logic.[^5]
- **A route inventory.** List every route with its permission. A test that fails on a route without a permission finds the gap before release. The examples make the permission a required argument for the same reason.
- **Static analysis.** CWE rates automated static analysis as limited here. Tools have difficulty with custom authorization schemes, and some functions are public by design.[^1] CodeQL for C# has the queries `cs/web/missing-function-level-access-control`[^9] and `cs/web/insecure-direct-object-reference`.[^10]
- **Manual review.** CWE rates manual analysis as moderate, and architecture or design review as highly effective.[^1] For each handler, ask who the caller is and whether the caller may do this action to this object.
- **Dynamic scanners.** They can find interfaces that do not require authorization. A person must still decide whether each one breaks the business rules.[^1]

## How to prevent it

- **Deny by default.** Grant each permission on purpose, and refuse everything else.[^4][^5]
- **Check every request on the server.** Do not trust the client to hide or block an action.[^5][^6]
- **Implement the check once.** Use one middleware, dispatcher or framework attribute, and make routes declare their permission.[^4]
- **Scope data access to the user.** Look up records through the owner or the tenant, not by id alone.[^7]
- **Map roles to areas.** CWE suggests anonymous, normal, privileged and administrative areas, with role-based access control at the boundaries.[^1]
- **Check business rules.** A database may limit medical records to one database user. CWE notes that each record may still be meant only for the patient and the doctor.[^1]
- **Log failures.** The OWASP Top 10:2025 says to log access control failures and alert admins when appropriate.[^4]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2009-2213 | published 2009-06-25 | The default configuration of the Citrix NetScaler Access Gateway set the Default Authorization Action to Allow. Authenticated users might bypass the intended access restrictions.[^11] |
| CVE-2009-3168 | published 2009-09-11 | Basic PHP Events Lister 2.0 did not restrict access to two admin scripts. Authenticated users could reset administrator passwords or add administrators with a direct request.[^12] |
| CVE-2021-36539 | published 2023-01-26 | Instructure Canvas LMS did not deny access to locked or unpublished files through the file preview URL.[^13] CWE lists it under CWE-639, because file ids were not restricted to the user.[^2] |
| CVE-2024-6845 | published 2024-09-25 | A REST endpoint of the Chatbot with ChatGPT WordPress plugin, before 2.4.6, had no proper authorization. Unauthenticated users could get the encoded OpenAI API key and decode it.[^14] |

## References
