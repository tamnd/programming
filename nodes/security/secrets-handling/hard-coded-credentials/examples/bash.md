```bash run id=find-in-history
set -eu
# Use no global or system Git settings, so the example runs the same everywhere.
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
repo=$(mktemp -d)
trap 'rm -rf "$repo"' EXIT
cd "$repo"
git init -q -b main
git config user.name "Example Developer"
git config user.email "dev@example.com"

printf 'password = "example-not-a-real-password"\n' > config.py
git add config.py
git commit -q -m "add database config"

printf 'import os\npassword = os.environ["EXAMPLE_DB_PASSWORD"]\n' > config.py
git commit -q -am "read the password from the environment"

echo "the file today:"
cat config.py
echo "commits that add or remove the string:"
git log --all --format='  %s' -S'example-not-a-real-password'
echo "lines in the history that match the pattern:"
git log --all -p --format= -G'password = "' | grep '^[-+]password'
```

```output for=find-in-history
the file today:
import os
password = os.environ["EXAMPLE_DB_PASSWORD"]
commits that add or remove the string:
  read the password from the environment
  add database config
lines in the history that match the pattern:
-password = "example-not-a-real-password"
+password = os.environ["EXAMPLE_DB_PASSWORD"]
+password = "example-not-a-real-password"
```

The file today has no password, but the first commit still has it. `-S` finds the commit that added the string and the commit that removed it. `-G` finds diffs with lines that match a pattern, which helps when the exact value is not known.

```bash run id=from-environment
set -u

load_config() {
  if [ -z "${EXAMPLE_DB_PASSWORD:-}" ]; then
    echo "refused to start: EXAMPLE_DB_PASSWORD is not set"
    return 1
  fi
  db_user=${EXAMPLE_DB_USER:-app}
  echo "user=$db_user password=<set>"
}

# 1. This shell has no such variable, so the configuration fails.
unset EXAMPLE_DB_PASSWORD
load_config || true

# 2. A deployment that sets the variable.
EXAMPLE_DB_PASSWORD=example-not-a-real-password
load_config
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
user=app password=<set>
```

The check makes a missing secret an error, and `${name:-default}` gives a default to a setting that is not secret. The script prints that the password is set, never the value.

- Verdict: stdlib. Bash reads the environment with parameter expansion. `${name:?message}` also stops a script when the variable is unset or empty, with a message on standard error.
- Do not pass a secret as a command-line argument, as in `mysql -p"$password"`. Other users on the system can often see the arguments of a process. Pass it through the environment, standard input or a file.
- Turn off tracing with `set +x` around lines that use a secret, because `set -x` prints each command with its expanded values.
- `git log -S<string> --all` and `git log -G<regex> --all` search every branch and tag. A secret scanner such as Gitleaks searches for many kinds of secrets at once.
- After a secret leaks, revoke or rotate it first. `git filter-repo --sensitive-data-removal` can then remove it from the history.
