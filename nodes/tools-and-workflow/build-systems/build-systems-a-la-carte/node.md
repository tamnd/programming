---
tag: NEW-build-systems-a-la-carte
kind: concept
slug: build-systems-a-la-carte
name: Build system models
aliases: [build systems a la carte, build systems à la carte, scheduler and rebuilder, verifying traces, constructive traces, early cutoff, minimal build system, dynamic dependencies]
area: tools-and-workflow/build-systems
level: research
summary: >-
  A model of build systems by Mokhov, Mitchell and Peyton Jones splits each build system into a scheduler, which orders the tasks, and a rebuilder, which decides if a task must run again.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2018
  people: [Andrey Mokhov, Neil Mitchell, Simon Peyton Jones]
  note: The paper Build Systems à la Carte appeared at ICFP 2018, and an extended version, Build systems à la carte, Theory and practice, appeared in the Journal of Functional Programming in 2020.
see_also: [build-system-concept, build-graph, incremental-builds, remote-caching-and-execution]
sources:
  - paper: doi:10.1145/3236774
    title: Andrey Mokhov, Neil Mitchell and Simon Peyton Jones, Build Systems à la Carte, Proceedings of the ACM on Programming Languages 2 (ICFP), Article 79, 2018
    locator: 1 Introduction; 2 Background (Table 1); 3.1 Common Vocabulary; 3.4 and 3.5 (applicative and monadic tasks); 3.6 Correctness; 4.1 The Scheduler; 4.2 The Rebuilder; 4.4 (Table 2); 6.2 Parallelism; 6.3 Impure Computations; 6.4 Cloud Implementation; 8 Conclusions
  - paper: doi:10.1017/S0956796820000088
    title: Andrey Mokhov, Neil Mitchell and Simon Peyton Jones, Build systems à la carte, Theory and practice, Journal of Functional Programming 30, e11, 2020
    locator: the abstract and the publication record
  - paper: doi:10.1145/2364527.2364538
    title: Neil Mitchell, Shake before building, replacing make with Haskell, Proceedings of the 17th ACM SIGPLAN International Conference on Functional Programming (ICFP 2012), pages 55 to 66
    locator: the publication record
  - url: https://ninja-build.org/manual.html
    title: The Ninja build system manual
    locator: The Ninja log; Rule variables (restat)
  - url: https://bazel.build/remote/caching
    title: Bazel documentation, Remote Caching
    locator: the two types of data in the remote cache (the action cache and the CAS)
---

## Idea

Build tools differ in many ways. Make compares the times of files, Excel recalculates the cells of a sheet, Shake finds dependencies while its rules run, and Bazel shares results between users through a cache. The paper *Build Systems à la Carte*, by Andrey Mokhov, Neil Mitchell and Simon Peyton Jones, describes all of them with one small model.[^1]

In the model, a build system brings a **store** up to date. The store maps **keys** to **values**. In a software build the store is the file system, the keys are file names and the values are the contents of the files. In Excel the keys are cell names such as `A1`.[^1] Some keys are inputs, which the user sets. A **task description** says how to compute the value of each other key from the values of the keys that it needs. The store also holds the **persistent build information**: what the build system keeps from one run to the next, such as file times or hashes.[^1]

The main result of the paper is that two choices are usually built deep into each build system, and that the two are independent:[^1]

- The **scheduler** decides the order in which the tasks run.
- The **rebuilder** decides if a task must run, or if the value that the store holds is still up to date.

A build system is then one scheduler with one rebuilder. The paper places real build systems in a table of these pairs, and finds pairs that no tool used.[^1]

## How it works

**Correctness and minimality.** The model gives exact meanings to two words that build tools use loosely. A build result is correct when the inputs are unchanged, and when each non-input key that the target needs holds the value that its task computes from the final store.[^1] A build system is minimal when it runs each task at most once per build, and only when the task transitively depends on an input that changed since the last build.[^1]

