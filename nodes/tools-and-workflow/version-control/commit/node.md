---
tag: QFRVVY
kind: feature
owner: git
slug: commit
name: git commit
aliases: [commit, revision, changeset, check-in, changelist]
area: tools-and-workflow/version-control
level: core
summary: >-
  A commit is a saved snapshot of the whole project, with an author, a date, a message and links to its parent commits. Git names each commit by the hash of its bytes.
spec: {standard: null, url: "https://git-scm.com/docs/git-commit#_description", locator: "git-commit(1), DESCRIPTION"}
history:
  year: 2005
  people: [Linus Torvalds]
  note: The first commit of Git, on 7 April 2005, describes blob, tree and changeset objects in a content-addressable object store. Each object is named by the SHA-1 hash of its content.
see_also: [git, git/branch, git/merge, git/rebase]
same_as:
  wikidata: Q20058545
  wikipedia: Commit_(version_control)
sources:
  - url: https://git-scm.com/book/en/v2/Getting-Started-What-is-Git%3F
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, What is Git?
    locator: Snapshots, Not Differences
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-commit#_description
    title: Git documentation, git-commit(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Internals-Git-Objects
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Git Internals, Git Objects
    locator: Tree Objects; Commit Objects; Object Storage
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitglossary#def_commit_object
    title: Git documentation, gitglossary(7)
    locator: commit object; parent
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-hash-object#_description
    title: Git documentation, git-hash-object(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/hash-function-transition#_background
    title: Git documentation, hash-function-transition
    locator: Background; Objective
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-commit#Documentation/git-commit.txt---amend
    title: Git documentation, git-commit(1)
    locator: OPTIONS, --amend
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-reflog#_description
    title: Git documentation, git-reflog(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Internals-Packfiles
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Git Internals, Packfiles
    locator: Packfiles
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitformat-pack#_deltified_representation
    title: Git documentation, gitformat-pack(5)
    locator: Deltified representation
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.13.0.adoc
    title: Git 2.13 Release Notes
    locator: the SHA-1 implementation that detects attempts to create collisions, by Marc Stevens and Dan Shumow, made the default
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.29.0.adoc
    title: Git 2.29 Release Notes
    locator: SHA-256 support marked as experimental in the documentation
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-init#Documentation/git-init.txt---object-formatformat
    title: Git documentation, git-init(1)
    locator: OPTIONS, --object-format
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/Revlog
    title: Mercurial wiki, Revlog
    locator: the first section (full versions or compressed deltas)
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/RevisionNumber
    title: Mercurial wiki, RevisionNumber
    locator: the first section (a strictly local identifier)
    accessed: 2026-11-10
  - url: https://svnbook.red-bean.com/en/1.7/svn.basic.in-action.html#svn.basic.in-action.revs
    title: Version Control with Subversion, for Subversion 1.7
    locator: Chapter 1, Revisions
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/hashpolicy.wiki
    title: Fossil documentation, Hash Policy
    locator: the history of SHA1 and SHA3-256 artifact names
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/fossil-v-git.wiki
    title: Fossil documentation, Fossil Versus Git
    locator: 2.0 Differences Between Fossil And Git, Sync over push
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/git-comparison/
    title: Jujutsu documentation, Comparison with Git
    locator: Overview, the working copy is automatically committed
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/glossary/
    title: Jujutsu documentation, Glossary
    locator: Change ID; Commit ID
    accessed: 2026-11-10
  - url: https://help.perforce.com/helix-core/server-apps/cmdref/current/Content/CmdRef/p4_submit.html
    title: P4 CLI Documentation (formerly Helix Core), p4 submit
    locator: Description (the atomic submit of a changelist)
    accessed: 2026-11-10
  - url: https://github.com/git/git/commit/e83c5163316f89bfbde7d9ab23ca2e25604af290
    title: Linus Torvalds, Initial revision of "git", the information manager from hell, commit e83c516 of git/git, 7 April 2005
    locator: README, the description of the object database
    accessed: 2026-11-10
---

## Synopsis

A version control system keeps the history of a set of files. The unit of that history is the commit: one recorded state of all the files, with a note about who made it, when and why. Other systems call it a revision, a changeset, a check-in or a changelist. There are two ways to store such a history. A delta-based system stores each file as a list of changes over time. A snapshot-based system stores the whole state of the project at each commit, and finds the changes by comparing two snapshots. Git works the second way.[^1]

The forms of `git commit` that this page uses:

```text
git commit -m <message>          record the index as a commit, with this message
git commit -a -m <message>       first stage every change to a tracked file, then commit
git commit <path>...             commit the listed tracked files as they are in the working tree
git commit --amend [--no-edit]   replace the tip of the branch with a corrected commit
git commit --fixup <commit>      make a "fixup!" commit for a later rebase --autosquash
git commit --allow-empty -m <m>  record a commit whose tree equals its parent's tree
```

The commands that read a commit back:

```text
git cat-file -p <commit>         print the raw commit object
git show <commit>                print the message and the diff against the parent
git log [--oneline] [--graph]    walk the history back through the parent links
```

## Semantics

**r1.** `git commit` records the content of the index, not of the working tree. The index is the staging area that `git add` fills. A change that is in the working tree but not in the index is not part of the commit. The `-a` option stages every change to a file that Git already tracks, and then commits.[^2] Example: [`first-commit`](#first-commit).

**r2.** A commit object holds the name of one tree object, the names of zero or more parent commits, an author line, a committer line and a message. The tree is the top directory of the snapshot. Each author and committer line has a name, an email address, a time in seconds since 1970 and a time zone offset.[^3] [^4] Example: [`first-commit`](#first-commit).

**r3.** The first commit of a repository has no parent. An ordinary commit has one parent. A merge commit has two or more. The parent links point back in time, so the commits form a directed acyclic graph.[^4]

**r4.** The name of an object is a hash of its type, its size in bytes, a null byte and its content. In a default repository the hash is SHA-1, and the name is 40 hexadecimal digits.[^3] [^5] Example: [`hash`](#hash).

**r5.** Because the commit object holds the names of its tree and its parents, the name of a commit depends on the whole snapshot and on the whole history behind it. A change to any file, to the message, to a date or to any earlier commit gives a different name.[^6] Example: [`hash`](#hash).

**r6.** A commit object never changes. `git commit --amend` makes another commit with the same parents and author, and moves the branch to it. The replaced commit stays in the object store, and the reflog still records it.[^7] [^8] Example: [`hash`](#hash).

**r7.** After the commit, the current branch points to it. The commit is a child of the commit that `HEAD` named before. When `HEAD` is detached, `HEAD` itself moves and no branch changes.[^2]

**r8.** A snapshot does not mean a full copy on disk. A file that did not change has the same blob, and both trees name that one blob. When Git packs objects, it can store an object as a delta against a similar object.[^9] [^10] Example: [`storage`](#storage).

## Examples

The examples on this page ran with the Git shown in the first output. Each one starts in an empty directory. The `export` lines fix the identity and the dates, so the hashes on this page are the hashes that you get. `GIT_CONFIG_GLOBAL=/dev/null` keeps your own settings out of the run.

### first-commit

```bash run id=first-commit
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
git --version
printf 'apples\nbread\n' > list.txt
git add list.txt
git commit -m "Add the shopping list"
printf 'milk\n' >> list.txt
git commit -a -m "Add milk"
git cat-file -p HEAD
```

```output for=first-commit
git version 2.56.0
[main (root-commit) e2e401f] Add the shopping list
 1 file changed, 2 insertions(+)
 create mode 100644 list.txt
[main da1f0d3] Add milk
 1 file changed, 1 insertion(+)
tree ccf3dd9e458e303832d744e188ddedae774c3686
parent e2e401f61d550716e959afda92e41bba5a84c675
author Ada <ada@example.com> 1767607200 +0000
committer Ada <ada@example.com> 1767607200 +0000

Add milk
```

The second commit names one tree and one parent. The parent is the first commit, `e2e401f`. The number `1767607200` is the time 2026-01-05 10:00:00 UTC in seconds since 1970.

### hash

The name of a commit is the hash of its bytes. `git hash-object` computes the hash of the raw commit again, and the result is the same name. A change of one second in the committer date gives a different commit.

```bash run id=hash
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git rev-parse HEAD
git cat-file commit HEAD | git hash-object -t commit --stdin
git cat-file -s HEAD
GIT_COMMITTER_DATE=2026-01-05T10:00:01Z git commit -q --amend --no-edit
git rev-parse HEAD
git reflog
```

```output for=hash
e2e401f61d550716e959afda92e41bba5a84c675
e2e401f61d550716e959afda92e41bba5a84c675
164
393bf1779a4d6bcfbc7ea156957fe72eb6dea193
393bf17 HEAD@{0}: commit (amend): Add the shopping list
e2e401f HEAD@{1}: commit (initial): Add the shopping list
```

The commit object is 164 bytes long. The amend did not change the first commit. It made a second commit, and the reflog keeps both names.

### storage

Two commits share every file that did not change. Here `list.txt` has the same blob, `950a188`, in both trees. Before packing, the repository holds 7 loose objects: 2 commits, 2 trees and 3 blobs. After `git gc`, the pack stores the old `numbers.txt` as a delta of 17 bytes against the changed one, at depth 1.

```bash run id=storage
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
seq 1 2000 > numbers.txt
printf 'apples\n' > list.txt
git add .
git commit -q -m "Add two files"
sed 's/^1000$/one thousand/' numbers.txt > tmp && mv tmp numbers.txt
git commit -q -a -m "Spell out one thousand"
git ls-tree HEAD~1
git ls-tree HEAD
git count-objects -v | head -1
git gc -q
git verify-pack -v .git/objects/pack/pack-*.idx | grep blob
```

```output for=storage
100644 blob 950a188f0a4224ce6a1ac710915113df972aeb2c	list.txt
100644 blob 7972c09aa90a9b3d8519064681f2cca009f8777c	numbers.txt
100644 blob 950a188f0a4224ce6a1ac710915113df972aeb2c	list.txt
100644 blob dbd76b1c15c7f2996384aad22a122e845315cbd9	numbers.txt
count: 7
950a188f0a4224ce6a1ac710915113df972aeb2c blob   7 16 282
dbd76b1c15c7f2996384aad22a122e845315cbd9 blob   8901 4235 298
7972c09aa90a9b3d8519064681f2cca009f8777c blob   17 29 4693 1 dbd76b1c15c7f2996384aad22a122e845315cbd9
```

So Git has a snapshot model and delta storage at the same time. The model decides what a commit means. The storage only decides how many bytes it takes.

## Versions

The commit format has not changed since 2005. Two changes concern the hash. Git 2.13 made the default SHA-1 code one that detects the known collision attack.[^11] Git 2.29 can create SHA-256 repositories with `git init --object-format=sha256`, and the documentation marks this as experimental.[^12] A SHA-256 repository cannot exchange objects with a SHA-1 repository.[^13]

## Pitfalls

### A change that is not staged is not committed

`git commit` with no option records the index. A file that you changed but did not add stays out, and Git refuses to make a commit that changes nothing.

```bash run id=unstaged
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
printf 'bread\n' >> list.txt
git commit -m "Add bread" || echo "exit status $?"
```

```output for=unstaged
On branch main
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
	modified:   list.txt

no changes added to commit (use "git add" and/or "git commit -a")
exit status 1
```

Run `git add list.txt` first, or use `git commit -a`.

### A later commit does not remove a file from history

Each commit keeps its own snapshot. When you commit a secret and then delete it in the next commit, the older snapshot still holds it. Any clone of the repository can read it back.

```bash wrong id=secret
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'token=s3cr3t\n' > settings.ini
git add settings.ini
git commit -q -m "Add the settings"
git rm -q settings.ini
git commit -q -m "Remove the settings"
git ls-tree HEAD
git show HEAD~1:settings.ini
```

```output for=secret
token=s3cr3t
```

The tree of `HEAD` is empty, but `HEAD~1` still has the token. Treat a committed secret as public and replace it. Removing it from history means rewriting every later commit, which gives every later commit a different name (r5).

### Amend rewrites a commit that others may have

`--amend` gives the tip of the branch a different name (r6). When the replaced commit was already pushed, other people still have it. The rebase page explains what happens then.

## Comparison

**Mercurial** also records each commit, called a changeset, under a 40-digit hexadecimal ID. It stores each file in a revlog, as a full version or as a compressed delta against the version before, so it is delta-based on disk.[^14] It also gives each changeset a short revision number, which is local to one repository and can differ between clones.[^15]

**Subversion** has one central repository. Each commit gets the next global revision number, and each revision number names a tree that is a snapshot of the whole repository. The server assigns the number, so a commit needs the server.[^16]

**Fossil** calls a commit a check-in and stores it in an SQLite database. Fossil first named its artifacts by SHA1 hashes. Fossil 2.0 learned to read SHA3-256 names, and a repository made with `fossil new` uses them by default. Old check-ins keep their SHA1 names.[^17] By default, autosync sends each commit at once to the repository that it was cloned from.[^18]

**Jujutsu** commits the working copy automatically, so there is no staging area and no separate commit step.[^19] Each commit has a commit ID, which is a hash like Git's, and a change ID. The change ID stays the same when the commit is rewritten.[^20]

**Perforce** (P4) groups the files of a commit in a changelist. The server numbers the changelist, and `p4 submit` saves all of its files in the depot or none of them.[^21]

## Implementation notes

The first commit of Git, `e83c516` of 7 April 2005, already describes an object store that names each object by the SHA-1 hash of its content, with blobs, trees and a changeset object that holds a tree and its parents.[^22] A commit is stored as a loose object in `.git/objects` until `git gc` or a transfer packs it. The pack format stores some objects as deltas against other objects in the same pack, with two kinds of base: an offset in the pack or an object name.[^10]

## References
