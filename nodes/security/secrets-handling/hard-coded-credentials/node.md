---
tag: TX2C28
kind: weakness
slug: hard-coded-credentials
name: Hard-coded credentials
aliases: [hardcoded credentials, hard-coded password, hard-coded key, embedded credentials, secrets in source code, cwe-798]
area: security/secrets-handling
level: core
summary: >-
  Hard-coded credentials are passwords, keys or tokens written into the source code or the binary of a program, where everyone who can read the code or the history can read them.
cwe: 798
abstraction: base
languages_affected: [any]
owasp_top10_2025: A07
history:
  year: 2010
  people: []
  note: The Stuxnet worm used a hard-coded password in the Siemens Simatic WinCC and PCS 7 SCADA software to reach a back-end database, as CVE-2010-2772 records for July 2010.
sources:
  - url: https://cwe.mitre.org/data/definitions/798.html
    title: "CWE-798: Use of Hard-coded Credentials"
    locator: Description, Technical Nuance; Common Consequences; Potential Mitigations; Relationships; Detection Methods; Demonstrative Examples; Observed Examples; Content History
  - url: https://top10.owasp.org/2025/A07_2025-Authentication_Failures/
    title: "OWASP Top 10:2025, A07:2025 Authentication Failures"
    locator: List of Mapped CWEs
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list; CWE-798 is not on it
  - url: https://cwe.mitre.org/top25/archive/2024/2024_top25_list.html
    title: 2024 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 22
  - url: https://www.cve.org/CVERecord?id=CVE-2010-2772
    title: CVE-2010-2772, Siemens Simatic WinCC and PCS 7
    locator: the description and the publication date
  - url: https://cheatsheetseries.owasp.org/cheatsheets/Secrets_Management_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, Secrets Management Cheat Sheet
    locator: 2.2 Centralize and Standardize; 5.1 Injection of Secrets into a Container; 8 Detection; 9.2 Remediation
  - url: https://12factor.net/config
    title: The Twelve-Factor App, III. Config
    locator: the whole page
  - url: https://www.ndss-symposium.org/ndss-paper/how-bad-can-it-git-characterizing-secret-leakage-in-public-github-repositories/
    title: "Michael Meli, Matthew R. McNiece and Bradley Reaves, How Bad Can It Git? Characterizing Secret Leakage in Public GitHub Repositories, NDSS 2019"
    locator: the abstract
  - url: https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/removing-sensitive-data-from-a-repository
    title: GitHub Docs, Removing sensitive data from a repository
    locator: About removing sensitive data from a repository; Side effects of rewriting history; Purging a file from your local repository's history using git-filter-repo
  - url: https://docs.github.com/en/code-security/concepts/secret-security/secret-scanning
    title: GitHub Docs, About secret scanning
    locator: the introduction
  - url: https://docs.github.com/en/code-security/concepts/secret-security/push-protection
    title: GitHub Docs, About push protection
    locator: the introduction; bypassing push protection
  - url: https://git-scm.com/docs/git-log
    title: Git documentation, git-log
    locator: the options -S, -G and --all
  - url: https://github.com/gitleaks/gitleaks
    title: Gitleaks, README
    locator: the introduction
  - url: https://docs.astral.sh/ruff/rules/hardcoded-password-string/
    title: Ruff rules, hardcoded-password-string (S105)
    locator: What it does; Why is this bad?
  - url: https://github.com/securego/gosec/blob/master/RULES.md
    title: gosec, rules
    locator: G101
  - url: https://www.cve.org/CVERecord?id=CVE-2012-3503
    title: CVE-2012-3503, Katello
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2022-30271
    title: CVE-2022-30271, Motorola ACE1000 RTU
    locator: the description and the publication date
---

## Description

A credential proves who a program or a person is. Passwords, API keys, tokens, private keys and the keys for encryption are credentials. A hard-coded credential is one that is written into the source code, so it is also in every build and every copy of the program.[^1] Everyone who can read the code can read the credential. That group is larger than it seems. It includes every developer, the build system, every backup, every clone and fork of the repository, and anyone who gets a copy of the binary. CWE says that a password in a client binary is usually not hard to extract.[^1]

CWE-798 describes two variants.[^1] In the inbound variant, the product checks logins against a fixed account, such as a default administrator with a fixed password. Every installation has the same password, and administrators often cannot change or turn off the account without a patch. One leaked password then opens every installation. In the outbound variant, the product connects to another system, such as a database or an API, with credentials that are in the code. Anyone who reads them can connect as the product.

A hard-coded credential is also hard to change. To rotate it, someone must change the code, build it and deploy it again. Until then, the old value keeps working.

CWE-798 is a base weakness. Its children are CWE-259, use of hard-coded password, and CWE-321, use of hard-coded cryptographic key.[^1] In the OWASP Top 10:2025, it is part of A07:2025 Authentication Failures.[^2] It is not on the 2025 CWE Top 25.[^3] It was rank 22 of the 2024 list.[^4]

### History

The Stuxnet worm is a well-known case. CVE-2010-2772 records that the Siemens Simatic WinCC and PCS 7 SCADA software used a hard-coded password. The Stuxnet worm used it in the wild in July 2010 to reach a back-end database.[^5] In February 2010, before that case, CWE 1.8 had added CWE-798 as a more abstract entry for hard-coded passwords and hard-coded keys.[^1]

### The history of the repository

