---
tag: NEW-git-merge
kind: feature
owner: git
slug: merge
name: git merge
aliases: [merge, three-way merge, 3-way merge, merge commit, fast-forward]
area: tools-and-workflow/version-control
level: core
summary: >-
  A merge joins two lines of development. Git compares both tips with their merge base, keeps the changes of each side, and records the result as a commit with two parents.
spec: {standard: null, url: "https://git-scm.com/docs/git-merge#_description", locator: "git-merge(1), DESCRIPTION"}
history:
  year: 2007
  people: [Sanjeev Khanna, Keshav Kunal, Benjamin C. Pierce]
  note: The paper A Formal Investigation of Diff3 gave a precise model of the three-way merge of text that diff3 and version control systems use, and showed which intuitions about it fail.
see_also: [git, git/commit, git/branch, git/rebase]
same_as:
  wikidata: Q1921621
  wikipedia: Merge_(version_control)
sources:
  - url: https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging
    title: Scott Chacon and Ben Straub, Pro Git, 2nd edition, Basic Branching and Merging
    locator: Basic Merging; Basic Merge Conflicts
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge#_fast_forward_merge
    title: Git documentation, git-merge(1)
    locator: FAST-FORWARD MERGE
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge#_true_merge
    title: Git documentation, git-merge(1)
    locator: TRUE MERGE
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge-base#_description
    title: Git documentation, git-merge-base(1)
    locator: DESCRIPTION; DISCUSSION; OPTIONS, --all
    accessed: 2026-11-10
  - paper: https://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf
    title: Sanjeev Khanna, Keshav Kunal and Benjamin C. Pierce, A Formal Investigation of Diff3, FSTTCS 2007
    locator: The Diff3 Algorithm (stable chunks, chunks changed in A or in B, falsely and truly conflicting chunks)
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge#_how_conflicts_are_presented
    title: Git documentation, git-merge(1)
    locator: HOW CONFLICTS ARE PRESENTED; CONFIGURATION, merge.conflictStyle
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge#_how_to_resolve_conflicts
    title: Git documentation, git-merge(1)
    locator: HOW TO RESOLVE CONFLICTS
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge#_merge_strategies
    title: Git documentation, git-merge(1)
    locator: MERGE STRATEGIES (ort, recursive, resolve, octopus, ours, subtree, and the paragraph on reverted changes)
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-merge-tree#_description
    title: Git documentation, git-merge-tree(1)
    locator: DESCRIPTION; OUTPUT; EXIT STATUS
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.34.0.adoc
    title: Git 2.34 Release Notes
    locator: the ort strategy is used instead of recursive as the default
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.35.0.adoc
    title: Git 2.35 Release Notes
    locator: the zealous diff3 style of conflict presentation
    accessed: 2026-11-10
  - url: https://github.com/git/git/blob/v2.56.0/Documentation/RelNotes/2.38.0.adoc
    title: Git 2.38 Release Notes
    locator: git merge-tree learned a mode that takes two commits and computes the merged tree
    accessed: 2026-11-10
  - url: https://git-scm.com/docs/git-rerere#_description
    title: Git documentation, git-rerere(1)
    locator: DESCRIPTION
    accessed: 2026-11-10
  - url: https://wiki.mercurial-scm.org/Merge
    title: Mercurial wiki (archive), Merge
    locator: the description of the common ancestor, the three-way merge and the commit after it
    accessed: 2026-11-10
  - url: https://svnbook.red-bean.com/en/1.7/svn.branchmerge.basicmerging.html#svn.branchmerge.basicmerging.mergeinfo
    title: Version Control with Subversion, for Subversion 1.7
    locator: Chapter 4, Basic Merging; Merge Tracking; Mergeinfo and Previews
    accessed: 2026-11-10
  - url: https://subversion.apache.org/docs/release-notes/1.5.html#merge-tracking
    title: Apache Subversion 1.5 Release Notes
    locator: Merge tracking (foundational)
    accessed: 2026-11-10
  - url: https://fossil-scm.org/home/doc/trunk/www/branching.wiki
    title: Fossil documentation, Branching, Forking, Merging, and Tagging
    locator: Forking Versus Branching (fossil merge, merge child and direct child)
    accessed: 2026-11-10
  - url: https://docs.jj-vcs.dev/latest/conflicts/
    title: Jujutsu documentation, First-class conflicts
    locator: Introduction
    accessed: 2026-11-10
  - url: https://help.perforce.com/helix-core/server-apps/cmdref/current/Content/CmdRef/p4_resolve.html
    title: P4 CLI Documentation (formerly Helix Core), p4 resolve
    locator: Description (yours, theirs, base and the three-way merge)
    accessed: 2026-11-10
---

## Synopsis