**Tasks and their dependencies.** Each task gets a callback, `fetch`, that returns the value of another key. When a task is an applicative computation, the keys that it fetches are fixed, and a build system can list them without running the task. When a task is a monadic computation, it can choose the next key from the value of a key that it already fetched, so its dependencies are dynamic.[^1] An Excel formula with `INDIRECT` reads a cell whose name it computes. A Shake rule can read a list of files from a file that the build made, and then depend on each file in the list.[^1]

**Three schedulers.**[^1]

- A **topological** scheduler computes one linear order of the tasks before it runs them. It must know all the dependencies first, so it cannot handle dynamic dependencies. Make uses it.
- A **restarting** scheduler runs the tasks in some order and finds the dependencies as it goes. When a task fetches a key that is out of date, the scheduler stops the task, builds the key, and runs the task again later. Excel keeps the order that it found, the calc chain, for the next build, so that it seldom has to stop a task. The build engine of Bazel also restarts tasks.
- A **suspending** scheduler builds a dependency at the moment that a task asks for it, and the task waits. With a record of the keys that it already built, this gives a minimal build system with dynamic dependencies. Shake uses it.

**Four rebuilders.**[^1]

- A **dirty bit** is one bit per key. After a build all bits are clean, and a change marks a key dirty. Excel has a dirty bit for each cell. Make uses file times as a dirty bit: a file is dirty when it is older than one of its dependencies.
- A **verifying trace** records, for each key, the hashes of the dependencies that the task read in the last run, and the hash of its result. The key is up to date when each recorded dependency still has the same hash. Shake and Ninja work this way.
- A **constructive trace** also stores the result value. A build system can keep many traces for each key and share them between users. When a trace matches, the build system copies its value into the store and does not run the task. This is a cloud build.
- A **deep constructive trace** records only the hashes of the terminal inputs, not of the keys between them. The check is faster, but the tasks must be deterministic.

The paper puts the pairs in a table. Eight of the twelve cells hold a real build system:[^1]

| Rebuilder | Topological | Restarting | Suspending |
|---|---|---|---|
| Dirty bit | Make | Excel | none |
| Verifying traces | Ninja | none | Shake |
| Constructive traces | CloudBuild | Bazel | none |
| Deep constructive traces | Buck | none | Nix |

The authors say that each empty cell gives a build system that works. They call the suspending scheduler with constructive traces Cloud Shake, because it would join the dynamic dependencies of Shake with the shared cache of Bazel.[^1]

**Early cutoff.** When a task runs again and gives the same value as before, the tasks that depend on it do not need to run. Shake and Bazel stop the build there, and Make and Excel do not.[^1] With file times, Make cannot mark a file clean without marking the files that depend on it dirty.[^1] Ninja gives a form of early cutoff with the `restat` rule variable: after the command runs, "Each output whose modification time the command did not change will be treated as though it had never needed to be built."[^4]

## In languages

The examples model a small sheet. The inputs are `A1 = 10`, `A2 = 20` and `C1 = 1`. The tasks are `B1 = A1 + A2`, `B2 = B1 * 2`, and `B3 = B2` when `C1` is 1 and `B3 = A2` when it is not. So `B3` has a dynamic dependency. Each example builds the targets `B2` and `B3` with two build systems that share the same tasks:

- **Make-like**: a topological scheduler with a dirty bit. It needs a static list of dependencies, so the list for `B3` holds `C1`, `B2` and `A2`. Writing an input sets its dirty bit, as writing a file sets its time.
- **Shake-like**: a suspending scheduler with verifying traces. A task calls `fetch`, and the build system brings that key up to date before it returns. The hash in a trace is the value as text.

Then the examples change the inputs and print the tasks that each build system runs:

- `A2 = 20 again` writes the same value. The Make-like system runs all three tasks. The Shake-like system compares the hashes and runs nothing.
- `A1 = 11, A2 = 19` keeps `B1` at 30. The Shake-like system runs `B1`, sees the same result, and stops there: this is early cutoff.
- `C1 = 2` makes `B3` read `A2` in place of `B2`. Both systems run only `B3`.
- `A1 = 12` changes `B1` and `B2`. The Make-like system also runs `B3`, because its static list holds `B2`. The trace of the Shake-like system shows that `B3` read only `C1` and `A2` in its last run.

At the end, both systems hold the same values. Both are correct, and only the Shake-like one is minimal.

## Variations and related ideas

- **Parallel builds.** The paper models one thread, and says that all the build systems that it covers can build independent keys in parallel. The extra complexity stays in the scheduler.[^1]
- **Impure tasks.** Real tasks are not always functions. A task can read a dependency that the build system does not track, such as the version of the compiler. It can be non-deterministic, as when a parallel compiler gives different but equal results. It can be volatile, as when it must run in every build. The paper models a volatile task as one that depends on a special key whose value changes in each build.[^1]
- **Cloud builds.** Constructive traces lead to more design questions: how much data to send, workers in the cloud that run the tasks, eviction of old values from the cache, and shallow builds that download only the final products.[^1] Bazel keeps an action cache, "a map of action hashes to action result metadata", and a content-addressable store of output files.[^5] See [[remote-caching-and-execution]].
- **Self-tracking.** Most build systems track changes to the inputs but not to the tasks. Excel and Ninja also notice a change to a task.[^1] Ninja keeps a log of the command that built each file, and builds the file again when the command line changed.[^4]

## Pitfalls

- **The model is not the tool.** No implementation of Make comes with a formal specification, so the paper models an approximation of Make that is close to the usual tools.[^1] Check the documentation of the real tool before you depend on a property from the table.
- **File times are a weak dirty bit.** A command such as `touch` changes the time of a file without a change to its content. So Make is minimal only when nobody does that. Make also needs file times that only go forward, and backup software can break this.[^1]
- **Static lists of dependencies over-approximate.** A topological scheduler must list every key that a task can read. A task with a choice then runs more often than it needs to, as `B3` does in the examples. Excel makes the same over-approximation for `INDIRECT` and for both branches of `IF`.[^1]
- **Restarting wastes work.** A restarting scheduler can start a task, do useful work, and then stop it, so it is not minimal.[^1]
- **Deep traces need deterministic tasks.** When a deep constructive trace skips two or more levels of dependencies, a non-deterministic task can make the build incorrect.[^1]

## Cost

A dirty bit costs one bit for each key and no hashing, but it gives no early cutoff. A verifying trace stores only hashes, so it stays small, and it costs one hash of each file that a task reads. A system with verifying traces gains little from more than one trace for each key.[^1] A constructive trace stores whole values, so the cache grows until the system evicts old items.[^1] To verify a key, a constructive trace looks at the direct dependencies of the key, which must be up to date first, so the time grows with the number of transitive dependencies. A deep constructive trace looks only at the inputs, so it is faster to check, but it cannot stop early except at the levels that it records.[^1]

A suspending scheduler needs tasks that can wait in the middle. Shake first used cheap green threads, and later continuation-passing style.[^1]

## History

Make was the main build tool for many years. The paper says that the challenges of scale then led large firms such as Microsoft, Facebook and Google to write their own build systems.[^1] Neil Mitchell presented Shake, a build system written in Haskell, at ICFP 2012.[^3] The paper says that Shake was made to handle dynamic dependencies without the loss of minimality.[^1] Mokhov, Mitchell and Peyton Jones presented *Build Systems à la Carte* at ICFP 2018. They wrote all the models in Haskell, so each model runs.[^1] The paper ends with a plan to build the monadic suspending cloud build system that the table suggests.[^1] An extended version, *Build systems à la carte: Theory and practice*, appeared in the Journal of Functional Programming in 2020.[^2]

## References