Removing a credential from the code does not remove it from version control. Git keeps every earlier version of each file, so the credential stays in the history. Copies of the history stay in every clone and every fork.[^9] A study of public GitHub repositories found secrets in over 100,000 repositories, and found that thousands of unique secrets leaked every day.[^8] Treat a credential that reached a shared repository as known to others.

## Vulnerable code

The module below has the database password in a string literal. The program compiles the module to a `.pyc` file, as Python does when it imports a module. Then it lists the readable strings in that file. This is what the `strings` tool does with any binary.

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

The password is in the compiled file as plain text. The same is true of a Java class file, a .NET assembly and a native binary. Obfuscation and encoding, such as Base64, do not help, because the program must decode the value to use it.

## Fixed code

The fix keeps the credential out of the code. The code reads it at run time from the environment of the process, from a file that the platform mounts, or from a secret store. The task [[read-an-environment-variable]] shows how to read a variable in each language. The function below takes the mapping as an argument, so the example can show both a missing setting and a deployment that sets one.

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

The program has no default password. When the setting is missing, it stops with a clear message, and it does not fall back to a value in the code. It never prints the secret.

### Where the secret comes from

The Twelve-Factor App keeps configuration in environment variables, separate from the code. Its test is whether the codebase could be made open source at any moment without compromising any credentials.[^7] That test is a good goal for every repository.

Environment variables are not the strongest store. The OWASP Secrets Management Cheat Sheet says that they are generally accessible to all processes, and that they may appear in logs or system dumps. It does not recommend them unless the other methods are not possible.[^6] It prefers files that the orchestrator mounts into the container at run time, and a central secret manager.[^6] A secret manager also gives access control, audit logs and rotation in one place. Never put a secret in a container image, for example with `ENV` or `ARG` in a Dockerfile.[^6] Keep secret files and `.env` files out of version control with `.gitignore`.

### Find and remove a committed secret

When a secret reaches a repository, the first step is to revoke or rotate it. GitHub's guide says so before it explains any change to the history.[^9] The OWASP cheat sheet also says to revoke exposed keys at once.[^6] Rewriting the history does not help with copies that others already have.

To find where a string entered the history, use `git log -S<string> --all`. The option `-S` finds commits that change the number of times the string occurs, and `--all` searches every ref, not only the current branch.[^12] The option `-G<regex>` finds commits whose diff adds or removes a line that matches a pattern.[^12] The Bash example file runs both on a small repository. Tools such as Gitleaks scan the whole history for many kinds of secrets.[^13]

After the rotation, a team can rewrite the history to remove the value. GitHub recommends `git-filter-repo` with the option `--sensitive-data-removal`. It warns that old commits can stay in clones, in forks, in cached views and in pull requests.[^9]

### Inbound credentials

For an account that users log in to, ship no default password. CWE suggests a first-login mode that requires the user to set a unique strong password or key.[^1] Store passwords as salted one-way hashes, not as plain text, so that a stolen file does not give the passwords.[^1]

## How to detect it

- **Secret scanners.** Gitleaks detects passwords, API keys and tokens in Git repositories and files.[^13] The OWASP cheat sheet recommends detection in a pre-commit hook, and more than one tool, because each tool finds different patterns.[^6] GitHub secret scanning scans the entire Git history on all branches of a repository.[^10]
- **Push protection.** GitHub push protection blocks a push that contains a secret before the secret reaches the repository. A user with write access can bypass the block with a reason, and the bypass is logged.[^11]
- **Pattern linters.** Ruff rules S105, S106 and S107 flag string literals assigned to names, arguments and defaults such as `password`.[^14] gosec rule G101 looks for hard-coded credentials.[^15]
- **Source and binary analysis.** CWE rates manual and automated analysis of source code as highly effective. It rates analysis of binaries as partly effective.[^1]
- **Code review.** Look for literals next to names such as `password`, `secret`, `token` and `key`, and for connection strings with a user and password in them.

## How to prevent it

- **Read credentials at run time.** Use the environment, mounted secret files or a secret manager. Keep the code free of every real value.[^6]
- **Fail when a secret is missing.** Do not fall back to a default credential.
- **Keep secrets out of logs.** Override the string form of configuration objects, as the example files do. Then a log line or an error message cannot print the secret.
- **Scan before and after the commit.** Run a scanner in a pre-commit hook and in CI, and turn on push protection where the host has it.[^6][^11]
- **Use short-lived credentials.** Tokens that expire limit the time that a leaked value works. CWE suggests credentials that are valid only for a limited time.[^1]
- **Rotate after a leak.** Revoke the credential first. Then clean the history, if needed.[^9]
- **Give each installation its own secret.** Generate keys and passwords at install time, never at build time.
- **Limit what each credential can do.** A front end that reaches a back end needs only the rights for its own actions.[^1]

## Real cases

| Case | Date | What happened |
|---|---|---|
| CVE-2010-2772 | published 2010-07-22 | Siemens Simatic WinCC and PCS 7 used a hard-coded password that gave access to a back-end database. The Stuxnet worm used it in the wild in July 2010.[^5] |
| CVE-2012-3503 | published 2012-08-25 | The installer of Katello 1.0 and earlier gave each default installation the same `secret_token` value. Remote attackers could use the known token to authenticate.[^16] |
| CVE-2022-30271 | published 2022-07-26 | The Motorola ACE1000 RTU shipped with a hard-coded SSH private key. Its scripts made another key only if no key file existed, so the shipped key was likely in use.[^17] |

## References
