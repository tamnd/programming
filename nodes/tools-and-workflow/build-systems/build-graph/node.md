---
tag: KR9AVG
kind: concept
slug: build-graph
name: Build graph
aliases: [dependency graph, build dependency graph, action graph, target graph, DAG of build steps, topological order, parallel build]
area: tools-and-workflow/build-systems
level: core
summary: >-
  A build graph links each target of a build to the inputs that it needs. A build tool runs the steps in a topological order of this graph, and runs steps in parallel when no path joins them.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1962
  people: [A. B. Kahn]
  note: Kahn published a general method for the topological sorting of large networks in 1962. Make runs its tasks in a topological order of its dependency graph.
see_also: [build-system-concept, incremental-builds, build-a-project, fork-join, thread-pool]
sources:
  - url: https://www.gnu.org/software/make/manual/html_node/How-Make-Works.html
    title: GNU make manual (version 4.4.1), 2.3 How make Processes a Makefile
    locator: the default goal; rules that the goal does not depend on
  - url: https://bazel.build/basics/artifact-based-builds
    title: Bazel documentation, Artifact-Based Build Systems
    locator: the steps of bazel build; parallelism
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake-buildsystem.7.rst
    title: CMake 3.28.3 documentation source, cmake-buildsystem(7)
    locator: Introduction; Binary Targets
  - paper: doi:10.1145/3236774
    title: Andrey Mokhov, Neil Mitchell and Simon Peyton Jones, Build Systems à la Carte, Proceedings of the ACM on Programming Languages 2 (ICFP), Article 79, 2018
    locator: 1 Introduction; 2.1 Make; 2.2 Excel; 2.3 Shake; 4.1 The Scheduler (Topological, Restarting, Suspending); 6.2 Parallelism
  - paper: doi:10.1145/368996.369025
    title: A. B. Kahn, Topological sorting of large networks, Communications of the ACM 5(11), pages 558 to 562, 1962
    locator: abstract
  - url: https://www.gnu.org/software/make/manual/html_node/Parallel.html
    title: GNU make manual (version 4.4.1), 5.4 Parallel Execution
    locator: the -j option; job slots
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/make.html
    title: The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), make
    locator: SYNOPSIS (-j maxjobs); CHANGE HISTORY, Issue 8
  - url: https://ninja-build.org/manual.html
    title: The Ninja build system manual (version 1.13.1)
    locator: Comparison to Make (builds run in parallel; underspecified dependencies; build edges with many outputs)
  - url: https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html
    title: GNU make manual (version 4.4.1), 4.3 Types of Prerequisites
    locator: normal and order-only prerequisites
  - url: https://bazel.build/basics/task-based-builds
    title: Bazel documentation, Task-Based Build Systems
    locator: difficulty of parallelizing build steps
---

## Idea

Each step of a build needs some inputs and makes some outputs. When the output of one step is an input of another step, the second step depends on the first. These relations form a graph. The nodes are the files or the targets, and an edge goes from a target to each thing that it needs. This is the **build graph**, also called the dependency graph.

The build tool reads the graph from the build file. In GNU Make, the rules give it: before make can process the rule for a target, it must process the rules for the files that the target depends on.[^1] In Bazel, the first step of `bazel build` parses every `BUILD` file in the workspace to create a graph of dependencies among the artifacts.[^2] In CMake, the dependencies between the targets determine the build order and the rules for regeneration after a change.[^3]

Three properties of the graph decide what the tool can do:

- **It has no cycles.** If a target needs itself, through other targets or directly, no order can build it.
- **It gives an order.** A list of the nodes in which each node comes after all the nodes that it needs is a topological order. Make runs its tasks in a topological order of the dependency graph.[^4]
- **It shows the parallel work.** Two steps with no path between them in the graph can run at the same time.

## How it works

**Find the part of the graph that the goal needs.** The tool starts at the goal and follows the edges. Bazel uses the graph to find the transitive dependencies of the target: every target that it depends on, and every target that those targets depend on, recursively.[^2] Make does not process a rule that the goal does not depend on.[^1]

**Put the steps in order.** A topological sort gives an order. In Kahn's method, each node keeps a count of the dependencies that are not done. The nodes with a count of zero are ready. The method takes a ready node, puts it in the order, and lowers the count of each node that needs it. A node whose count falls to zero becomes ready. Kahn published this method for large networks in 1962, for problems such as PERT charts.[^5] The paper calls the topological scheduler of Make one that "pre-computes a linear order of tasks".[^4]

**Run the steps in parallel.** A step can start when all its dependencies are done. Bazel starts by building each target that has no other dependencies, and it starts a target as soon as all its dependencies are built.[^2] The paper describes the parallel form of the topological scheduler in the same way.[^4] If you group the steps by their depth in the graph, each group, or wave, can run at the same time when there are enough workers. The longest path in the graph is the lower limit of the time of the build, however many workers there are.

GNU Make runs one recipe at a time by default. The `-j` option tells it to run many recipes at the same time, and an integer after `-j` sets the number of job slots.[^6] POSIX make has the option `-j maxjobs` too.[^7] Ninja runs builds in parallel by default, with a number of jobs from the number of CPUs.[^8]

