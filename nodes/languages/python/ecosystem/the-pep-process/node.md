---
tag: 016HMH
kind: feature
owner: python
slug: the-pep-process
name: The PEP process
aliases: [PEP, Python Enhancement Proposal, PEP 1, PEP-Delegate, Typing Council, C API Working Group, Standards Track PEP]
area: languages/python/ecosystem
level: core
summary: >-
  A PEP is a design document that proposes a change to Python. PEP 1 sets the types, the statuses and the steps. The Steering Council or a PEP-Delegate accepts or rejects each PEP.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0001/", locator: "PEP 1, PEP Purpose and Guidelines"}
see_also: [python/governance-of-python, python/the-python-package-index]
sources:
  - url: https://peps.python.org/pep-0001/
    title: "PEP 1: PEP Purpose and Guidelines"
    locator: What is a PEP?; PEP Types; PEP Workflow; PEP Review & Resolution; PEP Header Preamble
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0729/
    title: "PEP 729: Typing governance process"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0731/
    title: "PEP 731: C API Working Group Charter"
    locator: Abstract; Specification
    accessed: 2026-11-16
  - url: https://github.com/golang/proposal/blob/master/README.md
    title: Go Proposal Process
    locator: The Proposal Process; Proposal Review
    accessed: 2026-11-16
---

## Synopsis

```python run id=header
from email.parser import HeaderParser

text = """PEP: 9999
Title: A sample proposal
Author: A. Author <author@example.org>
Sponsor: B. Sponsor <sponsor@example.org>
Status: Draft
Type: Standards Track
Created: 16-Nov-2026
"""

header = HeaderParser().parsestr(text)
print(header['PEP'], header['Status'], header['Type'])
```

```output for=header
9999 Draft Standards Track
```

A PEP is a design document in the repository of PEPs. It starts with a header of fields in the RFC 2822 style, and the standard library can read that header. A new PEP has the status Draft.

## Semantics

**r1.** "PEP stands for Python Enhancement Proposal. A PEP is a design document providing information to the Python community, or describing a new feature for Python or its processes or environment."[^1] PEPs are "the primary mechanisms for proposing major new features, for collecting community input on an issue, and for documenting the design decisions that have gone into Python."[^1]

**r2.** "There are three kinds of PEP".[^1] "A Standards Track PEP describes a new feature or implementation for Python."[^1] "An Informational PEP describes a Python design issue, or provides general guidelines or information to the Python community, but does not propose a new feature."[^1] "A Process PEP describes a process surrounding Python, or proposes a change to (or an event in) a process."[^1]

**r3.** "Most enhancements and bug fixes don’t need a PEP and can be submitted directly to the Python issue tracker."[^1] The author first tests the idea in public. "Posting to the Ideas category of the Python Discourse is usually the best way to go about this".[^1]

**r4.** "Each PEP must have a champion".[^1] The PEP also needs a sponsor, for example a member of the Python core team, of the PEP editors team, of the Typing Council or of the Packaging Council.[^1]

**r5.** The draft is "submitted as a draft PEP via a GitHub pull request."[^1] The PEP editors check the structure and the format, and "Once approved, they will assign your PEP a number."[^1]

**r6.** After the PEP has a number, "a discussion thread for the PEP should be created to provide a central place to discuss and review its contents".[^1] "The PEPs category of the Python Discourse is the preferred choice for most new PEPs".[^1]

**r7.** "The final authority for PEP approval is the Steering Council."[^1] A core developer can offer to be the PEP-Delegate. "If the Steering Council approves their offer, the PEP-Delegate will then have the authority to approve or reject that PEP."[^1]

**r8.** "For PEPs related to the Python type system, the Typing Council (PEP 729) provides a recommendation to the Steering Council."[^1] PEP 729 created "a council that is responsible for maintaining and developing the Python type system."[^2]

**r9.** PEP 731 created "the C API Working Group: a small committee of Python core developers responsible for overseeing and coordinating the development and maintenance of the Python C API."[^3]

**r10.** The Status header takes one of the values Draft, Active, Accepted, Provisional, Deferred, Rejected, Withdrawn, Final and Superseded.[^1] "When the reference implementation is complete and incorporated into the main source code repository, the status will be changed to “Final”."[^1] "Some Informational and Process PEPs may also have a status of “Active” if they are never meant to be completed."[^1] Example: [`statuses`](#statuses).

**r11.** "Each PEP must begin with an RFC 2822 style header preamble."[^1] The Type header is one of Standards Track, Informational and Process.[^1] Example: [`header`](#header).

## Examples

Check the Status and Type fields of a header against the values of PEP 1 (rules r10 and r11):

```python run id=statuses
from email.parser import HeaderParser

STATUSES = {'Draft', 'Active', 'Accepted', 'Provisional', 'Deferred',
            'Rejected', 'Withdrawn', 'Final', 'Superseded'}
TYPES = {'Standards Track', 'Informational', 'Process'}

def check(text):
    header = HeaderParser().parsestr(text)
    problems = []
    if header['Status'] not in STATUSES:
        problems.append('unknown status ' + repr(header['Status']))
    if header['Type'] not in TYPES:
        problems.append('unknown type ' + repr(header['Type']))
    return problems or ['ok']

print(check('PEP: 9999\nStatus: Draft\nType: Process\n'))
print(check('PEP: 9998\nStatus: Approved\nType: Feature\n'))
```

```output for=statuses
['ok']
["unknown status 'Approved'", "unknown type 'Feature'"]
```

A header field can continue on the next line. The next line starts with a space, as in RFC 2822 (rule r11):

```python run id=continuation
from email.parser import HeaderParser

text = 'PEP: 9999\nAuthor: A. Author <a@example.org>,\n        B. Author <b@example.org>\nStatus: Draft\n'
header = HeaderParser().parsestr(text)
authors = ' '.join(header['Author'].split())
print(authors)
```

```output for=continuation
A. Author <a@example.org>, B. Author <b@example.org>
```

## Pitfalls

Accepted is not the same as Final (rule r10). "Once a PEP has been accepted, the reference implementation must be completed."[^1] Until then, the feature is not in a release. Check the Status header and the Python version of the change before you use a feature from a PEP.

A resolved PEP is not the documentation of a feature. "Once resolution is reached, a PEP is considered a historical document rather than a living specification."[^1] "Formal documentation of the expected behavior should be maintained elsewhere, such as the Language Reference for core features, the Library Reference for standard library modules or the PyPA Specifications for packaging."[^1]

An Informational PEP is not a rule. "Informational PEPs do not necessarily represent a Python community consensus or recommendation, so users and implementers are free to ignore Informational PEPs or follow their advice."[^1]

Most changes do not need a PEP (rule r3). A bug fix or a small improvement goes to the issue tracker.

## Comparison

Go has a proposal process too. "The proposal process is the process for reviewing a proposal and reaching a decision about whether to accept or decline the proposal."[^4] "The proposal author creates a brief issue describing the proposal."[^4] If needed, "The proposal author writes a design doc to work out details of the proposed design".[^4] "A group of Go team members holds “proposal review meetings” approximately weekly to review pending proposals."[^4] A Python proposal starts as a discussion on the Python Discourse and becomes a PEP file with a number. A Go proposal starts as an issue on the issue tracker.

[^1]: [PEP 1: PEP Purpose and Guidelines](https://peps.python.org/pep-0001/)
[^2]: [PEP 729: Typing governance process](https://peps.python.org/pep-0729/)
[^3]: [PEP 731: C API Working Group Charter](https://peps.python.org/pep-0731/)
[^4]: [Go Proposal Process](https://github.com/golang/proposal/blob/master/README.md)
