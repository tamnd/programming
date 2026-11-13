---
tag: GQPMEX
kind: practice
slug: convention-over-configuration
name: Convention over configuration
aliases: [coding by convention, CoC, sensible defaults]
area: software-design/design-principles
level: core
summary: >-
  A framework decides the common cases by convention, so a developer writes configuration only for the parts of the application that differ from the convention.
history:
  year: 2004
  people: [David Heinemeier Hansson]
  note: David Heinemeier Hansson introduced the phrase to describe the philosophy of Ruby on Rails, which he first released as open source in July 2004.
see_also: [dont-repeat-yourself, principle-of-least-astonishment, inversion-of-control, design-principle]
sources:
  - url: https://en.wikipedia.org/wiki/Convention_over_configuration
    title: Wikipedia, Convention over configuration
    locator: the lead and the sections Disadvantages, Motivation and Usage
  - url: https://rubyonrails.org/doctrine
    title: David Heinemeier Hansson, The Rails Doctrine
    locator: section Convention over Configuration
  - url: https://en.wikipedia.org/wiki/Ruby_on_Rails
    title: Wikipedia, Ruby on Rails
    locator: the sections History and Technical overview
---

## Statement

Convention over configuration "is a software design paradigm used by software frameworks that attempts to decrease the number of decisions that a developer using the framework is required to make without necessarily losing flexibility".[^1] "The phrase essentially means a developer only needs to specify unconventional aspects of the application."[^1]

## Meaning

A framework that follows the principle makes a default choice for each common decision. "When the convention implemented by the tool matches the desired behavior, it behaves as expected without having to write configuration files. Only when the desired behavior deviates from the implemented convention is explicit configuration required."[^1]

The standard example is the mapping from classes to database tables. "For example, if there is a class Sales in the model, the corresponding table in the database is called 'sales' by default. It is only if one deviates from this convention, such as the table 'product sales', that one needs to write code regarding these names."[^1]

David Heinemeier Hansson explains the value in the Rails Doctrine. "There are thousands of such decisions that just need to be made once, and if someone else can do it for you, all the better."[^2] He asks of primary keys: "Does it really matter whether it's 'id', 'postId', 'posts_id', or 'pid'? Is this a decision that's worthy of recurrent deliberation? No."[^2] Conventions also build on each other: "If we can depend on a Person class mapping to people table, we can use that same inflection to map an association declared as has_many :people to look for a Person class."[^2] And they help beginners: "There are so many conventions in Rails that a beginner doesn't even need to know about, but can just benefit from in ignorance."[^2]

## Examples

The example below maps model classes to table names. The convention is the class name in lower case with an s at the end. One class differs from the convention, so it is the only class with configuration.

```javascript run id=table-names
// The convention: the table is the class name in lower case, plus "s".
function tableFor(model) {
  return model.table ?? `${model.name.toLowerCase()}s`;
}

class Sale {}
class Order {}
class ProductSale {
  static table = "product_sales"; // the one deviation needs configuration
}

for (const model of [Sale, Order, ProductSale]) {
  console.log(`${model.name} -> ${tableFor(model)}`);
}
```

```output
Sale -> sales
Order -> orders
ProductSale -> product_sales
```

Without the convention, each class would need a line of configuration. With it, only ProductSale has one.

| Language | What the example shows |
|---|---|
| JavaScript | a static field that overrides the convention, as above |
| Python | a class attribute that overrides the convention |
| Go | an optional TableName method, found with a type assertion |

## Criticism and limits

- **Implicit behaviour.** The disadvantages "can occur due to conflicts with other software design principles, like the Zen of Python's 'explicit is better than implicit.'"[^1] A reader who does not know the convention cannot see in the code why a class uses a given table.
- **Hard cases are harder.** A framework of this kind "often involves a domain-specific language with a limited set of constructs or an inversion of control in which the developer can only affect behavior using a limited set of hooks". Both can make behaviours that the conventions do not express "more difficult than when using a software library" that does not try to make these decisions.[^1] See [[inversion-of-control|Inversion of control]].
- **Knowing when to deviate.** Hansson wrote: "As with anything, though, the power of convention isn't without peril." Most applications "have some elements that are unique in some way. It may only be 5% or 1%, but it's there." He calls "knowing when to stray from convention" the hard part.[^2]

## History

"The concept was introduced by David Heinemeier Hansson to describe the philosophy of the Ruby on Rails web framework, but is related to earlier ideas like the concept of 'sensible defaults' and the principle of least astonishment in user interface design."[^1] Hansson "first released Rails as open source in July 2004".[^3] Wikipedia also traces the idea to "the concept of a default" and to the JavaBeans specification. It gives early versions of the Java mapper Hibernate as a case of the opposite: they described the mappings from entities to tables in XML files, and later versions used conventions instead.[^1] See [[principle-of-least-astonishment|Principle of least astonishment]].

## References
