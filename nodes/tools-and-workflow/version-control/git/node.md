---
tag: NEW-git
kind: tool
slug: git
name: Git
aliases: [git, git-scm]
area: tools-and-workflow/version-control
level: core
summary: >-
  Git is a distributed version control system. Each clone holds the full history of a project, and Git stores that history as snapshots that are named by their hashes.
role: [version-control]
licence: gpl-2.0-only
repo: https://github.com/git/git
first_release: 2005-04-07
history:
  year: 2005
  people: [Linus Torvalds, Junio C Hamano]
  note: Linus Torvalds wrote the first version in April 2005, after the Linux kernel project lost its free use of BitKeeper. Junio C Hamano tagged v0.99.2 in July 2005 and has tagged the Git releases since then.
see_also: [git/commit, git/branch, git/merge, git/rebase]
same_as:
  wikidata: Q186055
  wikipedia: Git
sources:
  - url: https://git-scm.com/book/en/v2/Getting-Started-What-is-Git%3F
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, What is Git?
    locator: Snapshots, Not Differences; Nearly Every Operation Is Local; Git Has Integrity; The Three States
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Getting-Started-Installing-Git
    title: Pro Git, 2nd edition, Installing Git
    locator: Installing on Linux; Installing on macOS; Installing on Windows
    accessed: 2026-11-10
  - url: https://git-scm.com/install/mac
    title: git-scm.com, Install for macOS
    locator: Homebrew; MacPorts; Xcode Command Line Tools
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Getting-Started-First-Time-Git-Setup
    title: Pro Git, 2nd edition, First-Time Git Setup
    locator: the system, global and local configuration files; Your Identity
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-status#_short_format
    title: Git documentation, git-status(1)
    locator: OUTPUT, Short Format
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Internals-Git-Objects
    title: Pro Git, 2nd edition, Git Objects
    locator: the blob example; Tree Objects; Commit Objects
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitglossary
    title: Git documentation, gitglossary(7)
    locator: index; working tree; ref; repository
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-revert#_description
    title: Git documentation, git-revert(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-restore#_description
    title: Git documentation, git-restore(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.23.0.adoc
    title: Git 2.23 Release Notes
    locator: the two commands git switch and git restore
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-config
    title: Git documentation, git-config(1)
    locator: COMMANDS; OPTIONS, --show-origin and --show-scope; FILES
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.46.0.adoc
    title: Git 2.46 Release Notes
    locator: the operation mode options of git config replaced with subcommands
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-push#_description
    title: Git documentation, git-push(1)
    locator: DESCRIPTION; OPTIONS, -u
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-clone#_description
    title: Git documentation, git-clone(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/Revlog
    title: Mercurial wiki (archive), Revlog
    locator: the whole page
    accessed: 2026-11-10
  - url: https://svnbook.red-bean.com/en/1.7/svn.basic.in-action.html#svn.basic.in-action.revs
    title: Version Control with Subversion, for Subversion 1.7
    locator: Chapter 1, Revisions
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/fossil-v-git.wiki
    title: Fossil documentation, Fossil Versus Git
    locator: 2.1 Featureful; 2.3 Query Language
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/git-comparison/
    title: Jujutsu documentation, Comparison with Git
    locator: Introduction; Overview
    accessed: 2026-11-10
  - url: https://help.perforce.com/helix-core/server-apps/cmdref/current/Content/CmdRef/p4_submit.html
    title: P4 CLI Documentation (formerly Helix Core), p4 submit
    locator: Description
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Getting-Started-A-Short-History-of-Git
    title: Pro Git, 2nd edition, A Short History of Git
    locator: the whole section
    accessed: 2026-11-10
  - url: https://marc.info/?l=linux-kernel&m=111280216717070
    title: Linus Torvalds, Kernel SCM saga.., linux-kernel mailing list, 2005-04-06
    locator: the whole message
    accessed: 2026-11-10
  - url: https://github.com/git/git/commit/e83c5163316f89bfbde7d9ab23ca2e25604af290
    title: "git/git commit e83c516, Initial revision of \"git\", the information manager from hell"
    locator: commit metadata, 2005-04-07; README
    accessed: 2026-11-10
  - url: https://github.com/git/git/releases/tag/v0.99.2
    title: git/git tag v0.99.2
    locator: tagger and date
    accessed: 2026-11-10
  - url: https://github.com/git/git/releases/tag/v1.0.0
    title: git/git tag v1.0.0
    locator: tagger and date
    accessed: 2026-11-10
  - url: https://github.com/git/git/releases/tag/v2.0.0
    title: git/git tag v2.0.0
    locator: tagger and date
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.29.0.adoc
    title: Git 2.29 Release Notes
    locator: SHA-256 support marked as experimental; the final leg of the SHA-256 transition
    accessed: 2026-11-10
  - url: https://github.com/git/git/releases/tag/v2.56.0
    title: git/git tag v2.56.0
    locator: tagger and date
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/COPYING
    title: Git source tree, COPYING
    locator: the note above the GNU General Public License version 2
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Internals-Git-References
    title: Pro Git, 2nd edition, Git References
    locator: Tags
    accessed: 2026-11-10
---

## What it is

Git is a distributed version control system. It records the history of a directory of files. Each clone of a repository holds the whole history, so most commands work without a network.[^1] Git stores each version as a snapshot of the project, not as a list of changes. Git names each stored object by a hash of its content, so a changed or damaged object gets a different name and Git notices it.[^1] Git is free software under version 2 of the GNU General Public License.[^28]

## Install and first use

On a Debian or Ubuntu system, run `sudo apt install git-all`. On Fedora, run `sudo dnf install git-all`.[^2] On macOS, run `brew install git` with Homebrew, or run `xcode-select --install` to get the Git that comes with the Xcode Command Line Tools.[^3] On Windows, install Git for Windows, a separate project that packages Git.[^2]

Before your first commit, tell Git your name and email. Git writes them into each commit.[^4]

```text
git config set --global user.name "Ada"
git config set --global user.email ada@example.com
```

The examples on this page set the identity and the dates with environment variables instead, so that the hashes are the same each time they run. `git status --short` marks a file that Git does not track with `??`, and a file that is added to the next commit with `A`.[^5]

```bash run id=first-use
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
git --version
printf 'apples\n' > list.txt
git status --short
git add list.txt
git status --short
git commit -m "Add the shopping list"
git log
```

```output for=first-use
git version 2.56.0
?? list.txt
A  list.txt
[main (root-commit) c6fb135] Add the shopping list
 1 file changed, 1 insertion(+)
 create mode 100644 list.txt
commit c6fb135760531277211142d3556246d6d0d6c46d
Author: Ada <ada@example.com>
Date:   Mon Jan 5 10:00:00 2026 +0000

    Add the shopping list
```

## Concepts of the tool

A Git repository has three places for your files. The working tree is the files that you edit. The index, also called the staging area, holds the snapshot that the next commit will record. The Git directory, `.git`, holds the objects and the refs.[^1] [^7]

```bash run id=areas
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
printf 'bread\n' >> list.txt
git add list.txt
printf 'milk\n' >> list.txt
git status --short
git diff
git diff --staged
```

```output for=areas
MM list.txt
diff --git a/list.txt b/list.txt
index 5be50bc..e21221b 100644
--- a/list.txt
+++ b/list.txt
@@ -1,2 +1,3 @@
 apples
 bread
+milk
diff --git a/list.txt b/list.txt
index 950a188..5be50bc 100644
--- a/list.txt
+++ b/list.txt
@@ -1 +1,2 @@
 apples
+bread
```

`MM` means that the file differs between the last commit and the index, and also between the index and the working tree. `git diff` shows the second difference, and `git diff --staged` shows the first one.[^5]

The object database holds four types of object. A blob holds the bytes of one file. A tree lists names, modes and the objects that they point to, so it is one directory. A commit points to one tree and to its parent commits. A tag object holds a tagger, a date, a message and a pointer to another object, usually a commit.[^6] [^29] Refs are names that point to objects. A branch is a ref that moves when you commit. A tag is a ref that stays.[^7] The commit, branch, merge and rebase pages explain these ideas in detail.

```bash run id=objects
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
mkdir docs
printf 'apples\n' > list.txt
printf 'Shopping\n' > docs/README
git add .
git commit -q -m "Add the list and a README"
git cat-file -p HEAD
git cat-file -p 'HEAD^{tree}'
git cat-file -p HEAD:docs
git cat-file -t HEAD:list.txt
git cat-file -p HEAD:list.txt
printf 'apples\n' | git hash-object --stdin
```

```output for=objects
tree b784625c0c88e980304c33247088e1a62014436d
author Ada <ada@example.com> 1767607200 +0000
committer Ada <ada@example.com> 1767607200 +0000

Add the list and a README
040000 tree cef55b8e07c6f20567cd388db7a52944e9c596b6	docs
100644 blob 950a188f0a4224ce6a1ac710915113df972aeb2c	list.txt
100644 blob 6d1077d2c0b9d9a161d4935b6ae07dddf4d44d48	README
blob
apples
950a188f0a4224ce6a1ac710915113df972aeb2c
```

The blob of `list.txt` has the same name as the hash of the bytes `apples` and a newline. The name depends only on the content.

## Common tasks

`git restore <file>` puts back the version of a file from the index and drops your edits.[^9] `git revert <commit>` makes a commit that undoes an earlier commit, and keeps the earlier commit in the history.[^8] `git tag` names a commit, and `git diff` compares two commits.

```bash run id=tasks
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git tag v1
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
printf 'oops\n' > list.txt
git restore list.txt
git revert --no-edit HEAD
git log --oneline --decorate
git diff v1 HEAD
```

```output for=tasks
[main 62a0a6d] Revert "Add bread"
 Date: Mon Jan 5 10:00:00 2026 +0000
 1 file changed, 1 deletion(-)
62a0a6d (HEAD -> main) Revert "Add bread"
d54235d Add bread
c6fb135 (tag: v1) Add the shopping list
```

`git diff v1 HEAD` prints nothing, because the revert brought back the snapshot of `v1`.

To share work, you push commits to another repository and clone or fetch from it. `git push -u` also records the remote branch as the upstream of your branch. `git clone` copies the whole history.[^13] [^14]

```bash run id=remote
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git -c init.defaultBranch=main init -q --bare ../server.git
git remote add origin ../server.git
git push -u origin main
git clone -q ../server.git ../copy
git -C ../copy log --oneline --decorate
```

```output for=remote
To ../server.git
 * [new branch]      main -> main
branch 'main' set up to track 'origin/main'.
c6fb135 (HEAD -> main, origin/main, origin/HEAD) Add the shopping list
```

## Configuration

Git reads configuration from three levels of files. The system file applies to all users. The global file, `~/.gitconfig`, applies to all of your repositories. The local file, `.git/config`, applies to one repository and wins over the others.[^4] Git 2.46 gave `git config` subcommands such as `get`, `set`, `unset` and `list`. The older options such as `--get` still work but are deprecated.[^12] `--show-origin` and `--show-scope` tell you which file a value comes from.[^11]

```bash run id=config
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
git config set user.name "Ada"
git config set alias.lg "log --oneline --graph --decorate"
git config get --show-origin --show-scope user.name
git config get alias.lg
sed -n '/^\[user\]/,$p' .git/config
```

```output for=config
local	file:.git/config	Ada
log --oneline --graph --decorate
[user]
	name = Ada
[alias]
	lg = log --oneline --graph --decorate
```

An alias such as `lg` adds a command: `git lg` runs `git log --oneline --graph --decorate`.[^11]

## Alternatives

**Mercurial** is also a distributed system. It stores the history of each file in a revlog. Each version is stored compressed, either whole or as a delta from the version before it.[^15] **Subversion** is a central system. Clients commit to one repository, and each commit makes a revision with a number.[^16] **Fossil** is a distributed system that stores its objects in a SQLite database, and it adds a wiki, tickets and a forum.[^17] **Jujutsu** is a version control system that can use Git repositories, and it changes how commits, conflicts and the working copy behave.[^18] **Perforce** (P4) is a central system. A user submits a changelist of files to a server.[^19]

## History and versions

The Linux kernel project used BitKeeper, a proprietary tool, from 2002. In 2005 the company behind it took away its free-of-charge status.[^20] On 2005-04-06 Linus Torvalds wrote to the kernel mailing list that he had stopped using BitKeeper and was looking at alternatives.[^21] He made the first commit of Git on 2005-04-07. Its message calls Git "the information manager from hell", and its README calls Git "the stupid content tracker".[^22] Junio C Hamano tagged v0.99.2 on 2005-07-26, and he has tagged the Git releases since then.[^23]

| Version | Date | Notes |
|---|---|---|
| 1.0.0 | 2005-12-21 | first release numbered 1.0[^24] |
| 2.0.0 | 2014-05-28 | first release numbered 2.0[^25] |
| 2.23 | 2019 | added `git switch` and `git restore`, which split the work of `git checkout`[^10] |
| 2.29 | 2020 | finished SHA-256 object names, marked as experimental[^26] |
| 2.46 | 2024 | added subcommands to `git config`[^12] |
| 2.56.0 | 2026-09-27 | the version that ran the examples on this page[^27] |

## References
