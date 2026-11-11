---
tag: GZ31FD
kind: weakness
slug: incorrect-authorization
name: Incorrect authorization
aliases: [incorrect authorization check, flawed access control check, broken authorization logic, cwe-863]
area: security/authentication-and-sessions
level: core
summary: >-
  Incorrect authorization happens when a program checks permission but the check is wrong, such as a test on the wrong object, a loose match of a role name, or a value the client controls.
cwe: 863
abstraction: class
languages_affected: [any]
owasp_top10_2025: A01
cwe_top25_2025: 17
history:
  year: 1988
  people: [Norm Hardy]
  note: In The Confused Deputy, Hardy described a compiler at Tymshare that overwrote the billing file. The operating system checked the authority of the compiler, which was the wrong authority for a file name that the user supplied.
sources:
  - url: https://cwe.mitre.org/data/definitions/863.html
    title: "CWE-863: Incorrect Authorization"
    locator: Description; Alternate Terms; Relationships; Modes Of Introduction; Common Consequences; Potential Mitigations; Detection Methods; Demonstrative Examples, Example 1; Observed Examples; Content History
  - url: http://cap-lore.com/CapTheory/ConfusedDeputy.html
    title: "Norm Hardy, The Confused Deputy (or why capabilities might have been invented), Operating Systems Review, Vol. 22, No. 4, 1988"
    locator: the whole note, from the story of (SYSX)FORT and (SYSX)BILL to the paragraph on the two sources of authority
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 17, Rank Last Year
  - url: https://top10.owasp.org/2025/A01_2025-Broken_Access_Control/
    title: "OWASP Top 10:2025, A01:2025 Broken Access Control"
    locator: Description, the list of common vulnerabilities; How to prevent; List of Mapped CWEs
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Authorization_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Authorization Cheat Sheet
    locator: Prefer Attribute and Relationship Based Access Control over RBAC; Ensure Lookup IDs are Not Accessible Even When Guessed or Cannot Be Tampered With; Verify that Authorization Checks are Performed in the Right Location; Exit Safely when Authorization Checks Fail; Create Unit and Integration Test Cases for Authorization Logic
  - url: https://cwe.mitre.org/data/definitions/602.html
    title: "CWE-602: Client-Side Enforcement of Server-Side Security"
    locator: Description; Observed Examples
  - url: https://cwe.mitre.org/data/definitions/807.html
    title: "CWE-807: Reliance on Untrusted Inputs in a Security Decision"
    locator: Description
  - url: https://cwe.mitre.org/data/definitions/639.html
    title: "CWE-639: Authorization Bypass Through User-Controlled Key"
    locator: Description; Relationships
  - url: https://cwe.mitre.org/data/definitions/441.html
    title: "CWE-441: Unintended Proxy or Intermediary (Confused Deputy)"
    locator: Alternate Terms
  - url: https://top10.owasp.org/2025/A06_2025-Insecure_Design/
    title: "OWASP Top 10:2025, A06:2025 Insecure Design"
    locator: List of Mapped CWEs
  - url: https://www.cve.org/CVERecord?id=CVE-2006-6679
    title: CVE-2006-6679, chetcpasswd
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2008-4577
    title: CVE-2008-4577, Dovecot
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2019-15900
    title: CVE-2019-15900, slicer69 doas
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-39155
    title: CVE-2021-39155, Istio
    locator: the description, the CWE and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2025-32796
    title: CVE-2025-32796, Dify
    locator: the description and the publication date
---

## Description

Incorrect authorization happens when the product performs an authorization check when an actor attempts to access a resource or perform an action, but it does not correctly perform the check.[^1] The check is in the code. A reviewer who looks for it finds it. A test with the expected users passes. But the check answers a different question from the one that the policy asks, so some requests that the policy forbids get through.

The difference from [[missing-authorization]] is the presence of the check. With a missing check, the fix is to add one. With an incorrect check, the code already looks safe, and that is why the weakness survives review. CWE-863 and CWE-862 are siblings under CWE-285, Improper Authorization.[^1]

The checks go wrong in a few common ways:

