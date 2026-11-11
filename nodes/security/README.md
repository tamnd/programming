This area is about weaknesses in code and how to prevent them. It follows the CWE catalogue for the weaknesses, and it covers the practices that keep code secure: validation, safe use of cryptography, secrets handling and the supply chain.

## What it holds

- The weakness catalogue: the Common Weakness Enumeration, with a card for each entry.
- Injection: SQL injection, command injection, cross-site scripting and their kin.
- Memory-safety weaknesses: buffer overflows, use after free and other memory errors as weaknesses.
- Authentication and sessions: passwords, tokens and sessions in code.
- Cryptography use: how to use cryptographic libraries correctly.
- Secrets handling: keeping keys and passwords out of code and logs.
- Input validation and output encoding: validating input and encoding output.
- Supply-chain security: risks from dependencies and the build, and the defences.
- Secure defaults: the safe and unsafe defaults of each language and library.
- Sandboxing: running code with fewer rights.
- Threat modelling for programmers: finding the threats to a program before they happen.

## What it does not hold

- The mathematics of cryptography. This site links out for it.
- Memory safety as a language property. It is in [Memory and resources](/memory-and-resources/). This area holds the weaknesses.
- Authentication protocols on the web, such as OAuth flows. They are in [Web development](/web-development/).
- Network and system administration security. It is out of scope.
