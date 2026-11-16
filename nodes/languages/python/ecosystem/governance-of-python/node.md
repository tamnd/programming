---
tag: CWGJJN
kind: feature
owner: python
slug: governance-of-python
name: Governance of Python
aliases: [PEP 13, Python Steering Council, Python core team, Python Software Foundation, PSF, BDFL, PEP 8016]
area: languages/python/ecosystem
level: core
summary: >-
  PEP 13 governs Python. The core team elects a Steering Council of five people after each feature release. The PSF is the charity that supports Python. Guido van Rossum was BDFL until July 2018.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0013/", locator: "PEP 13, Python Language Governance"}
see_also: [python/the-pep-process, python/the-python-package-index]
sources:
  - url: https://peps.python.org/pep-0013/
    title: "PEP 13: Python Language Governance"
    locator: Current steering council; The steering council; The core team; History
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-8107/
    title: "PEP 8107: 2026 Term Steering Council election"
    locator: Abstract; Results
    accessed: 2026-11-16
  - url: https://www.python.org/psf/mission/
    title: Mission, Python Software Foundation
    locator: Mission
    accessed: 2026-11-16
  - url: https://www.python.org/psf/
    title: Python Software Foundation
    locator: home page
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#history
    title: Go FAQ
    locator: What is the history of the project?
    accessed: 2026-11-16
  - url: https://github.com/golang/proposal/blob/master/README.md
    title: Go Proposal Process
    locator: Proposal Review
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0001/
    title: "PEP 1: PEP Purpose and Guidelines"
    locator: PEP Review & Resolution
    accessed: 2026-11-16
---

## Synopsis

```python run id=council
COUNCIL_2026 = ['Barry Warsaw', 'Donghee Na', 'Pablo Galindo Salgado',
                'Savannah Ostrowski', 'Thomas Wouters']
print(len(COUNCIL_2026), 'members')
for name in COUNCIL_2026:
    print(' ', name)
```

```output for=council
5 members
  Barry Warsaw
  Donghee Na
  Pablo Galindo Salgado
  Savannah Ostrowski
  Thomas Wouters
```

PEP 13 is the governance document of Python. The core team elects a Steering Council of five people, and the council makes the final decisions. The list above is the council for the 2026 term, from PEP 13. The Python Software Foundation (PSF) is a separate organization. It is the charity behind Python.

## Semantics

**r1.** "The Python project was started by Guido van Rossum, who served as its Benevolent Dictator for Life (BDFL) from inception until July 2018, when he stepped down."[^1] The core developers then voted on new models of governance. "Eventually the proposal in PEP 8016 was selected as the new governance model, and was used to create the initial version of this PEP."[^1]

