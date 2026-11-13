---
tag: ZRSZQ4
kind: practice
slug: reversibility
name: Reversibility
aliases: [reversible decisions, reduce irreversibility]
area: software-design/design-principles
level: core
summary: >-
  Make design decisions in a way that lets you change them later at low cost, or put them off until you have more information, because irreversible decisions are a main source of complexity.
history:
  year: 2002
  people: [Enrico Zaninotto, Martin Fowler]
  note: Enrico Zaninotto spoke at the XP 2002 conference about irreversibility as a driver of complexity; Martin Fowler applied the idea to software design in IEEE Software in 2003 and in his article Is Design Dead? in 2004.
see_also: [you-arent-gonna-need-it, simple-design, orthogonality, design-principle]
sources:
  - url: https://martinfowler.com/articles/designDead.html
    title: Martin Fowler, Is Design Dead?, section Reversibility added May 2004
    locator: the section Reversibility; Significant Revisions
  - url: https://martinfowler.com/ieeeSoftware/whoNeedsArchitect.pdf
    title: Martin Fowler, Who Needs an Architect?, IEEE Software, July/August 2003
    locator: pages 3 and 4
---

## Statement

"If you can easily change your decisions, this means it's less important to get them right - which makes your life much simpler."[^1] Martin Fowler gives the advice for design: "Rather than trying to get the right decision now, look for a way to either put off the decision until later (when you'll have more information) or make the decision in such a way that you'll be able to reverse it later on without too much difficulty."[^1]

## Meaning

The idea comes from a view of complexity. In a talk at the XP 2002 conference, the economist Enrico Zaninotto said that "irreversibility was one of the prime drivers of complexity." He saw agile methods "as a shift that seeks to contain complexity by reducing irreversibility".[^2] Fowler wrote: "In this view one of the main source of complexity is the irreversibility of decisions."[^1]

Fowler connected this to the meaning of architecture. People want to get some things right early "because they perceive those things as hard to change", so "you might end up defining architecture as 'things that people perceive as hard to change.'"[^2] He then wrote: "I think that one of an architect's most important tasks is to remove architecture by finding ways to eliminate irreversibility in software designs."[^2]

His example is the database schema. It is "commonly believed" that the schema of an enterprise application must be right early, "because it's hard to change the database schema", in particular after the system goes live.[^2] On one project, the database administrator Pramod Sadalage "devised a system that let us change the database schema (and migrate the data) easily". Fowler: "By doing this, he made it so that the database schema was no longer architectural."[^2]

## Examples

The example below makes a change to a schema reversible. The migration has an up step that makes the change and a down step that reverses it.

```javascript run id=reversible-migration
const table = { columns: ["id", "name"] };

const addEmail = {
  up: (t) => t.columns.push("email"),
  down: (t) => t.columns.splice(t.columns.indexOf("email"), 1),
};

console.log(`before: ${table.columns.join(", ")}`);
addEmail.up(table);
console.log(`up:     ${table.columns.join(", ")}`);
addEmail.down(table); // the decision is reversed
console.log(`down:   ${table.columns.join(", ")}`);
```

```output
before: id, name
up:     id, name, email
down:   id, name
```

A team that can run the down step does not need to be sure that the email column is right before it adds the column.

| Language | What the example shows |
|---|---|
| JavaScript | the migration as an object with two functions, as above |
| Python | the same migration as a pair of functions in a dict |
| Go | the same migration as a struct with two function fields |

## Criticism and limits

- **Version control is not enough.** Fowler wrote that the wish to support reversibility is one reason why agile methods put a lot of emphasis on source code control. "While this does not guarantee reversibility, particularly for longed-lived decisions, it does provide a foundation that gives confidence to a team, even if it's rarely used."[^1]
- **Errors must show up quickly.** "Designing for reversibility also implies a process that makes errors show up quickly." A decision is cheap to reverse only while the cost of the fix is low, so "you have to set things up so that potential problem areas are rapidly tested".[^1] See [[fail-fast|Fail fast]].
- **Experiments cost work.** Fowler suggests "doing experiments to see how hard future changes can be, even if you don't actually make the real change now - effectively doing a throw-away prototype on a branch of the system."[^1] The prototype is work that the team throws away.

## History

Enrico Zaninotto gave his talk on agile methods and lean manufacturing at XP 2002.[^1] Martin Fowler wrote about it in his column Who Needs an Architect? in IEEE Software, July/August 2003.[^2] In May 2004, he added a section on reversibility to his article Is Design Dead?.[^1]

## References
