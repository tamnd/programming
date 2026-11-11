---
tag: NJ9A34
kind: feature
owner: git
slug: branch
name: git branch
aliases: [branch, branching, line of development, bookmark, codeline]
area: tools-and-workflow/version-control
level: core
summary: >-
  A branch is a line of development in a version control system. In Git, a branch is a name that points to one commit, and the name moves to each commit that you make on it.
spec: {standard: null, url: "https://git-scm.com/docs/git-branch#_description", locator: "git-branch(1), DESCRIPTION"}
history:
  year: 2005
  people: [Linus Torvalds]
  note: The tutorial of Git v0.99, in July 2005, makes a branch by writing a commit name into a file under .git/refs/heads, and HEAD is a symbolic link to refs/heads/master.
see_also: [git, git/commit, git/merge, git/rebase]
same_as:
  wikidata: Q925385
  wikipedia: Branching_(version_control)
sources:
  - url: https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Branches in a Nutshell
    locator: Branches in a Nutshell; Creating a New Branch
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitglossary#def_branch
    title: Git documentation, gitglossary(7)
    locator: branch; head
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitglossary#def_HEAD
    title: Git documentation, gitglossary(7)
    locator: HEAD
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-switch#_description
    title: Git documentation, git-switch(1)
    locator: DESCRIPTION; OPTIONS, -c and --detach
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-checkout#_detached_head
    title: Git documentation, git-checkout(1)
    locator: DETACHED HEAD
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-reflog#_description
    title: Git documentation, git-reflog(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge-base#_description
    title: Git documentation, git-merge-base(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitrevisions#_specifying_ranges
    title: Git documentation, gitrevisions(7)
    locator: SPECIFYING RANGES, Dotted Range Notations
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-branch#Documentation/git-branch.txt--d
    title: Git documentation, git-branch(1)
    locator: OPTIONS, -d and -D
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/gitglossary#def_upstream_branch
    title: Git documentation, gitglossary(7)
    locator: upstream branch; remote-tracking branch
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-status#_branch_headers
    title: Git documentation, git-status(1)
    locator: OUTPUT, Short Format and Branch Headers
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.23.0.adoc
    title: Git 2.23 Release Notes
    locator: the two commands git switch and git restore
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.28.0.adoc
    title: Git 2.28 Release Notes
    locator: the configurable name of the first branch in new repositories
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.52.0.adoc
    title: Git 2.52 Release Notes
    locator: git init uses main as the initial branch, not master, starting Git 3.0
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.22.0.adoc
    title: Git 2.22 Release Notes
    locator: git branch learned --show-current
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Internals-Git-References
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Git Internals, Git References
    locator: Git References; The HEAD
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.45.0.adoc
    title: Git 2.45 Release Notes
    locator: the reftable backend, git init --ref-format=reftable
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/NamedBranches
    title: Mercurial wiki (archive), NamedBranches
    locator: Named branches; Create a Branch
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/Bookmarks
    title: Mercurial wiki (archive), Bookmarks
    locator: the introduction (bookmarks compared with Git branches)
    accessed: 2026-11-10
  - url: https://svnbook.red-bean.com/en/1.7/svn.branchmerge.using.html
    title: Version Control with Subversion, for Subversion 1.7
    locator: Chapter 4, Using Branches; Creating a Branch; Cheap Copies
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/branching.wiki
    title: Fossil documentation, Branching, Forking, Merging, and Tagging
    locator: Forking Versus Branching; Tags And Properties
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/bookmarks/
    title: Jujutsu documentation, Bookmarks
    locator: Introduction; Mapping to Git branches
    accessed: 2026-11-10
  - url: https://help.perforce.com/helix-core/server-apps/cmdref/current/Content/CmdRef/p4_integrate.html
    title: P4 CLI Documentation (formerly Helix Core), p4 integrate
    locator: Description (codelines and branching by copy)
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v0.99/Documentation/tutorial.txt#L579-L590
    title: Git v0.99, Documentation/tutorial.txt, July 2005
    locator: lines 54 and 579 to 590 (HEAD as a symbolic link; a branch as a file under .git/refs/heads)
    accessed: 2026-11-10
---

## Synopsis

Work on a project often splits. One person fixes a bug while another builds a feature, and both start from the same commit. A branch is one of these lines of development. Version control systems store branches in two main ways. Some record the branch name in each commit, so the name is part of the history forever. Others keep the name outside the commits, as a pointer that can move or go away. Git uses pointers.[^1] [^2]

```text
git branch                       list the local branches; * marks the current one
git branch <name> [<start>]      make a branch at <start> (default HEAD); do not switch to it
git switch <name>                make <name> the current branch
git switch -c <name> [<start>]   make a branch and switch to it
git switch --detach <commit>     check out a commit with no current branch
git branch -d <name>             delete a branch that is merged
git branch -D <name>             delete a branch even when it is not merged
git branch -m <old> <new>        rename a branch
git branch -v | -vv              show the tip of each branch, and its upstream with -vv
git branch --show-current        print the name of the current branch
git branch -u <remote>/<branch>  set the upstream of the current branch
```

`git checkout <name>` and `git checkout -b <name>` are the older forms of `git switch` and `git switch -c`.[^4]

## Semantics

**r1.** A branch is a ref under `refs/heads/`. A ref is a name that holds the name of one commit. That commit is the tip of the branch.[^2] [^16] Example: [`pointer`](#pointer).

**r2.** Making a branch writes one ref. It copies no file and no commit. Two branches can point to the same commit.[^1] Example: [`pointer`](#pointer).

**r3.** `HEAD` names the current branch. It is a symbolic ref: it holds the name of a branch, such as `ref: refs/heads/main`, not a commit.[^3] [^16] Example: [`pointer`](#pointer).

**r4.** When you commit, Git moves the branch that `HEAD` names to the commit. Other branches stay where they are.[^2]

**r5.** The commits of a branch are all commits that its tip can reach through parent links. So a commit can be on many branches at once. When two branches have different commits, they have diverged. A merge base of the two is a common ancestor that no other common ancestor descends from.[^7] Example: [`diverge`](#diverge).

**r6.** `A..B` means the commits that `B` can reach and `A` cannot. So `git rev-list main..topic` lists the commits that are on `topic` and not yet on `main`.[^8] Example: [`diverge`](#diverge).

**r7.** When `HEAD` holds a commit name instead of a branch name, `HEAD` is detached. Commits that you make there belong to no branch. When you switch away, only the reflog still names them.[^5] [^6] Example: [`detached`](#detached).

**r8.** `git branch -d` deletes a branch only when its tip is merged into its upstream, or into `HEAD` when it has no upstream. `-D` deletes it anyway. Deleting a branch deletes the ref, not the commits.[^9] Example: [`delete`](#delete).

**r9.** A remote-tracking branch, such as `origin/main`, records where a branch of another repository was at the last fetch or push. The upstream of a local branch is the branch that it merges from or rebases onto by default. `git status` compares the local branch with its upstream.[^10] [^11] Example: [`upstream`](#upstream).

## Examples

Each example starts in an empty directory. The `export` lines fix the identity and the dates, so the hashes on this page are the hashes that you get.

### pointer

```bash run id=pointer
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
git --version
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git branch topic
cat .git/HEAD
cat .git/refs/heads/main
cat .git/refs/heads/topic
git branch
git branch --show-current
```

```output for=pointer
git version 2.56.0
ref: refs/heads/main
c6fb135760531277211142d3556246d6d0d6c46d
c6fb135760531277211142d3556246d6d0d6c46d
* main
  topic
main
```

Each branch file holds the 40 digits of one commit name and a newline. `HEAD` holds the name of a branch.

### diverge

```bash run id=diverge
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -c topic
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git switch main
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
git log --graph --oneline --decorate --all
git merge-base main topic
git rev-list --count main..topic
```

```output for=diverge
Switched to a new branch 'topic'
Switched to branch 'main'
* 8489f80 (HEAD -> main) Add a README
| * d54235d (topic) Add bread
|/
* c6fb135 Add the shopping list
c6fb135760531277211142d3556246d6d0d6c46d
1
```

The two branches share `c6fb135`, which is their merge base. `topic` has one commit that `main` does not have.

### detached

```bash run id=detached
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch --detach HEAD
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git switch main
git branch rescue HEAD@{1}
git log --oneline rescue
```

```output for=detached
HEAD is now at c6fb135 Add the shopping list
Warning: you are leaving 1 commit behind, not connected to
any of your branches:

  d54235d Add bread

If you want to keep it by creating a new branch, this may be a good time
to do so with:

 git branch <new-branch-name> d54235d

Switched to branch 'main'
d54235d Add bread
c6fb135 Add the shopping list
```

`HEAD@{1}` is the place where `HEAD` was one move ago. The branch `rescue` gives the lost commit a name again.

### delete

```bash run id=delete
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git switch -q main
git branch -d topic || echo "exit status $?"
git branch -D topic
git cat-file -t d54235d
```

```output for=delete
error: the branch 'topic' is not fully merged
hint: If you are sure you want to delete it, run 'git branch -D topic'
hint: Disable this message with "git config set advice.forceDeleteBranch false"
exit status 1
Deleted branch topic (was d54235d).
commit
```

The commit `d54235d` still exists after the delete. No branch names it now, so a later `git gc` can remove it when the reflog entries expire.

### upstream

```bash run id=upstream
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git -c init.defaultBranch=main init -q --bare ../origin.git
git remote add origin ../origin.git
git push -q -u origin main
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git status -sb
git branch -vv
```

```output for=upstream
## main...origin/main [ahead 1]
* main d54235d [origin/main: ahead 1] Add bread
```

## Versions

Git 2.23 added `git switch` for branch work and `git restore` for file work. Before it, `git checkout` did both.[^12] Git 2.22 added `git branch --show-current`.[^15] Git 2.28 added the setting `init.defaultBranch`, which names the first branch of a repository that `git init` makes.[^13] The release notes of Git 2.52 say that from Git 3.0 that first branch is `main` when nothing else is set, instead of `master`.[^14] The examples on this page set the name, so they do not depend on this default.

## Pitfalls

### Commits on a detached HEAD can get lost

Example `detached` shows the warning. Git only keeps the commit while the reflog names it. Make a branch before you switch away: `git switch -c <name>`.[^5]

### A remote-tracking branch is not live

`origin/main` changes only on `git fetch`, `git pull` or `git push`. When `git status` says "up to date", it means up to date with the last fetch. Someone may have pushed since then. Run `git fetch` before you trust the count.[^10]

### Force-deleting a branch removes the name of its work

`git branch -D` succeeds even when no other branch has the commits. The output prints the old tip, `(was d54235d)`. Keep that name: `git branch <name> d54235d` brings the branch back while the object still exists.[^9]

## Comparison

**Mercurial** has two kinds of branch. A named branch is a field of each changeset, so the name stays in the history for good.[^18] A bookmark is a movable name for a changeset, close to a Git branch.[^19]

**Subversion** has no branch object. By convention, a branch is a directory such as `/branches/my-branch` that `svn copy` makes from `/trunk` on the server. The copy takes constant time and space, because the repository points to the existing tree and does not copy data.[^20]

**Fossil** records branches with tags on check-ins. A propagating `branch` tag applies to a check-in and its direct descendants. Fossil also names a fork: two leaves on the same branch. Autosync tries to prevent forks, and it stops a commit that would make one.[^21]

**Jujutsu** calls its named pointers bookmarks and maps them to Git branches. A bookmark moves when its commit is rewritten, but there is no current bookmark, so a bookmark does not move when you make a commit.[^22]

**Perforce** (P4) calls a line of development a codeline. A codeline is a set of files under a depot path, and you make one by copying another codeline with `p4 integrate`, `p4 copy` or `p4 populate`.[^23]

## Implementation notes

The model has not changed since 2005. The Git v0.99 tutorial makes a branch with `cat .git/HEAD > .git/refs/heads/mybranch`, and `HEAD` is a symbolic link to `refs/heads/master`.[^24] Git later made `HEAD` a small file that holds `ref: refs/heads/...`, as example `pointer` shows. The files backend stores each branch as a file in `.git/refs/heads/`, and `git pack-refs` can move many refs into the one file `.git/packed-refs`.[^16]

Git 2.45 added a second backend, reftable, which you choose with `git init --ref-format=reftable`. The release notes say that it is meant to manage repositories with many refs more efficiently.[^17] With reftable, the ref files are only placeholders, so a script must ask Git for a ref and not read the file:

```bash run id=reftable
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q --ref-format=reftable shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
cat .git/HEAD
cat .git/refs/heads
git symbolic-ref HEAD
git rev-parse main
```

```output for=reftable
ref: refs/heads/.invalid
this repository uses the reftable format
refs/heads/main
c6fb135760531277211142d3556246d6d0d6c46d
```

## References