**r2.** "The steering council is a 5-person committee."[^1] "The 2026 term steering council consists of: Barry Warsaw, Donghee Na, Pablo Galindo Salgado, Savannah Ostrowski, Thomas Wouters".[^1] Example: [`council`](#council).

**r3.** The council works, among other things, to "Maintain the quality and stability of the Python language and CPython interpreter" and to "Act as a “court of final appeal” for decisions where all other methods have failed."[^1]

**r4.** "The council has broad authority to make decisions about the project."[^1] For example, it can "Accept or reject PEPs" and "Enforce or update the project’s code of conduct".[^1] "However, they cannot modify this PEP, or affect the membership of the core team, except via the mechanisms specified in this PEP."[^1]

**r5.** "To use its powers, the council votes."[^1] "Passing requires a strict majority of non-abstaining council members."[^1] Example: [`council-vote`](#council-vote).

**r6.** "A new council is elected after each feature release."[^1] "There are no term limits."[^1] In the vote, "Each core team member can assign zero to five stars to each candidate."[^1] The outcome "is determined using the STAR voting system, modified to use the Multi-winner Bloc STAR approach."[^1]

**r7.** "at most 2 members of the council can work for any single employer."[^1] "In a council election, if 3 of the top 5 vote-getters work for the same employer, then whichever of them ranked lowest is disqualified and the 6th-ranking candidate moves up into 5th place; this is repeated until a valid council is formed."[^1] Example: [`employer-rule`](#employer-rule).

**r8.** "The core team is the group of trusted volunteers who manage Python."[^1] "Core team members may participate in formal votes, typically to nominate new team members and to elect the steering council."[^1] Membership "is granted by receiving at least two-thirds positive votes in a core team vote that is open for one week".[^1]

**r9.** "The mission of the Python Software Foundation is to promote, protect, and advance the Python programming language, and to support and facilitate the growth of a diverse and international community of Python programmers."[^3] "The Python Software Foundation is the charitable organization behind the Python programming language."[^4]

## Examples

The election for the 2026 term ran in 2025. "Of 106 eligible voters, 74 cast ballots."[^2] PEP 8107 gives the star total of each candidate. The tabulation has a scoring round and an automatic runoff for each seat.[^2] In this election, the five highest totals are also the five winners:

```python run id=results-2026
stars = {
    'Pablo Galindo Salgado': 313,
    'Savannah Ostrowski': 249,
    'Barry Warsaw': 239,
    'Donghee Na': 191,
    'Thomas Wouters': 187,
    'Gregory P. Smith': 173,
}
ranked = sorted(stars, key=stars.get, reverse=True)
print(ranked[:5])
print(f'turnout {74 / 106:.0%}')
```

```output for=results-2026
['Pablo Galindo Salgado', 'Savannah Ostrowski', 'Barry Warsaw', 'Donghee Na', 'Thomas Wouters']
turnout 70%
```

A council vote passes with a strict majority of the members who do not abstain (rule r5):

```python run id=council-vote
def passes(yes, no, abstain):
    assert yes + no + abstain == 5
    return yes > (yes + no) / 2

print(passes(3, 2, 0))
print(passes(2, 1, 2))
print(passes(2, 2, 1))
```

```output for=council-vote
True
True
False
```

The employer rule of rule r7, with invented candidates in rank order:

```python run id=employer-rule
def seat(ranked, employer, seats=5, limit=2):
    pool = list(ranked)
    while True:
        top = pool[:seats]
        counts = {}
        for name in top:
            counts[employer[name]] = counts.get(employer[name], 0) + 1
        over = [e for e, n in counts.items() if n > limit]
        if not over:
            return top
        lowest = [name for name in top if employer[name] == over[0]][-1]
        pool.remove(lowest)

ranked = ['A', 'B', 'C', 'D', 'E', 'F', 'G']
employer = {'A': 'X', 'B': 'Y', 'C': 'X', 'D': 'X', 'E': 'Z', 'F': 'X', 'G': 'W'}
print(seat(ranked, employer))
```

```output for=employer-rule
['A', 'B', 'C', 'E', 'G']
```

D is the third candidate from employer X in the top five, so D drops out and F moves up. F also works for X, so F drops out too, and G gets the fifth seat.

## Pitfalls

The PSF and the Steering Council are different bodies. The council makes decisions about the language and CPython (rule r3). The PSF is the charity that promotes and protects Python (rule r9).

"BDFL-Delegate" in an old PEP means the person who decided that PEP. The title is from the time before 2018 (rule r1). PEP 1 says: "The term “BDFL-Delegate” is a deprecated alias for PEP-Delegate, a legacy of the time when Python was led by a BDFL."[^7]

The council changes after each feature release (rule r6). A list of members is true for one term only. Read the "Current steering council" section of PEP 13 for the current list.

## Comparison

"Go became a public open source project on November 10, 2009."[^5] In Go, "A group of Go team members holds “proposal review meetings” approximately weekly to review pending proposals."[^6] In Python, the core team elects the council that makes the final decisions (rules r2 and r6).

[^1]: [PEP 13: Python Language Governance](https://peps.python.org/pep-0013/)
[^2]: [PEP 8107: 2026 Term Steering Council election](https://peps.python.org/pep-8107/)
[^3]: [Mission, Python Software Foundation](https://www.python.org/psf/mission/)
[^4]: [Python Software Foundation](https://www.python.org/psf/)
[^5]: [Go FAQ, What is the history of the project?](https://go.dev/doc/faq#history)
[^6]: [Go Proposal Process](https://github.com/golang/proposal/blob/master/README.md)
[^7]: [PEP 1: PEP Purpose and Guidelines](https://peps.python.org/pep-0001/)