**Find a cycle.** A depth-first search finds a cycle: if it reaches a node that is still on its current path, the edges from that node back to itself form a cycle. Kahn's method finds a cycle in a different way: when no node is ready, but some nodes are not in the order, those nodes are on a cycle or need a node on a cycle. The run below gives GNU Make a makefile with the cycle `a.o` needs `gen.h`, and `gen.h` needs `a.o`. GNU Make prints a message, drops one edge of the cycle, and builds the rest.

```bash run id=make-cycle
d=$(mktemp -d)
cd "$d"
cat >Makefile <<'END'
app: a.o
	@echo link app
a.o: gen.h
	@echo compile a.o
gen.h: a.o
	@echo generate gen.h
END
make 2>&1
cd /
rm -rf "${d:?}"
```

```output for=make-cycle
make: Circular gen.h <- a.o dependency dropped.
generate gen.h
compile a.o
link app
```

A build that continues after a dropped edge can run the steps in a wrong order. Treat the message as an error in the build file.

## In languages

The examples use the graph of a small C program with a parser. A parser generator makes `parse.c` and `parse.h` from `grammar.y`, and `app` links three object files. Each example finds a topological order with Kahn's method, groups the targets into waves for a parallel build, and searches for a cycle. Then it adds the edge "`lex.h` needs `lex.o`", which makes a cycle. Kahn's method then orders only 6 of the 10 nodes, and the search prints the cycle. To give the same order in each language, the examples take the ready node with the smallest name first.

## Variations and related ideas

- **Static and dynamic dependencies.** Make needs all the dependencies before the build starts, so its graph is static.[^4] Some build systems find dependencies while the tasks run. In Excel, the formula `INDIRECT("A" & C1)` depends on a cell whose name is known only after `C1` is computed.[^4] In Shake, a rule can read a file that lists other files, and then depend on each of them.[^4] A static topological order cannot handle dynamic dependencies. The paper describes two other schedulers for them: a restarting scheduler stops a task that needs a key that is out of date and tries it again later, and a suspending scheduler pauses the task while it builds the key.[^4]
- **Order-only prerequisites.** In GNU Make, a normal prerequisite gives an order and a dependency: the prerequisite is built first, and if it is newer, the target is out of date.[^9] An order-only prerequisite gives only the order. It is built first, but a change to it does not make the target out of date.[^9] A directory that must exist before files go into it is the usual example.
- **Graphs of targets and graphs of actions.** CMake works with high-level targets, such as one library or one executable.[^3] The build tool that CMake generates works with the individual commands. The two graphs have different sizes, but the same rules apply.
- **Task graphs.** In a task-based tool, the graph links tasks, not files. The tool cannot tell if two tasks touch the same file, so it cannot know in general that running them at the same time is safe.[^10]

## Pitfalls

- **A missing edge.** If a step reads a file that another step writes, but the build file does not have the edge, a serial build can still work by luck, because the steps run in the order of the file. A parallel build then fails at random, or reads a half-written file. The Ninja manual says that underspecified build dependencies result in incorrect builds.[^8]
- **An extra edge.** An edge that is not needed makes the graph deeper. The build is still correct, but fewer steps can run in parallel, and a change to the extra dependency rebuilds the target for no reason.
- **One step that writes many files.** A parser generator writes `parse.c` and `parse.h` in one command. If the build file has two separate rules, a parallel build can run the command two times at the same time. Use the syntax of the tool for a rule with many outputs. In Ninja, a build edge may have many outputs.[^8]
- **A cycle in generated code.** A generated header that needs a program that includes the header makes a cycle. Break it with a separate step that generates the header first.
- **Waves are not a schedule.** Steps have different durations, so a tool that waits for each wave to finish loses time. A tool starts each step as soon as its own dependencies are done.[^2]

## Cost

Kahn's method looks at each node and each edge one time. The paper of 1962 says that a PERT network of 30,000 activities can be ordered in less than one hour of machine time, on the IBM 7090 of that time.[^5] For a build tool, the cost of the graph is usually small next to the cost of the steps. The cost is in the reading: Bazel parses every `BUILD` file in the workspace before it builds,[^2] and each tool must check the inputs before it can start.

Parallel builds trade memory and CPU for time. With `n` workers, the time of a build is at least the total time of the steps divided by `n`, and at least the time of the longest path. The Bazel documentation says that parallel steps can give an order of magnitude improvement over building targets one at a time on a multicore machine.[^2]

## History

Make uses a static graph and a topological order.[^4] GNU Make has the `-j` option for parallel execution,[^6] and POSIX added `-j maxjobs` to make in Issue 8, the edition of 2024.[^7] The paper says that for years Make dominated, but the challenges of scale then drove large firms such as Microsoft, Facebook and Google to develop their own build systems.[^4] Bazel builds the full graph from the `BUILD` files of the workspace.[^2] Ninja leaves the graph to a generator program, and runs it as fast as it can.[^8] In 2018, Mokhov, Mitchell and Peyton Jones described the order of a build as one choice in the design of a build system, the scheduler, and compared the topological, restarting and suspending schedulers.[^4]

## References
