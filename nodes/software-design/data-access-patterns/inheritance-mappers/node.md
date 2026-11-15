---
tag: HV8WRK
kind: pattern
slug: inheritance-mappers
name: Inheritance Mappers
aliases: [inheritance mappers pattern, mapper hierarchy]
area: software-design/data-access-patterns
level: advanced
summary: >-
  Inheritance Mappers organizes the mappers of a class hierarchy: one mapper for each class maps the fields that the class declares, and one mapper for the hierarchy chooses the concrete mapper.
family: architectural
polarity: positive
intent: "A structure to organize database mappers that handle inheritance hierarchies."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), as the common structure behind Single Table Inheritance, Class Table Inheritance and Concrete Table Inheritance.
see_also: [single-table-inheritance, class-table-inheritance, concrete-table-inheritance, data-mapper, template-method-pattern, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/inheritanceMappers.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Inheritance Mappers
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Inheritance Mappers
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1128-L1140
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/mapper.py
    locator: Mapper.polymorphic_map, lines 1128 to 1140
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1335-L1358
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/mapper.py
    locator: Mapper._configure_inheritance, lines 1335 to 1358
  - url: https://docs.sqlalchemy.org/en/20/orm/inheritance.html
    title: SQLAlchemy 2.0 documentation, Mapping Class Inheritance Hierarchies
    locator: Joined Table Inheritance; Single Table Inheritance; Concrete Table Inheritance
  - url: https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/SingleTableEntityPersister.java#L56-L68
    title: Hibernate ORM 6.6.9 source, SingleTableEntityPersister.java
    locator: class comment, lines 56 to 68
  - url: https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/JoinedSubclassEntityPersister.java#L83-L96
    title: Hibernate ORM 6.6.9 source, JoinedSubclassEntityPersister.java
    locator: class comment, lines 83 to 96
  - url: https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/UnionSubclassEntityPersister.java#L69-L80
    title: Hibernate ORM 6.6.9 source, UnionSubclassEntityPersister.java
    locator: class comment, lines 69 to 80
---

## Intent

Inheritance Mappers is "A structure to organize database mappers that handle inheritance hierarchies."[^1] Each mapper maps the fields that its class declares and uses the mapper of the superclass for the rest, and one mapper for the whole hierarchy chooses the right concrete mapper for each object and each row.

## Problem

Fowler states the two needs: "When you map from an object-oriented inheritance hierarchy in memory to a relational database you have to minimize the amount of code needed to save and load the data to the database. You also want to provide both abstract and concrete mapping behavior that allows you to save or load a superclass or a subclass."[^1] With `Footballer`, `Cricketer` and `Bowler` under `Player`, the code for the fields of `Player` must not be copied into three mappers. And a program that asks for "player 3" does not know which class player 3 has, so some mapper must find out and load the right class.

## Solution

Make a hierarchy of mapper classes that follows the hierarchy of domain classes, and add one mapper for the hierarchy as a whole.[^2]

- An **abstract mapper** for the root class, such as `AbstractPlayerMapper`, saves and loads the fields of `Player`, and holds the steps of insert and find that do not depend on the class.
- A **concrete mapper** for each concrete class, such as `FootballerMapper`, saves and loads the fields that its class declares, and calls the mapper of the superclass first. `BowlerMapper` is a subclass of `CricketerMapper`, as `Bowler` is a subclass of `Cricketer`. Each concrete mapper knows how to create an empty object of its class.
- A **hierarchy mapper**, such as `PlayerMapper`, is what the rest of the program uses. On insert, it chooses the concrete mapper from the class of the object. On find, it reads the type of the row and delegates to the concrete mapper for that type.

The mapper classes call each other like the template method pattern: the abstract mapper has the steps, and the subclasses fill in the fields. The same structure works for each of the three table designs; Fowler says that "the general structure works the same for all of them."[^1] Only the code that reads and writes the rows changes.

```text
  domain classes            mapper classes
  Player                    AbstractPlayerMapper    name; the steps of insert and find
   |- Footballer             |- FootballerMapper    club, then calls AbstractPlayerMapper
   |- Cricketer              |- CricketerMapper     runs, then calls AbstractPlayerMapper
       |- Bowler                 |- BowlerMapper    wickets, then calls CricketerMapper
                            PlayerMapper            chooses a concrete mapper by class or by type
```

The following program saves a footballer through the hierarchy mapper. The mapper for the fields of `Player` writes the name, and the concrete mapper adds the type and the club.

```python run id=delegate-to-concrete-mapper
class Player:
    def __init__(self, name):
        self.name = name


class Footballer(Player):
    def __init__(self, name, club):
        super().__init__(name)
        self.club = club


class PlayerFieldsMapper:
    def save(self, p, row):
        row["name"] = p.name  # The fields of Player are mapped in one place.


class FootballerMapper(PlayerFieldsMapper):
    def save(self, p, row):
        super().save(p, row)
        row["type"], row["club"] = "footballer", p.club


class PlayerMapper:
    mappers = {Footballer: FootballerMapper()}

    def save(self, p):
        row = {}
        self.mappers[type(p)].save(p, row)  # Choose the concrete mapper and delegate.
        return row


print(PlayerMapper().save(Footballer("Ann", "Lions")))
```

```output
{'name': 'Ann', 'type': 'footballer', 'club': 'Lions'}
```

## Example

Each example file stores the players in one table with a type column, as in [[single-table-inheritance]], and has the five mapper classes. It first prints the chain of mapper classes for each type: `BowlerMapper`, `CricketerMapper` and then `AbstractPlayerMapper`. It inserts a footballer, a cricketer and a bowler through `PlayerMapper`, and each insert prints which mapper wrote which field. For the bowler, three mapper classes write one row. Then it finds player 3 without knowing its class: `PlayerMapper` reads the type `bowler` from the row and uses `BowlerMapper`. The last line loads all the players, each with its own mapper. Python, JavaScript and Java use class inheritance for the mappers. Go and Rust have no class inheritance, so each mapper holds the mapper of its superclass and calls it.

## Consequences

**Benefits.**

- **Each field is mapped in one place.** The code for the fields of `Player` is in `AbstractPlayerMapper` only, so a new field in `Player` changes one mapper.
- **The program can save and load through the superclass.** `PlayerMapper` gives one find and one insert for all players, and the caller does not need to know the class.
- **The structure does not depend on the table design.** A change from one table to a table for each class changes the row code in the mappers, not the structure.

**Liabilities.**

**More classes.** A hierarchy of four domain classes gets five mapper classes. For a small hierarchy with one table design, a single mapper with a branch on the type can be simpler.

**The two hierarchies must change together.** A new subclass of `Player` needs a new concrete mapper, and `PlayerMapper` must learn its class and its type code.

**Without the structure, the code for the shared fields is copied.** In the following program, each mapper has its own copy of the code for the fields of `Player`. A later field `country` was added to one copy only, so the cricketer loses it.

```python wrong id=copied-superclass-fields
class Player:
    def __init__(self, name, country):
        self.name, self.country = name, country


class Footballer(Player):
    def __init__(self, name, country, club):
        super().__init__(name, country)
        self.club = club


class Cricketer(Player):
    def __init__(self, name, country, runs):
        super().__init__(name, country)
        self.runs = runs


# Wrong: each mapper copies the code for the fields of Player. The field country
# was added later, and only one of the copies was changed.
def save_footballer(p):
    return {"name": p.name, "country": p.country, "club": p.club}


def save_cricketer(p):
    return {"name": p.name, "runs": p.runs}


rows = [save_footballer(Footballer("Ann", "NZ", "Lions")), save_cricketer(Cricketer("Bo", "NZ", 412))]
print([(r["name"], r.get("country")) for r in rows])
```

```output
[('Ann', 'NZ'), ('Bo', None)]
```

## When not to use it

**When there is no inheritance to map.** For one class and one table, use a plain [[data-mapper]].

**When an ORM does the mapping.** SQLAlchemy, Hibernate and Entity Framework Core have this structure inside them. The program declares the hierarchy and the table design, and does not write the mappers.

## Variants and related patterns

**The table designs.** The mappers are the same for [[single-table-inheritance]], [[class-table-inheritance]] and [[concrete-table-inheritance]]. With one table, the hierarchy mapper reads the type column. With a table for each class, the concrete mapper of each class writes its own table. With a table for each concrete class, the hierarchy mapper must try each table or use a union.

**Registry of mappers.** Instead of a fixed list in the hierarchy mapper, each concrete mapper can add itself to a map from type code to mapper. SQLAlchemy does this with its polymorphic map.[^3]

**Related patterns.** Each mapper is a [[data-mapper]], and the calls from a mapper to the mapper of its superclass follow the [[template-method-pattern]].

## Known uses

- SQLAlchemy: a mapper for a subclass inherits from the mapper of its superclass. In [Mapper._configure_inheritance in mapper.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1335-L1358), each mapper takes the `polymorphic_map` of the mapper that it inherits from and adds its own `polymorphic_identity` to it.[^4] The docstring in [mapper.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1128-L1140) says: "An inheritance chain of mappers will all reference the same polymorphic map object. The object is used to correlate incoming result rows to target mappers."[^3] The documentation shows the same structure for joined, single and concrete table inheritance.[^5]
- Hibernate: each entity has a persister, and the class of the persister follows the table design. [SingleTableEntityPersister at 6.6.9](https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/SingleTableEntityPersister.java#L56-L68) is "implemented as a single table for all classes of the hierarchy, with a discriminator column used to determine which concrete class a row represents."[^6] [JoinedSubclassEntityPersister](https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/JoinedSubclassEntityPersister.java#L83-L96) uses "a separate table for each subclass"[^7], and [UnionSubclassEntityPersister](https://github.com/hibernate/hibernate-orm/blob/c93425dd920293564a43c7b082feab3b30193fb9/hibernate-core/src/main/java/org/hibernate/persister/entity/UnionSubclassEntityPersister.java#L69-L80) uses "a separate table for each concrete class, with all inherited attributes persisted as columns of that table."[^8] All three extend `AbstractEntityPersister`.

## References
