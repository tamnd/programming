```python wrong id=strings
import pathlib
import py_compile
import re
import tempfile

# The source of a module that connects to a database.
SOURCE = '''
def connect(host):
    password = "example-not-a-real-password"
    return {"host": host, "user": "app", "password": password}
'''

with tempfile.TemporaryDirectory() as tmp:
    source = pathlib.Path(tmp, "app.py")
    source.write_text(SOURCE)
    compiled = py_compile.compile(str(source), cfile=str(pathlib.Path(tmp, "app.pyc")), dfile="app.py")
    data = pathlib.Path(compiled).read_bytes()

# What the strings tool does: print the runs of readable characters.
for run in re.findall(rb"[ -~]{12,}", data):
    print(run.decode())
```

```output for=strings
example-not-a-real-password
```

The compiled file holds the password as plain text. A reader needs no source code and no Python to find it.

```python run id=from-environment
import os
import pathlib
import tempfile


class ConfigError(Exception):
    pass


def load_config(env):
    """Read the database settings from a mapping such as os.environ."""
    path = env.get("EXAMPLE_DB_PASSWORD_FILE")
    if path:
        password = pathlib.Path(path).read_text().strip()
    else:
        password = env.get("EXAMPLE_DB_PASSWORD", "")
    if not password:
        raise ConfigError("set EXAMPLE_DB_PASSWORD_FILE or EXAMPLE_DB_PASSWORD")
    return {"user": env.get("EXAMPLE_DB_USER", "app"), "password": password}


def describe(config):
    # Logs show that a secret is present, never the secret itself.
    return f"user={config['user']} password=<set>"


# 1. This process has no such variables, so the program refuses to start.
try:
    load_config(os.environ)
except ConfigError as error:
    print("refused to start:", error)

# 2. A deployment that sets the variable.
print(describe(load_config({"EXAMPLE_DB_PASSWORD": "example-not-a-real-password"})))

# 3. A deployment that mounts the secret as a file and names the file.
with tempfile.TemporaryDirectory() as tmp:
    secret_file = pathlib.Path(tmp, "db_password")
    secret_file.write_text("example-not-a-real-password\n")
    print(describe(load_config({"EXAMPLE_DB_PASSWORD_FILE": str(secret_file)})))
```

```output for=from-environment
refused to start: set EXAMPLE_DB_PASSWORD_FILE or EXAMPLE_DB_PASSWORD
user=app password=<set>
user=app password=<set>
```

`load_config` takes a mapping, so tests can pass a dictionary and production passes `os.environ`. A missing setting stops the program.

- Verdict: stdlib. `os.environ` and `os.getenv` read the environment, and `pathlib` reads a mounted secret file. The task [[read-an-environment-variable]] covers the details of `os.environ`.
- Prefer a file that the platform mounts, named by a variable such as `EXAMPLE_DB_PASSWORD_FILE`, over the secret in the variable itself. Child processes inherit the environment of the parent.
- Do not keep secrets in `settings.py` or in a `config.py` that is in version control. Libraries from PyPI such as python-dotenv read a `.env` file into the environment. Keep that file out of the repository with `.gitignore`.
- The `repr` of a dictionary or a dataclass prints every value. Write a `__repr__` that hides the secret, or keep the secret out of objects that the program logs.
- Ruff rule S105 flags a string literal assigned to a name such as `password`. Rules S106 and S107 flag such a literal as an argument and as a default value.