- **A loose match.** The code compares a role or a path with a prefix, a substring, a pattern or a comparison that ignores case where the policy needs an exact value, or the reverse. A role named `admin-readonly` starts with `admin`.
- **The wrong object.** The code checks the object that the request names in one place and then acts on an object that the request names in another place. A user who belongs to project 1 asks to delete task 99 "in project 1", and the code checks project 1 but never checks that task 99 is in it. CWE-639, Authorization Bypass Through User-Controlled Key, is a child of CWE-863.[^8]
- **A value the client controls.** The code reads the role from a cookie, a hidden field, a header or a flag in the request body. CWE-807 covers a protection mechanism that relies on an input that an untrusted actor can modify.[^7] The first demonstrative example of CWE-863 is a medical records application that reads the role from a cookie.[^1]
- **A check on the client only.** The browser or the mobile app hides the button, and the server accepts the request. CWE-602 describes a server that relies on the client to implement a protection mechanism that is intended to protect the server.[^6] The OWASP Top 10:2025 maps CWE-602 and CWE-807 to A06:2025 Insecure Design.[^10]
- **A check that fails open.** An error in the check, such as a failed parse or an exception, leads to "allow". The OWASP Authorization Cheat Sheet asks that a failure of an authorization check does not put the software into a state that could lead to a bypass.[^5]
- **Logic errors in the rules.** A deny rule is read as an allow rule, or two rules interact in a way that nobody planned. The Dovecot case below is an example.

CWE lists two ways that the weakness starts. A single-user application moves to a multi-user environment. Or a developer assumes that attackers cannot modify inputs such as headers or cookies.[^1] An attacker can then read or modify data, or gain privileges, past the intended restrictions.[^1]

CWE-863 is a class weakness.[^1] It is rank 17 on the 2025 CWE Top 25, up from rank 18 the year before.[^3] The OWASP Top 10:2025 maps it to A01:2025 Broken Access Control.[^4] That category lists access control checks bypassed by modifying the URL, internal application state or the HTML page, and a cookie or hidden field manipulated to elevate privileges.[^4]

### History

Norm Hardy published "The Confused Deputy" in 1988. He describes events at Tymshare, a timesharing company. A compiler was installed in the directory SYSX. It had a "home files license", so that it could write a statistics file in SYSX. A user could name a file to receive the debugging output of the compiler. One user named the billing file (SYSX)BILL. The operating system saw that the compiler had home files license, and it let the compiler write over the billing file.[^2]

The operating system made a check, and the check passed. It was the wrong check. Hardy names the problem: the compiler runs with authority stemming from two sources, its own license and the authority of the user who invoked it, and it has no way to keep them apart.[^2] For the debugging output, the right question was whether the user may write that file. CWE uses "confused deputy" as an alternate term for CWE-441, Unintended Proxy or Intermediary.[^9] The same mistake appears in a web service that checks its own database rights where it should check the rights of the user who sent the request.

CWE-863 entered CWE in version 1.13, released on 2011-06-01.[^1]

## Vulnerable code

The three handlers below each have a check. Dictionaries stand for the database, and each function stands for a request handler, as on the [[missing-authorization]] page. The demonstrations are the classic ones for each mistake: an auditor with the role `admin-readonly`, a task id from another project, and a cookie set to `role=admin`.

```python wrong id=three-checks
# Dictionaries stand for the database. Each function stands for a request handler.
USERS = {
    "ana": {"role": "admin"},
    "bao": {"role": "admin-readonly"},  # an auditor: may read, may not change
    "chi": {"role": "member"},
    "dan": {"role": "member"},
}
PROJECTS = {1: {"members": {"chi"}}, 2: {"members": {"ana"}}}
TASKS = {7: {"project": 1, "title": "fix login"}, 99: {"project": 2, "title": "pay invoices"}}


# 1. The role check matches a prefix.
def delete_user(caller, name):
    if not USERS[caller]["role"].startswith("admin"):
        return "403 admins only"
    del USERS[name]
    return f"200 deleted {name}"


# 2. The check is on the project in the URL, not on the task that is deleted.
def delete_task(caller, project_id, task_id):
    if caller not in PROJECTS[project_id]["members"]:
        return "403 not a member"
    title = TASKS.pop(task_id)["title"]
    return f"200 deleted task {task_id} ({title})"


# 3. The role comes from a cookie, and the client sets the cookie.
def export_all(caller, cookies):
    if cookies.get("role") != "admin":
        return "403 admins only"
    return f"200 export of {len(USERS)} users"


print("bao", delete_user("bao", "dan"))
print("chi", delete_task("chi", 1, 99))
print("chi", export_all("chi", {"role": "admin"}))
```