Two people start from the same commit and change the files in different ways. A merge makes one state that holds both sets of changes. A two-way merge that compares only the two results cannot do this well. When one side has a line and the other side does not, it cannot tell whether one side added the line or the other side deleted it. A three-way merge also reads the common ancestor, called the base. For each region of the text, it compares each side with the base. A region that only one side changed takes that change. A region that both sides changed in the same way takes it once. A region that both sides changed in different ways is a conflict, and a person must decide.[^5]

```text
git merge <branch>                   merge <branch> into the current branch
git merge --no-ff <branch>           make a merge commit even when a fast-forward is possible
git merge --ff-only <branch>         fast-forward, or fail
git merge --squash <branch>          stage the combined changes; make no merge commit
git merge --abort                    stop a conflicted merge and go back to the state before it
git merge --continue                 commit the merge after you resolve the conflicts
git merge -s <strategy> <branch>     choose a strategy: ort (default), resolve, octopus, ours, subtree
git merge -X ours|theirs <branch>    resolve conflicting regions from one side
git merge-base [--all] <a> <b>       print the merge base of two commits
git merge-tree --write-tree <a> <b>  compute a merge without touching the index or the working tree
```

## Semantics

**r1.** When the current branch is an ancestor of the other commit, there is nothing to combine. Git moves the branch forward to the other commit and makes no merge commit. This is a fast-forward. `--no-ff` makes a merge commit anyway.[^2] Example: [`ff`](#ff).

**r2.** Otherwise Git finds the merge base. A common ancestor of two commits is a commit that both can reach through parent links. A merge base is a common ancestor that no other common ancestor descends from.[^4] Example: [`three-way`](#three-way).

**r3.** Git merges the tree of the base, the tree of `HEAD` and the tree of the other commit, file by file and region by region, as the synopsis describes. The `ort` strategy also detects renames.[^8] Example: [`three-way`](#three-way).

**r4.** A merge with no conflict makes a merge commit. Its first parent is the old `HEAD`, and its second parent is the merged commit.[^3] Example: [`three-way`](#three-way).

**r5.** When a region conflicts, `HEAD` does not move and `MERGE_HEAD` names the other commit. Each conflicted path has up to three entries in the index: stage 1 is the base, stage 2 is `HEAD` (ours) and stage 3 is the other side (theirs). The file in the working tree holds conflict markers.[^3] Example: [`conflict`](#conflict).

**r6.** You resolve a conflict by writing the right content, staging it with `git add`, and running `git commit` or `git merge --continue`. `git merge --abort` goes back to the state before the merge.[^7] Example: [`resolve`](#resolve).

**r7.** Two commits can have more than one merge base. This happens after a criss-cross merge, where each branch merged the other. In this case `ort` first merges the merge bases into one tree and uses that tree as the base.[^4] [^8] Example: [`criss-cross`](#criss-cross).

**r8.** A merge reads only the two tips and the base. It does not replay the commits in between. So a change that one side made and then reverted counts as no change on that side.[^8] Example: [`reverted`](#reverted).

**r9.** The result is right as text, not as a program. Git does not run or compile the code. A clean merge can still break the program.[^1] Example: [`semantic`](#semantic).

## Examples

Each example starts in an empty directory. The `export` lines fix the identity and the dates, so the hashes on this page are the hashes that you get.

### ff

```bash run id=ff
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
git switch -q main
git merge topic
git log --oneline --graph --decorate
```

```output for=ff
git version 2.56.0
Updating c6fb135..d54235d
Fast-forward
 list.txt | 1 +
 1 file changed, 1 insertion(+)
* d54235d (HEAD -> main, topic) Add bread
* c6fb135 Add the shopping list
```

`main` had no commit of its own, so Git moved it to `d54235d`. There is no merge commit.

### three-way

Each side changes a different line. The base has `apples` and `flour`, `topic` changes the first line, and `main` changes the last line.

```bash run id=three-way
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\ncheese\neggs\nflour\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
sed 's/^apples$/green apples/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for green apples"
git switch -q main
sed 's/^flour$/rye flour/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for rye flour"
git merge-base main topic
git merge --no-edit topic
cat list.txt
git cat-file -p HEAD
git log --oneline --graph
```

```output for=three-way
6dffd3f0b498d46e63a1c1fb19f3e4ab470f69d6
Auto-merging list.txt
Merge made by the 'ort' strategy.
 list.txt | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
green apples
bread
cheese
eggs
rye flour
tree 52318978d579eff646b97b011cadf4288d7f57e0
parent b571462977255ec5bb79439b375606a61486c77d
parent 4b6d64de66ee4c2997cc4fdd903f151ee7ea8ecc
author Ada <ada@example.com> 1767607200 +0000
committer Ada <ada@example.com> 1767607200 +0000

Merge branch 'topic'
*   b4d875d Merge branch 'topic'
|\
| * 4b6d64d Ask for green apples
* | b571462 Ask for rye flour
|/
* 6dffd3f Add the shopping list
```

The merge commit has two `parent` lines. The first is the old tip of `main`.

### conflict

Both sides change the same line, `bread`, in different ways.

```bash run id=conflict
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\nmilk\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
sed 's/^bread$/rye bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for rye bread"
git switch -q main
sed 's/^bread$/white bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for white bread"
git merge topic || echo "exit status $?"
cat list.txt
git status --short
git ls-files --stage
git checkout --conflict=zdiff3 list.txt
cat list.txt
```

```output for=conflict
Auto-merging list.txt
CONFLICT (content): Merge conflict in list.txt
Automatic merge failed; fix conflicts and then commit the result.
exit status 1
apples
<<<<<<< HEAD
white bread
=======
rye bread
>>>>>>> topic
milk
UU list.txt
100644 e21221bd88a090240b4b8ecb2300a7c3755f1b6b 1	list.txt
100644 87bc9043c5b51bcc8bc2c48fde311ca1cfb67663 2	list.txt
100644 56a695742d8dfec50231cf27e4d4f50dcc439de2 3	list.txt
Recreated 1 merge conflict
apples
<<<<<<< ours
white bread
||||||| base
bread
=======
rye bread
>>>>>>> theirs
milk
```

`git merge` exits with status 1. The default markers show ours and theirs. The three index entries are the base (stage 1), ours (stage 2) and theirs (stage 3). `git checkout --conflict=zdiff3` writes the file again in the zdiff3 style, which also shows the base text after `|||||||`.[^6] Set `merge.conflictStyle` to `zdiff3` to get this style for every merge.

### resolve

```bash run id=resolve
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\nmilk\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
sed 's/^bread$/rye bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for rye bread"
git switch -q main
sed 's/^bread$/white bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for white bread"
git merge -q topic
printf 'apples\nrye bread\nwhite bread\nmilk\n' > list.txt
git add list.txt
git commit -q --no-edit
git log --oneline --graph
git merge-tree --write-tree main~1 topic || echo "exit status $?"
```

```output for=resolve
Auto-merging list.txt
CONFLICT (content): Merge conflict in list.txt
Automatic merge failed; fix conflicts and then commit the result.
*   99fc490 Merge branch 'topic'
|\
| * a67d4f0 Ask for rye bread
* | c592da0 Ask for white bread
|/
* e2aec91 Add the shopping list
2dc9d68a54fbef84a4e300965086d7bbdb0e1a14
100644 e21221bd88a090240b4b8ecb2300a7c3755f1b6b 1	list.txt
100644 87bc9043c5b51bcc8bc2c48fde311ca1cfb67663 2	list.txt
100644 56a695742d8dfec50231cf27e4d4f50dcc439de2 3	list.txt

Auto-merging list.txt
CONFLICT (content): Merge conflict in list.txt
exit status 1
```

The resolution keeps both kinds of bread. The last command computes the same merge again with `git merge-tree`, which does not touch the index or the working tree. It prints the tree it would write, the conflicted entries and the messages, and it exits with status 1 because of the conflict.[^9]

### criss-cross

Here `topic` merges `main`, and `main` merges an earlier commit of `topic`. After that, the two tips have two common ancestors, `Main work` and `Topic work`, and neither of them descends from the other.

```bash run id=criss-cross
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'one\n' > a.txt
git add a.txt
git commit -q -m "Start"
git branch topic
printf 'main\n' > m.txt
git add m.txt
git commit -q -m "Main work"
git switch -q topic
printf 'topic\n' > t.txt
git add t.txt
git commit -q -m "Topic work"
git merge -q --no-edit main
git switch -q main
git merge -q --no-edit topic~1
git log --graph --oneline --decorate --all
git merge-base --all main topic
```

```output for=criss-cross
*   b6c3b0a (HEAD -> main) Merge branch 'topic' (early part)
|\
| | * d5d300f (topic) Merge branch 'main' into topic
| |/|
| |/
|/|
* | 44d2380 Main work
| * f2b7b9f Topic work
|/
* 18a98fd Start
44d238046786d17c627740860cd295e879df9fe5
f2b7b9f5a63e8e025a07fd9a8b702581073d7775
```

`git merge-base --all` prints both merge bases. A plain `git merge-base` prints only one of them.[^4]

## Versions

Git 2.34 made `ort` the default strategy in place of `recursive`.[^10] The documentation says that `recursive` was the default from Git v0.99.9k to v2.33.0, and that from v2.50.0 the name `recursive` means `ort`.[^8] Git 2.35 added the `zdiff3` conflict style.[^11] Git 2.38 added the mode of `git merge-tree` that takes two commits and writes the merged tree.[^12]

## Pitfalls

### A clean merge can still break the program

One branch renames a function and updates every call. The other branch adds a call with the old name in a different file. No region conflicts, so Git merges cleanly.

```bash wrong id=semantic
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'greet() { echo "Hello, $1"; }\n' > lib.sh
printf '. ./lib.sh\ngreet Ada\n' > app.sh
git add .
git commit -q -m "Add greet"
git switch -q -c rename
sed 's/^greet()/welcome()/' lib.sh > tmp && mv tmp lib.sh
sed 's/^greet /welcome /' app.sh > tmp && mv tmp app.sh
git commit -q -a -m "Rename greet to welcome"
git switch -q main
printf 'greet Grace\n' > extra.sh
git add extra.sh
git commit -q -m "Greet Grace too"
git merge -q --no-edit rename
cat lib.sh extra.sh
bash -c '. ./lib.sh; . ./extra.sh' || echo "exit status $?"
```

```output for=semantic
welcome() { echo "Hello, $1"; }
greet Grace
./extra.sh: line 1: greet: command not found
exit status 127
```

Run the build and the tests on the merge result, not only on each branch.

### A reverted change comes back

Both branches make the same change. Then `main` reverts it. After the merge, the change is back, because `main` as a whole did not change the line (r8).

```bash wrong id=reverted
export GIT_CONFIG_GLOBAL=/dev/null GIT_CONFIG_NOSYSTEM=1
export GIT_AUTHOR_NAME=Ada GIT_AUTHOR_EMAIL=ada@example.com GIT_AUTHOR_DATE=2026-01-05T10:00:00Z
export GIT_COMMITTER_NAME=Ada GIT_COMMITTER_EMAIL=ada@example.com GIT_COMMITTER_DATE=2026-01-05T10:00:00Z
exec 2>&1
git -c init.defaultBranch=main init -q shop && cd shop
printf 'apples\nbread\nmilk\n' > list.txt
git add list.txt
git commit -q -m "Add the shopping list"
git switch -q -c topic
sed 's/^bread$/rye bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Ask for rye bread"
git switch -q main
sed 's/^bread$/rye bread/' list.txt > tmp && mv tmp list.txt
git commit -q -a -m "Try rye bread"
git revert --no-edit HEAD
git merge -q --no-edit topic
git log --oneline --graph
cat list.txt
```

```output for=reverted
[main e010e67] Revert "Try rye bread"
 Date: Mon Jan 5 10:00:00 2026 +0000
 1 file changed, 1 insertion(+), 1 deletion(-)
*   fae47f5 Merge branch 'topic'
|\
| * a67d4f0 Ask for rye bread
* | e010e67 Revert "Try rye bread"
* | d5f88fb Try rye bread
|/
* e2aec91 Add the shopping list
apples
rye bread
milk
```

When you want the revert to win, revert the change on `topic` as well, or fix the merge result before you commit it.

### `-X ours` is not `-s ours`

`git merge -X ours` takes our side only in the regions that conflict, and still takes every other change from the other branch. `git merge -s ours` ignores the other branch completely and keeps our tree.[^8]

### The same conflict again

A long-lived branch that you merge many times can give the same conflict each time. `git rerere` records how you resolved a conflict and applies the same resolution when the same conflict comes back.[^13]

## Comparison

**Mercurial** finds the common ancestor and does a three-way merge of each file that both sides changed. The merge happens in the working directory, and you commit it after you check it. Mercurial does not let you start another merge before you commit.[^14]

**Subversion** merges with `svn merge` into a working copy, and you commit the result. Merge tracking came with Subversion 1.5. It records the merged revisions in the `svn:mergeinfo` property, so a later merge can skip them.[^15] [^16]

**Fossil** merges with `fossil merge`, and with no argument it merges all leaves of the current branch. The result is a check-in that is a direct child of one parent and a merge child of the other.[^17]

**Jujutsu** makes a merge as a commit with more than one parent. It can record a conflict inside a commit, so a merge or a rebase with a conflict succeeds, and you resolve the conflict later.[^18]

**Perforce** (P4) merges with `p4 resolve`, which uses three files: yours, theirs and base. It can accept yours, accept theirs or accept the merge, and it reports a conflict when the two sides change the same region.[^19]

## Implementation notes

Git's merge engine works on trees. It diffs the base with each side to match renames, then runs the textual three-way merge on each file that both sides changed. The `ort` strategy uses the histogram diff algorithm by default.[^8] `git merge-tree --write-tree` exposes the same engine without a working tree, so a server can test whether two branches merge cleanly.[^9] The model of the textual merge that Khanna, Kunal and Pierce describe splits the files into stable chunks, where all three versions agree, and unstable chunks between them. An unstable chunk is changed in one side, changed in the other side, or conflicting.[^5]

## References
