# Contributing

Thank you for your help. This file tells you how to change the content. To change the toolchain, go to [tamnd/programming-agent](https://github.com/tamnd/programming-agent).

## Rules for all content

- Write everything yourself. Do not copy text, figures, exercises or code from a book, a manual, a paper or a web site, even with a free licence. Read your sources, close them, and then write. Put each source that you used in `sources`.
- Write in plain English. Use short sentences, the active voice and simple words. We follow ASD-STE100 where we can.
- Each claim about a language must have a source. For a rule of the language, cite the reference or the standard at the version that the claim is about. For a version claim, such as "since Python 3.10", the build runs a probe on each version.
- Each code example must run. Its output on the page is the output that the build records. Do not type the output by hand.
- Each code file starts with the SPDX header. Copy it from a node that exists.

The content standard (spec 06) has the full rules.

## Make a change

1. Find an issue, or open one. Say which node you want to write or change.
2. Fork the repository and make a branch from `main`.
3. Make a node with `prog new`, or change a node.
4. Run `prog check -changed origin/main` and, for a task, `prog test -unsafe-local <slug>`.
5. Run `prog site serve` and read your page.
6. Open a pull request and fill in the template.

A new node gets a provisional tag that starts with `NEW-`. A maintainer gives it a permanent tag before the merge.

## Review

A maintainer reads each pull request. For a rule of a language, the reviewer reads the cited part of the reference. For code, the reviewer runs it. The review record goes into `reviews/`.

## Sign-off

Each commit must have a `Signed-off-by` line (the [Developer Certificate of Origin](https://developercertificate.org)). Use `git commit -s`. With this line, you state that you wrote the change and that you have the right to give it under the licence of its part.

## Licence

Your change is under the licence of the part that it goes into. [LICENSES/README.md](LICENSES/README.md) lists the parts.
