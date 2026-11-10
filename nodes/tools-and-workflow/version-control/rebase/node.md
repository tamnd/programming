---
tag: NEW-git-rebase
kind: feature
owner: git
slug: rebase
name: git rebase
aliases: [rebase, rebasing, history rewriting, interactive rebase, transplant]
area: tools-and-workflow/version-control
level: core
summary: >-
  A rebase replays a series of commits on top of another commit. The changes stay the same, but each replayed commit has a different parent, so it is a different commit with a different hash.
spec: {standard: null, url: "https://git-scm.com/docs/git-rebase#_description", locator: "git-rebase(1), DESCRIPTION"}
history:
  year: 2005
  people: [Junio C Hamano]
  note: Git got git-rebase-script on 2005-06-25, before Git v0.99. It used git-cherry to find the local commits that the upstream head did not have, and forward ported them.
see_also: [git, git/commit, git/branch, git/merge]
sources:
  - url: https://git-scm.com/docs/git-rebase#_description
    title: Git documentation, git-rebase(1)
    locator: DESCRIPTION; OPTIONS, --committer-date-is-author-date and --reset-author-date; BEHAVIORAL DIFFERENCES
    accessed: 2026-11-10
  - url: https://git-scm.com/book/en/v2/Git-Branching-Rebasing
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Rebasing
    locator: The Basic Rebase; The Perils of Rebasing; Rebase vs. Merge
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-commit#_commit_information
    title: Git documentation, git-commit(1)
    locator: COMMIT INFORMATION
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rebase#_transplanting_a_topic_branch_with_onto
    title: Git documentation, git-rebase(1)
    locator: TRANSPLANTING A TOPIC BRANCH WITH --ONTO
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rebase#_interactive_mode
    title: Git documentation, git-rebase(1)
    locator: INTERACTIVE MODE; OPTIONS, --autosquash
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-commit#Documentation/git-commit.txt---fixupamendrewordcommit
    title: Git documentation, git-commit(1)
    locator: OPTIONS, --fixup
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rebase#Documentation/git-rebase.txt---update-refs
    title: Git documentation, git-rebase(1)
    locator: OPTIONS, --update-refs
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rebase#_rebasing_merges
    title: Git documentation, git-rebase(1)
    locator: REBASING MERGES; OPTIONS, --rebase-merges
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rebase#_recovering_from_upstream_rebase
    title: Git documentation, git-rebase(1)
    locator: RECOVERING FROM UPSTREAM REBASE
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-range-diff#_description
    title: Git documentation, git-range-diff(1)
    locator: DESCRIPTION; OUTPUT STABILITY
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-reflog#_description
    title: Git documentation, git-reflog(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.18.0.adoc
    title: Git 2.18 Release Notes
    locator: git rebase learned --rebase-merges
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.22.0.adoc
    title: Git 2.22 Release Notes
    locator: --rebase-merges replaces --preserve-merges, which is marked as deprecated
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.26.0.adoc
    title: Git 2.26 Release Notes
    locator: Backward compatibility notes (the merge backend by default; rebase.backend=apply)
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.34.0.adoc
    title: Git 2.34 Release Notes
    locator: Backward compatibility notes (--preserve-merges removed)
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.38.0.adoc
    title: Git 2.38 Release Notes
    locator: the --update-refs option of git rebase
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.44.0.adoc
    title: Git 2.44 Release Notes
    locator: --autosquash for non-interactive rebase; the git replay command
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/RebaseExtension
    title: Mercurial wiki (archive), RebaseExtension
    locator: Configuration (the [extensions] section)
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/Phases
    title: Mercurial wiki (archive), Phases
    locator: Introduction; Available Phases
    accessed: 2026-11-10
  - url: https://svnbook.red-bean.com/en/1.7/svn.branchmerge.basicmerging.html#svn.branchemerge.basicmerging.stayinsync
    title: Version Control with Subversion, for Subversion 1.7
    locator: Chapter 4, Basic Merging, Keeping a Branch in Sync
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/rebaseharm.md
    title: Fossil documentation, Rebase Considered Harmful
    locator: 5.0 Rebase misrepresents the project history
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/fossil-v-git.wiki
    title: Fossil documentation, Fossil Versus Git
    locator: 2.7 What you should have done vs. What you actually did (amendments; no rebase, on purpose)
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/git-comparison/
    title: Jujutsu documentation, Comparison with Git
    locator: Overview (descendant commits are automatically rebased)
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/conflicts/
    title: Jujutsu documentation, First-class conflicts
    locator: Introduction (a rebase with a conflict succeeds and records the conflict)
    accessed: 2026-11-10
  - url: https://help.perforce.com/helix-core/server-apps/cmdref/current/Content/CmdRef/p4_resolve.html
    title: P4 CLI Documentation (formerly Helix Core), p4 resolve
    locator: Description (a submit fails until the user resolves the revision that another user submitted)
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-push#Documentation/git-push.txt---force-with-lease
    title: Git documentation, git-push(1)
    locator: OPTIONS, --force-with-lease
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rerere#_description
    title: Git documentation, git-rerere(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/glossary/
    title: Jujutsu documentation, Glossary
    locator: Rewrite; Change ID
    accessed: 2026-11-10
  - url: https://github.com/git/git/commit/59e6b23acef9d29b9bdabc38ee80361e19ef7ebe
    title: "git/git commit 59e6b23, git-rebase-script: rebase local commits to new upstream head"
    locator: commit message, 2005-06-25
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-replay#_description
    title: Git documentation, git-replay(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
---

## Synopsis

A branch often starts from an old commit while the main line goes on. There are two ways to bring the branch up to date. A merge keeps both lines and joins them with a merge commit. A rebase takes the commits of the branch and makes them again on top of the main line, so the history becomes one line. A commit names its parent, and the name of a commit is the hash of its content (see the commit page). So a replayed commit with the same change but a different parent is a different commit with a different name. A rebase does not move commits. It makes copies and moves the branch to the copies.[^1] [^2]

```text
git rebase <upstream>                       replay the commits of HEAD that are not in <upstream> on top of it
git rebase <upstream> <branch>              switch to <branch> first, then rebase it
git rebase --onto <newbase> <upstream>      replay the commits after <upstream> onto <newbase>
git rebase -i <upstream>                    edit the list of commits to replay before it starts
git rebase --autosquash <upstream>          move fixup! and squash! commits next to their targets
git rebase --update-refs <upstream>         also move branches that point into the replayed range
git rebase --rebase-merges <upstream>       keep the merge commits and the shape of the branch
git rebase --continue | --skip | --abort    go on after a conflict, drop this commit, or stop
git range-diff <upstream> <before> <after>  compare two versions of a series of commits
```

The words of an interactive rebase list:

| Command | Effect |
|---|---|
| `pick` | use the commit |
| `reword` | use the commit, and edit its message |
| `edit` | use the commit, and stop so that you can amend it |
| `squash` | meld into the commit before it, and combine the messages |
| `fixup` | meld into the commit before it, and keep only the message of the earlier commit |
| `drop` | remove the commit |
| `exec` | run a shell command |
| `break` | stop here; `git rebase --continue` goes on |
| `label`, `reset`, `merge` | name a point, go back to it, and make a merge; used by `--rebase-merges` |

## Semantics

**r1.** `git rebase <upstream>` lists the commits of the current branch that are not in `<upstream>`, and leaves out each commit whose change is already in `<upstream>`. It then detaches `HEAD` at `<upstream>`, replays the listed commits one by one in order, as `git cherry-pick` would, and moves the branch to the last one.[^1] Example: [`replay`](#replay).

**r2.** Each replayed commit has a different parent and often a different tree. So it has a different name, even when its diff is the same. The author and the author date stay. The committer comes from the current identity, and the committer date is the current time.[^1] [^3] Example: [`replay`](#replay).

**r3.** The old commits do not change. At the start, `ORIG_HEAD` names the old tip, and the reflog of the branch keeps it as `@{1}`. You can go back with `git reset --hard ORIG_HEAD` while that name is still right.[^1] [^11] Example: [`replay`](#replay).

**r4.** `--onto <newbase> <upstream>` replays the commits that come after `<upstream>` onto `<newbase>`. This moves a branch off a branch that it no longer needs.[^4] Example: [`onto`](#onto).

**r5.** When a commit does not apply, the rebase stops with conflict markers. You resolve the files, run `git add`, and run `git rebase --continue`. `--skip` drops the commit. `--abort` puts the branch back as it was.[^1] Example: [`conflict`](#conflict).

**r6.** `-i` opens the list of commits in an editor before the replay starts, so you can reorder, edit, squash, fix up or drop commits. A commit that `git commit --fixup <commit>` made has a message that starts with `fixup!`. `--autosquash` moves it under its target and marks it `fixup`.[^5] [^6] Example: [`todo`](#todo).

**r7.** By default a rebase drops merge commits and replays the commits of all sides as one line. `--rebase-merges` keeps the merges and replays the shape of the branch.[^8]

**r8.** `--update-refs` moves every other branch that points to a commit in the replayed range to the copy of that commit. This keeps a stack of dependent branches together.[^7] Example: [`stack`](#stack).

## Examples

Each example starts in an empty directory. The `export` lines fix the identity and the dates, so the hashes on this page are the hashes that you get.

### replay

```bash run id=replay
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
git --version
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
printf 'milk\n' >> list.txt
git commit -q -a -m "Add milk"
git switch -q main
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
git switch -q topic
git log --oneline --graph --decorate --all
git rebase main
git log --oneline --graph --decorate --all
git cat-file -p ORIG_HEAD
git cat-file -p HEAD
git range-diff main ORIG_HEAD HEAD
```

```output for=replay
git version 2.56.0
* 8489f80 (main) Add a README
| * 9a7b48a (HEAD -> topic) Add milk
| * d54235d Add bread
|/
* c6fb135 Add the shopping list
Rebasing (1/2)
Rebasing (2/2)
Successfully rebased and updated refs/heads/topic.
* 7bcbf3c (HEAD -> topic) Add milk
* 88d42ef Add bread
* 8489f80 (main) Add a README
* c6fb135 Add the shopping list
tree ccf3dd9e458e303832d744e188ddedae774c3686
parent d54235db942477efc7fb63817b7c3be12fe87013
author Ada <ada@example.com> 1767607200 +0000
committer Ada <ada@example.com> 1767607200 +0000

Add milk
tree ad6fbdc7ef36850e0fda5ebdbc7745a0382a9f26
parent 88d42efff49539e18095b933b501f7e1c3a3f601
author Ada <ada@example.com> 1767607200 +0000
committer Ada <ada@example.com> 1767607200 +0000

Add milk
1:  d54235d = 1:  88d42ef Add bread
2:  9a7b48a = 2:  7bcbf3c Add milk
```

`Add bread` was `d54235d` and is now `88d42ef`. `Add milk` was `9a7b48a` and is now `7bcbf3c`. The two `Add milk` commits have the same author, committer and message. They differ in two lines: the tree, because the copy also has the README, and the parent. `git range-diff` pairs the old and the copied commits, and `=` means that their changes are the same.[^10]

### onto

`fix` started from `base-work`, but it does not need the bread commit. `--onto main base-work fix` replays only the commits after `base-work`.

```bash run id=onto
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c base-work
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git switch -q -c fix
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
git log --oneline --graph --decorate --all
git rebase --onto main base-work fix
git log --oneline --graph --decorate --all
```

```output for=onto
* 9259b65 (HEAD -> fix) Add a README
* d54235d (base-work) Add bread
* c6fb135 (main) Add the shopping list
Rebasing (1/1)
Successfully rebased and updated refs/heads/fix.
* d54235d (base-work) Add bread
| * 8489f80 (HEAD -> fix) Add a README
|/
* c6fb135 (main) Add the shopping list
```

### conflict

```bash run id=conflict
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
sed 's/^bread$/rye bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for rye bread"
git switch -q main
sed 's/^bread$/white bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for white bread"
git switch -q topic
git rebase main || echo "exit status $?"
git status --short
printf 'apples\nwhite bread\nrye bread\n' > list.txt
git add list.txt
GIT_EDITOR=true git rebase --continue
git log --oneline --graph --decorate --all
```

```output for=conflict
Rebasing (1/1)
Auto-merging list.txt
CONFLICT (content): Merge conflict in list.txt
error: could not apply 4c42593... Ask for rye bread
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
hint: Disable this message with "git config set advice.mergeConflict false"
Could not apply 4c42593... # Ask for rye bread
exit status 1
UU list.txt
[detached HEAD c6d70ae] Ask for rye bread
 1 file changed, 1 insertion(+)
Successfully rebased and updated refs/heads/topic.
* c6d70ae (HEAD -> topic) Ask for rye bread
* c0d1ef6 (main) Ask for white bread
* e2e401f Add the shopping list
```

`GIT_EDITOR=true` accepts the commit message as it is. In a terminal, Git opens your editor at this point.

### todo

The first commit has a typo, `bred`. A fixup commit corrects it. The sequence editor here only prints the list, so the rebase uses it as Git wrote it.

```bash run id=todo
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
printf 'bred\n' >> list.txt
git commit -q -a -m "Add bread"
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
sed 's/^bred$/bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a --fixup HEAD~1
git log --oneline
GIT_SEQUENCE_EDITOR="grep -v -e '^#' -e '^$'" git rebase -i --autosquash main
git log --oneline
```

```output for=todo
836ddf2 fixup! Add bread
9a2cd35 Add a README
7fadb6c Add bread
c6fb135 Add the shopping list
pick 7fadb6c # Add bread
fixup 836ddf2 # fixup! Add bread
pick 9a2cd35 # Add a README
Rebasing (2/3)
Rebasing (3/3)
Successfully rebased and updated refs/heads/topic.
9259b65 Add a README
d54235d Add bread
c6fb135 Add the shopping list
```

`--autosquash` put the `fixup!` commit after `Add bread`. After the rebase, `Add bread` adds the right word and has the hash `d54235d`, which is the hash of the same commit made without the typo in the other examples.

### stack

`part-2` is built on `part-1`. One rebase of `part-2` with `--update-refs` moves both branches.

```bash run id=stack
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c part-1
printf 'bread\n' >> list.txt
git commit -q -a -m "Add bread"
git switch -q -c part-2
printf 'milk\n' >> list.txt
git commit -q -a -m "Add milk"
git switch -q main
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
git switch -q part-2
git rebase --update-refs main
git log --oneline --graph --decorate --all
```

```output for=stack
Rebasing (1/3)
Rebasing (2/3)
Rebasing (3/3)
Successfully rebased and updated refs/heads/part-2.
Updated the following refs with --update-refs:
	refs/heads/part-1
* 7bcbf3c (HEAD -> part-2) Add milk
* 88d42ef (part-1) Add bread
* 8489f80 (main) Add a README
* c6fb135 Add the shopping list
```

## Versions

Git 2.18 added `--rebase-merges`.[^12] Git 2.22 marked the older `--preserve-merges` as deprecated, and Git 2.34 removed it. Scripts that use `--preserve-merges` must change to `--rebase-merges`.[^13] [^15] Git 2.26 made the merge backend the default. The old backend, based on patches, is still there as `--apply` or `rebase.backend=apply`, and a few details differ between the two.[^14] Git 2.38 added `--update-refs`.[^16] Git 2.44 made `--autosquash` work without `-i`, and added `git replay`, a command that replays commits without a working tree and is meant for servers.[^17]

## Pitfalls

### Do not rebase commits that other people have

A colleague bases work on your branch. You rebase the branch and push it. The colleague merges the rebased branch. Now the history has two copies of the same change.

```bash wrong id=shared
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
git switch -q -c colleague
printf 'Shopping\n' > README
git add README
git commit -q -m "Add a README"
git switch -q main
printf 'eggs\n' > other.txt
git add other.txt
git commit -q -m "Add eggs"
git switch -q topic
git rebase -q main
git switch -q colleague
git merge -q --no-edit topic
git log --oneline --graph
```

```output for=shared
*   d585907 Merge branch 'topic' into colleague
|\
| * 1c79cae Add bread
| * 01a9f8f Add eggs
* | 9259b65 Add a README
* | d54235d Add bread
|/
* c6fb135 Add the shopping list
```

`Add bread` is in the history twice, as `d54235d` and as `1c79cae`. Pro Git states the rule: "Do not rebase commits that exist outside your repository and that people may have based work on."[^2] When it has happened, the people downstream must rebase their work onto the rewritten branch.[^9]

### A force push can delete work

A rebased branch does not descend from the old one, so `git push` refuses it, and you need `--force`. A plain `--force` also deletes any commits that someone else pushed to that branch in the meantime. `git push --force-with-lease` fails when the remote branch is not where your remote-tracking branch says it is.[^26]

### Each replayed commit is untested

A rebase checks only that each patch applies. It does not build or test the commits that it makes, and a commit in the middle can fail to build. `git rebase --exec "<test command>"` runs a command after each commit and stops at the first one that fails.[^5]

### The same conflict can come back many times

A merge meets each conflict once. A rebase replays commit by commit, so a long branch that touches the same lines again and again can stop at each of those commits. `git rerere` can record a resolution and reuse it when the same conflict comes back.[^27]

## Comparison

**Mercurial** ships a rebase extension, which you turn on in the `[extensions]` section of the configuration.[^18] Mercurial 2.1 added phases. A changeset that was shared is public and immutable, and the history rewriting extensions refuse to delete it. So Mercurial enforces the rule of the first pitfall by default.[^19]

**Subversion** has no rebase. To keep a branch up to date, you merge trunk into it with `svn merge`.[^20]

**Fossil** has no rebase, on purpose. Its authors argue that a rebase loses the record of what really happened.[^21] To correct a check-in, Fossil adds a correction record. The correction applies from the amendment point forward, and the original data stays in the repository.[^22]

**Jujutsu** rebases the descendants of a commit automatically when you rewrite that commit, and the change ID of each commit generally stays the same.[^23] [^28] A rebase with a conflict does not stop. Jujutsu records the conflict in the rebased commit, and you resolve it later.[^24]

**Perforce** (P4) has no rebase of submitted changelists. When someone submits a later revision of a file that you have open, your submit fails, and you must run `p4 resolve` first. That is a merge into your pending work.[^25]

## Implementation notes

`git rebase` has two backends. Git 2.26 made the merge backend the default. It uses the merge machinery, `ort` by default, with a full copy of each file, so it detects directory renames. The apply backend makes a patch of each commit with `format-patch` and applies it with `am`. Patches carry only line numbers and context, so the apply backend can apply a change at the wrong place when two places in a file look the same.[^1] [^14] The first version, `git-rebase-script` from 2005, was a shell script that found the missing commits with `git-cherry`.[^29] `git rebase` changes the working tree. `git replay` replays a range of commits without the working tree and the index, and updates the refs in one atomic transaction. It came in Git 2.44, and its manual marks it as experimental.[^17] [^30]

## References
