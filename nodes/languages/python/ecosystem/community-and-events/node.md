---
tag: GG4KSJ
kind: feature
owner: python
slug: community-and-events
name: Community and events
aliases: [Python community, discuss.python.org, Discourse, PyCon US, PyLadies, Python Software Foundation, PSF, Code of Conduct, sprints, meetups]
area: languages/python/ecosystem
level: intro
summary: >-
  Python users meet on discuss.python.org, at events such as PyCon US, and in groups such as PyLadies. The Code of Conduct of the PSF covers discuss.python.org and the events of the PSF.
grammar: null
spec: {standard: null, url: "https://policies.python.org/python.org/code-of-conduct/", locator: "Python Software Foundation Code of Conduct"}
see_also: [python/governance-of-python, python/the-pep-process]
sources:
  - url: https://www.python.org/community/
    title: Our Community, Python.org
    locator: introduction
    accessed: 2026-11-16
  - url: https://policies.python.org/python.org/code-of-conduct/
    title: Python Software Foundation Code of Conduct
    locator: Our Community; Scope
    accessed: 2026-11-16
  - url: https://discuss.python.org/
    title: Discussions on Python.org
    locator: home page, categories
    accessed: 2026-11-16
  - url: https://us.pycon.org/2026/
    title: PyCon US 2026
    locator: home page, Conference
    accessed: 2026-11-16
  - url: https://pyladies.com/
    title: PyLadies
    locator: home page
    accessed: 2026-11-16
  - url: https://www.python.org/psf/
    title: Python Software Foundation
    locator: home page
    accessed: 2026-11-16
  - url: https://www.python.org/events/
    title: Our Events, Python.org
    locator: home page
    accessed: 2026-11-16
  - url: https://go.dev/help
    title: Help, The Go Programming Language
    locator: Get help
    accessed: 2026-11-16
---

## Synopsis

```python run id=pycon-2026
import datetime

parts = [
    ('Tutorials', datetime.date(2026, 5, 13), datetime.date(2026, 5, 14)),
    ('Main Conference', datetime.date(2026, 5, 15), datetime.date(2026, 5, 17)),
    ('Sprints', datetime.date(2026, 5, 18), datetime.date(2026, 5, 19)),
]
for name, first, last in parts:
    days = (last - first).days + 1
    print(f'{name:16} {first:%a %d %b} to {last:%a %d %b}, {days} days')
```

```output for=pycon-2026
Tutorials        Wed 13 May to Thu 14 May, 2 days
Main Conference  Fri 15 May to Sun 17 May, 3 days
Sprints          Mon 18 May to Tue 19 May, 2 days
```

The Python community is the people who use and make Python. They meet online on discuss.python.org, at conferences such as PyCon US, and in local groups and meetups. The Python Software Foundation (PSF) supports the language, and its Code of Conduct tells each member how to act in its online spaces and at its events. The example prints the parts of PyCon US 2026 from its schedule.

## Semantics

**r1.** "Great software is supported by great people, and Python is no exception."[^1] "Our user base is enthusiastic and dedicated to spreading use of the language far and wide."[^1]

**r2.** "The Python Software Foundation is the charitable organization behind the Python programming language."[^6]

**r3.** The Code of Conduct says: "Members of the Python community are open, considerate, and respectful."[^2] "The Python community is made up of members from around the globe with a diverse set of skills, personalities, and experiences."[^2]

**r4.** "This Code of Conduct applies to the following online spaces:"[^2] The list includes the mailing lists hosted on python.org, the PSF Discord and Slack servers, the "Python Software Foundation hosted Discourse server discuss.python.org", and the repositories, issue trackers and pull requests of each GitHub organization that the PSF controls.[^2] It also applies to the people at events that the PSF hosts.[^2] Example: [`coc-scope`](#coc-scope).

**r5.** discuss.python.org is for "Discussions related to the Python Programming Language, Python Community, and Python Software Foundation operations."[^3] Its category Python Help is a "General help/discussion forum for the Python programming language. All welcome."[^3] For the category Ideas, it asks: "Would you like to change something in Python? This might be your feedback forum."[^3] The category Events has "Announcements of conferences, workshops, meetups, and other Python-related events."[^3] Example: [`where-to-post`](#where-to-post).

**r6.** PyCon US 2026 was in Long Beach, California, from May 13 to May 19, 2026.[^4] The schedule had tutorials on May 13 and 14, the main conference from May 15 to 17, and sprints on May 18 and 19.[^4] Example: [`pycon-2026`](#pycon-2026).

**r7.** PyLadies says: "We are an international mentorship group with a focus on helping more women become active participants and leaders in the Python open-source community."[^5]

**r8.** python.org has calendars of conferences and of user group events. "The Python events calendars are maintained by the events calendar team."[^7]

## Examples

Find the category of discuss.python.org for a post (rule r5):

```python run id=where-to-post
categories = {
    'Python Help': 'a question about how to use Python',
    'Ideas': 'a proposal to change Python',
    'Events': 'an announcement of a conference or a meetup',
}

def where_to_post(kind):
    for name, use in categories.items():
        if use == kind:
            return name
    return 'read the list of categories first'

print(where_to_post('a question about how to use Python'))
print(where_to_post('a proposal to change Python'))
print(where_to_post('a bug report for a package on PyPI'))
```

```output for=where-to-post
Python Help
Ideas
read the list of categories first
```

Check if the Code of Conduct of the PSF applies to an online space (rule r4):

```python run id=coc-scope
covered = {
    'mailing lists hosted on python.org',
    'PSF Discord and Slack servers',
    'discuss.python.org',
    'GitHub organizations that the PSF controls',
}

for space in ('discuss.python.org', 'a private chat of two friends'):
    print(f'{space}: {space in covered}')
```

```output for=coc-scope
discuss.python.org: True
a private chat of two friends: False
```

The list in the example is not complete. The Code of Conduct also covers "Any other online space administered by the Python Software Foundation".[^2]

## Pitfalls

Read the Code of Conduct before you post on discuss.python.org or open an issue in a repository of the PSF (rule r4). It applies there.

Ask for help in the category Python Help, not in Ideas (rule r5). Ideas is for proposals to change Python.

The dates on this page are for PyCon US 2026 (rule r6). For another year or another event, read the site of the event or the events calendar of python.org (rule r8).

## Comparison

The help page of Go lists the Go Nuts Mailing List: "Get help from Go users, and share your work on the official mailing list."[^8] It also lists a forum: "The Go Forum is a discussion forum for Go programmers."[^8] It lists a Discord server and a Slack channel too.[^8] Python has its main forum, discuss.python.org, on a server that the PSF hosts, and the Code of Conduct of the PSF applies there (rule r4).

[^1]: [Our Community, Python.org](https://www.python.org/community/)
[^2]: [Python Software Foundation Code of Conduct](https://policies.python.org/python.org/code-of-conduct/)
[^3]: [Discussions on Python.org](https://discuss.python.org/)
[^4]: [PyCon US 2026](https://us.pycon.org/2026/)
[^5]: [PyLadies](https://pyladies.com/)
[^6]: [Python Software Foundation](https://www.python.org/psf/)
[^7]: [Our Events, Python.org](https://www.python.org/events/)
[^8]: [Help, The Go Programming Language](https://go.dev/help)
