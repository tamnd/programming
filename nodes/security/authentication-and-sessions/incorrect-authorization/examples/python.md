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

The three checks pass for requests that the policy forbids: a prefix match on the role, a check of the wrong project, and a role read from a cookie.

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

The fixed checks use an exact permission, the project of the loaded task, and a session that the server keeps.

- Verdict: idiom. Python has no authorization framework in its standard library. Dictionaries, sets and `secrets.token_urlsafe` are enough for the pattern.
- `str.startswith`, `in` on strings and `re.search` have no place in a role check. Use `in` on a set of exact permission names, or an `enum.Enum` of roles.
- Look up the role with `.get(role, set())`, so an unknown role has no permissions and the check does not raise.
- Django and other web frameworks have permission systems. The same rules apply: check an exact permission, on the object that the code acts on, with data from the server.