```output for=three-checks
bao 200 deleted dan
chi 200 deleted task 99 (pay invoices)
chi 200 export of 3 users
```

Each check passes, and each one should fail:

1. `bao` is an auditor. The role `admin-readonly` starts with `admin`, so the auditor deletes a user.
2. `chi` is a member of project 1. The check uses the project in the URL, and the code then deletes task 99, which belongs to project 2.
3. `chi` is a member. The handler reads the role from a cookie, and the client sets the cookie to `admin`. This is the case of the first demonstrative example of CWE-863.[^1]

## Fixed code

The fixed handlers keep the same shape. Each check now asks the right question.

```python run id=right-checks
import secrets

# Dictionaries stand for the database. Each function stands for a request handler.
USERS = {
    "ana": {"role": "admin"},
    "bao": {"role": "admin-readonly"},
    "chi": {"role": "member"},
    "dan": {"role": "member"},
}
PROJECTS = {1: {"members": {"chi"}}, 2: {"members": {"ana"}}}
TASKS = {7: {"project": 1, "title": "fix login"}, 99: {"project": 2, "title": "pay invoices"}}

# 1. Each role maps to an exact set of permissions. A role not listed has none.
PERMISSIONS = {
    "admin": {"users.read", "users.delete", "users.export"},
    "admin-readonly": {"users.read"},
    "member": set(),
}


def allowed(caller, permission):
    return permission in PERMISSIONS.get(USERS[caller]["role"], set())


def delete_user(caller, name):
    if not allowed(caller, "users.delete"):
        return "403 not allowed"
    del USERS[name]
    return f"200 deleted {name}"


# 2. Load the task first, then check the project that the task belongs to.
def delete_task(caller, project_id, task_id):
    task = TASKS.get(task_id)
    if task is None or task["project"] != project_id:
        return "404 no such task in this project"
    if caller not in PROJECTS[task["project"]]["members"]:
        return "403 not a member"
    del TASKS[task_id]
    return f"200 deleted task {task_id} ({task['title']})"


# 3. The cookie holds only a random session id. The server keeps the user.
SESSIONS = {}


def login(name):
    sid = secrets.token_urlsafe(16)
    SESSIONS[sid] = name
    return {"sid": sid}


def export_all(cookies):
    caller = SESSIONS.get(cookies.get("sid", ""))
    if caller is None:
        return "401 login required"
    if not allowed(caller, "users.export"):
        return "403 not allowed"
    return f"200 export of {len(USERS)} users"


print("bao", delete_user("bao", "dan"))
print("chi", delete_task("chi", 1, 99))
print("chi", delete_task("chi", 1, 7))
chi = login("chi")
print("chi", export_all({**chi, "role": "admin"}))
print("ana", export_all(login("ana")))
```

```output for=right-checks
bao 403 not allowed
chi 404 no such task in this project
chi 200 deleted task 7 (fix login)
chi 403 not allowed
ana 200 export of 4 users
```

1. **An exact permission.** `PERMISSIONS` maps each role to a set of permissions, and the check looks up the exact name of the permission that the action needs. A role that is not in the map gets an empty set. The code never compares part of a string.
2. **The object that the code acts on.** `delete_task` loads the task first. It checks that the task is in the project of the URL, and it checks membership in the project that the task belongs to. The check and the action use the same object. A task from another project gets 404, the same answer as a task that does not exist.
3. **State on the server.** The cookie holds only a random session id. The server finds the user from the session, and it finds the role from its own data. A `role` in the cookie has no effect. The OWASP Top 10:2025 says that access control is only effective in trusted server-side code, where the attacker cannot modify the access control check or metadata.[^4]

### Checks in the client

