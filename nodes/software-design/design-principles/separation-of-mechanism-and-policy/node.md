---
tag: XFH5RY
kind: practice
slug: separation-of-mechanism-and-policy
name: Separation of mechanism and policy
aliases: [mechanism, not policy, separate policy from mechanism, rule of separation]
area: software-design/design-principles
level: advanced
summary: >-
  Keep the code that does a job (the mechanism) apart from the code that decides how the job is used (the policy), so that the policy can change without a change to the mechanism.
history:
  year: 1970
  people: [Per Brinch Hansen, R. Levin, W. Wulf]
  note: Per Brinch Hansen introduced the idea in the nucleus of the RC 4000 multiprogramming system, described in 1970; the paper Policy/Mechanism separation in Hydra followed in 1975.
see_also: [separation-of-concerns, orthogonality, inversion-of-control, design-principle]
sources:
  - url: https://en.wikipedia.org/wiki/Separation_of_mechanism_and_policy
    title: Wikipedia, Separation of mechanism and policy
    locator: the lead, the section Rationale and implications, and the references
  - url: http://brinch-hansen.net/papers/1970a.pdf
    title: Per Brinch Hansen, The Nucleus of a Multiprogramming System, Communications of the ACM 13(4), April 1970
    locator: the abstract and section 2, System Nucleus
  - url: http://www.catb.org/esr/writings/taoup/html/ch01s06.html
    title: Eric S. Raymond, The Art of Unix Programming, chapter 1, Basics of the Unix Philosophy
    locator: Rule of Separation
---

## Statement

The principle "states that mechanisms (those parts of a system implementation that control the authorization of operations and the allocation of resources) should not dictate (or overly restrict) the policies according to which decisions are made about which operations to authorize, and which resources to allocate."[^1] Eric Raymond's short form is the Unix Rule of Separation: "Separate policy from mechanism; separate interfaces from engines."[^3]

## Meaning

A mechanism is the code that can do a job: run a process, allocate memory, draw a pixel. A policy decides how the job is used: which process runs next, who gets the memory, how a window looks. When the two are in one module, a change to the policy is a change to the mechanism.

The reason to separate them is that nobody can know all the policies in advance. "It is almost impossible to envision all of the different ways in which a system might be used by different types of users over the life of the product. This means that any hard-coded policies are likely to be inadequate or inappropriate for some (or perhaps even most) potential users."[^1] Decoupling the two "makes it possible for different applications to use the same mechanism implementations with different policies."[^1]

Raymond gives a second reason: "policy and mechanism tend to mutate on different timescales, with policy changing much faster than mechanism."[^3] He gives the X Window System as the example: its designers decided to make X "a generic graphics engine and leave decisions about user-interface style to toolkits and other levels of the system."[^3] According to Raymond, joining policy and mechanism "makes policy rigid and harder to change in response to user requirements, and it means that trying to change policy has a strong tendency to destabilize the mechanisms."[^3]

The principle applies to "a range of resource allocation problems (e.g. CPU scheduling, memory allocation, quality of service) as well as the design of software abstractions."[^1]

## Examples

The example below has one mechanism that runs a list of tasks. The mechanism does not decide the order. A policy function, given as an argument, picks the next task. Two policies give two orders with the same mechanism.

```javascript run id=task-policy
// Mechanism: runs the tasks; it does not decide the order.
function runAll(tasks, policy) {
  const queue = [...tasks];
  const done = [];
  while (queue.length > 0) {
    const next = policy(queue);
    done.push(queue.splice(next, 1)[0].name);
  }
  return done.join(" ");
}

// Policies: each one decides which task runs next.
const firstCome = () => 0;
const shortestFirst = (queue) =>
  queue.reduce((best, t, i) => (t.time < queue[best].time ? i : best), 0);

const tasks = [
  { name: "backup", time: 30 },
  { name: "email", time: 2 },
  { name: "report", time: 10 },
];
console.log(`first come: ${runAll(tasks, firstCome)}`);
console.log(`shortest first: ${runAll(tasks, shortestFirst)}`);
```

```output
first come: backup email report
shortest first: email report backup
```

A new policy, such as one that runs the reports first, is a new function. The mechanism does not change.

| Language | What the example shows |
|---|---|
| JavaScript | the policy as a function argument, as above |
| Python | the same mechanism, with the policy as a function argument |
| Go | the same mechanism, with the policy as a function type |

## Criticism and limits

- **The mechanism must not restrict the policy.** The principle says that mechanisms should not "dictate (or overly restrict)" the policies.[^1] A mechanism that gives the policy too little information, for example only the names of the tasks and not their times, cannot support a shortest-first policy. The designer must still guess which information future policies need.
- **Policies outside the code.** When "a wide or variable range of policies are anticipated depending on applications' needs, it makes sense to create some non-code means for specifying policies", such as the user, group and other permissions of Unix files.[^1] Such a language for policies is a new part of the system that must be designed and maintained.
- **Policy code ages.** Raymond notes that separation makes it "much easier to write good tests for the mechanism (policy, because it ages so quickly, often does not justify the investment)."[^3]

## History

"Per Brinch Hansen introduced the concept of separation of policy and mechanism in operating systems in the RC 4000 multiprogramming system."[^1] His paper of 1970 describes a system nucleus that "can be extended with a hierarchy of operating systems to suit diverse requirements of program scheduling and resource allocation." He wrote: "Our basic attitude during the designing was to make no assumptions about the particular strategy needed to optimize a given type of installation".[^2] In 1975, R. Levin, W. Wulf and others published "Policy/Mechanism separation in Hydra" at the fifth Symposium on Operating Systems Principles.[^1] "The separation of mechanism and policy is the fundamental approach of a microkernel that distinguishes it from a monolithic one."[^1]

## References
