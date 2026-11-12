This area is about weaknesses in code and how to prevent them. It follows the CWE catalogue for the weaknesses, and it covers the practices that keep code secure: validation, safe use of cryptography, secrets handling and the supply chain.

## What it holds

- **Security foundations.** The principles that every secure design and every weakness page uses.
- **Threat modelling.** How to find what can go wrong before writing code, and how to rank it.
- **Weakness catalogue.** The public lists that name weaknesses, attacks and vulnerabilities, and how to use them.
- **OWASP Top 10.** One page per category of the OWASP Top 10 (2025). Each page explains the category, lists its mapped CWEs with links to the weakness pages, and gives the main prevention steps.
- **Injection.** Weaknesses where untrusted data changes the meaning of a command, query, page or template in another interpreter.
- **Memory safety weaknesses.** Weaknesses where code reads or writes memory it does not own, or uses memory in the wrong state. Memory safety as a language property is in memory-and-resources/memory-safety.
- **Exploit mitigations.** The defences in compilers, operating systems and hardware that make memory corruption harder to exploit.
- **Input validation.** Checking untrusted input at the boundary, and encoding output for its context.
- **Web application weaknesses.** Weaknesses specific to browsers and HTTP, and the web platform controls against them.
- **Authentication and sessions.** Weaknesses in checking who a user is, what the user may do, and how sessions keep that state.
- **Cryptography use.** Using cryptography correctly from code: choose vetted libraries, use the right primitive and handle keys well. The mathematics is on math2.
- **Cryptographic weaknesses.** The CWE weaknesses of using cryptography wrongly. Each page has vulnerable and fixed code.
- **Secrets handling.** Keeping passwords, keys and tokens out of code, logs and errors, and managing them through their life.
- **Privacy engineering.** Handling personal data in code so that programs meet privacy goals and laws.
- **Resource exhaustion.** Weaknesses that let an attacker use up CPU, memory, disk or connections. The node uncontrolled-resource-consumption moves here from secure-defaults, because this sub-area holds its family.
- **Race condition weaknesses.** Security weaknesses that come from timing between threads, processes or checks.
- **Error handling and logging weaknesses.** Weaknesses in handling errors and in security logging. They match the OWASP categories A09 and A10 of 2025.
- **Secure defaults.** Configuration and permission weaknesses, and how to ship software that is safe without extra setup.
- **Secure coding by language.** One page per tier 1 language with its main weaknesses, its safe APIs and its secure defaults.
- **Supply chain.** Risks from the code and the build systems that a program depends on, and the controls against them.
- **Sandboxing.** Limiting what code can do when it fails or is hostile.
- **Security testing and review.** Finding security weaknesses in code with tools, tests and people.
- **Security in the development process.** How a team builds security into its process, and how it handles vulnerabilities after release.

## What it does not hold

- The mathematics of cryptography. This site links out for it.
- Memory safety as a language property. It is in [Memory and resources](/memory-and-resources/). This area holds the weaknesses.
- Authentication protocols on the web, such as OAuth flows. They are in [Web development](/web-development/).
- Network and system administration security. It is out of scope.