A check in the browser or in an app helps the user. It hides a button that would fail. It is not a control, because the attacker does not have to use that client. The OWASP Authorization Cheat Sheet says that developers must never rely on client-side access control checks, and that the checks must be performed on the server, at the gateway or in serverless functions.[^5] CWE-863 lists a chain in which reliance on client-side security let attackers bypass authorization with a custom client.[^1] The JavaScript example file shows a page that hides a button and sends an `isAdmin` flag, and a server that trusts the flag.

### Roles and objects

A role check answers a question about a type of action. It cannot answer a question about one object. CWE notes that role-based access control may not protect against horizontal authorization, where one user attacks another user with the same role.[^1] The OWASP Authorization Cheat Sheet prefers attribute-based and relationship-based access control for application code. It also warns that chains of role checks, such as a check for any of several roles, are easy to get wrong.[^5] A rule such as "a member of the project that owns this task" names both the user and the object, so the check cannot leave the object out.

## How to detect it

- **Read each check as a question.** For each check, write down the policy that it should enforce, in words. Then read the code and ask whether it answers that question for every input. Look for `startswith`, `contains`, `in`, regular expressions and case changes in security decisions.
- **Trace where each input comes from.** Every value in an authorization decision should come from the server: the session, the database or a signed token that the server checks. A role, a user id or a flag that comes from the request body, a cookie, a header or the query string is a finding.
- **Check that the check and the action use the same object.** If the code checks one id and acts on another, or loads the object twice, the two can differ.
- **Tests with the near misses.** Test with users whose role names look like the allowed ones, with ids from another project or tenant, with a changed cookie and with a request sent without the client. The cheat sheet asks for unit and integration tests of authorization logic, including tests that access is denied by default.[^5]
- **Look for fail-open paths.** Find what happens when the check throws, when a lookup returns nothing and when a parse fails. The answer must be "deny".[^5]
- **Know the limits of tools.** CWE rates automated static analysis as limited: tools have difficulty with custom authorization schemes, and they might not be able to tell whether a scheme can be bypassed. Automated dynamic analysis may not find interfaces that have a check, even if the check is weak. Manual analysis is rated as moderate.[^1]

## How to prevent it

- **Decide on the server, from server data.** Do not read a role, a permission or a user id from anything that the client sends, except a session id or a token that the server verifies.[^4][^7]
- **Compare exact values.** Map roles to sets of permissions, and check an exact permission. Use an enum or a constant, not a string pattern. Treat an unknown role as a role with no permissions.
- **Load the object, then check it.** Get the object that the code will act on, check the rule against that object, and act on that same object. Scope database queries to the parent or the owner.[^4]
- **Fail closed.** An error, a missing value or an unknown case leads to "deny".[^5]
- **Implement the check once.** Put the policy in one module that every handler uses. The OWASP Top 10:2025 says to implement access control mechanisms once and reuse them.[^4]
- **Treat client checks as user interface.** Keep them for the user, and repeat every rule on the server.[^5][^6]
- **Prefer rules that name the object.** Use attribute-based or relationship-based rules where a role does not say enough.[^5]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2006-6679 | published 2006-12-21 | chetcpasswd before 2.4 relied on the `X-Forwarded-For` HTTP header when it checked a client against an IP address ACL. Remote attackers could spoof the header.[^11] |
| CVE-2008-4577 | published 2008-10-15 | The ACL plugin in Dovecot before 1.1.4 treated negative access rights as if they were positive access rights.[^12] |
| CVE-2019-15900 | published 2019-10-18 | On platforms without `strtonum`, doas before 6.2 used `sscanf` without a check for errors. An uninitialized variable was checked instead, and in some cases the program ran the command as root when the user or group name did not exist.[^13] |
| CVE-2021-39155 | published 2021-08-24 | The Istio authorization policy compared the hostname in the HTTP `Host` header in a case-sensitive way, but the proxy routed the request in a case-insensitive way. The policy could be bypassed.[^14] |
| CVE-2025-32796 | published 2025-04-18 | In Dify before 0.6.12, normal users could enable or disable apps through the API, although the button for this action was disabled in the web UI.[^15] |

## References
