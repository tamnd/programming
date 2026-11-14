---
tag: 3HNEAC
kind: pattern
slug: foreign-key-mapping
name: Foreign Key Mapping
aliases: [foreign key mapping pattern, many-to-one mapping, one-to-many mapping]
area: software-design/data-access-patterns
level: core
summary: >-
  A Foreign Key Mapping saves a reference from one object to another as a foreign key column that holds the key of the other object's row, and turns the key back into a reference on load.
family: architectural
polarity: positive
intent: "Maps an association between objects to a foreign key reference between tables."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Object-relational mappers implement it as the many-to-one field, such as the ForeignKey of Django and the relationship() of SQLAlchemy.
see_also: [identity-field, identity-map, lazy-load, unit-of-work, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/foreignKeyMapping.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Foreign Key Mapping
    locator: pattern summary
  - url: https://en.wikipedia.org/wiki/Foreign_key
    title: Foreign key, Wikipedia
    locator: lead section and Summary
  - url: https://docs.sqlalchemy.org/en/20/orm/basic_relationships.html
    title: SQLAlchemy 2.0 documentation, Basic Relationship Patterns
    locator: One To Many; Many To One
  - url: https://www.sqlite.org/foreignkeys.html
    title: SQLite documentation, SQLite Foreign Key Support
    locator: 1. Introduction to Foreign Key Constraints; 2. Enabling Foreign Key Support; 3. Required and Suggested Database Indexes
  - url: https://docs.djangoproject.com/en/5.2/ref/models/fields/
    title: Django 5.2 documentation, Model field reference
    locator: ForeignKey; Database Representation
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/relationships/foreign-and-principal-keys
    title: Entity Framework Core documentation, Foreign and principal keys in relationships
    locator: introduction; Shadow foreign keys
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Foreign Key Mapping
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1218-L1255
    title: Django 5.2.8 source, django/db/models/base.py
    locator: Model._prepare_related_fields_for_save, lines 1218 to 1255
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L958-L965
    title: Django 5.2.8 source, django/db/models/fields/related.py
    locator: class ForeignKey and its docstring, lines 958 to 965
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L277-L351
    title: Django 5.2.8 source, django/db/models/fields/related_descriptors.py
    locator: ForwardManyToOneDescriptor.__set__, lines 277 to 351
---

## Intent

A Foreign Key Mapping "Maps an association between objects to a foreign key reference between tables."[^1] A reference from one object to another is saved as the key of the other object's row. On load, the mapper reads the key and finds the object again.

## Problem

"Objects can refer to each other directly by object references."[^1] An album refers to its artist, and holds a list of its tracks. Fowler states the difficulty: "To save these objects to a database, it's vital to save these references. However, since the data in them is specific to the specific instance of the running program, you can't just save raw data values."[^1] A memory address means nothing in the next run of the program.

A collection is harder. Fowler notes that a list of references in one field breaks the first normal form of a relational database, because a column holds one value, not a list.[^1]

## Solution

"A Foreign Key Mapping maps an object reference to a foreign key in the database."[^1] Each object has an [[identity-field]], so each row has a key. "A foreign key is a set of attributes in a table that refers to the primary key of another table, linking these two tables."[^2]

- **A single reference** (many-to-one). The table of the referring object gets a column with the key of the referenced row. To save `album.artist`, the mapper writes `artist.id` to `album.artist_id`. To load it, the mapper reads `artist_id` and finds the artist, if possible through the [[identity-map]].
- **A collection** (one-to-many). The key goes on the other side. "A one to many relationship places a foreign key on the child table referencing the parent."[^3] Each track row holds the key of its album, and the mapper loads `album.tracks` with a query on `track.album_id`.

```text
  memory                                   database
  Album "Kind of Blue"                      album(id=1, title, artist_id=1)
    .artist  --> Artist "Miles Davis"       artist(id=1, name)
    .tracks  --> [So What, Blue in Green]   track(id=1, title, album_id=1)
                                            track(id=2, title, album_id=1)
  save: reference -> key of the referenced row
  load: key -> query -> reference
```

The participants are the **referring object** and its table, which holds the foreign key column; the **referenced object**, whose identity field gives the key; and the **mapper**, which turns references into keys on save and keys into references on load.

The following program saves an album with its artist and two tracks, and then loads it from the keys alone.

```python run id=references-to-keys
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("pragma foreign_keys = on")
db.executescript("""
create table artist (id integer primary key, name text);
create table album (id integer primary key, title text,
                    artist_id integer references artist(id));
create table track (id integer primary key, title text,
                    album_id integer references album(id));
""")


class Artist:
    def __init__(self, name, id=None):
        self.id, self.name = id, name


class Album:
    def __init__(self, title, artist, tracks, id=None):
        self.id, self.title = id, title
        self.artist = artist  # An object reference in memory.
        self.tracks = tracks  # A collection of references in memory.


def save(album):
    # Many-to-one: the reference becomes the key of the artist row.
    sql = "insert into album (title, artist_id) values (?, ?)"
    album.id = db.execute(sql, (album.title, album.artist.id)).lastrowid
    # One-to-many: each track row gets the key of the album.
    for title in album.tracks:
        db.execute("insert into track (title, album_id) values (?, ?)", (title, album.id))


def load(album_id):
    sql = "select title, artist_id from album where id = ?"
    title, artist_id = db.execute(sql, (album_id,)).fetchone()
    name = db.execute("select name from artist where id = ?", (artist_id,)).fetchone()[0]
    sql = "select title from track where album_id = ? order by id"
    tracks = [t for (t,) in db.execute(sql, (album_id,))]
    return Album(title, Artist(name, artist_id), tracks, album_id)  # Keys become references.


miles = Artist("Miles Davis")
miles.id = db.execute("insert into artist (name) values (?)", (miles.name,)).lastrowid
save(Album("Kind of Blue", miles, ["So What", "Blue in Green"]))
print(db.execute("select * from album").fetchall())
print(db.execute("select * from track").fetchall())
album = load(1)
print(album.title, "by", album.artist.name, "|", ", ".join(album.tracks))
```

```output
[(1, 'Kind of Blue', 1)]
[(1, 'So What', 1), (2, 'Blue in Green', 1)]
Kind of Blue by Miles Davis | So What, Blue in Green
```

## Example

Each example file saves an artist, an album and two tracks, and prints the key columns. It loads the album and rebuilds the references from the keys. It then removes one track from the collection, adds a new one, and saves the collection by comparing it with the rows: the mapper inserts the track that has no key and deletes the row that is no longer in the list. The last line tries to save an album that refers to artist 99, which does not exist, and the foreign key constraint stops it. The Python file uses SQLite with `pragma foreign_keys = on`. The JavaScript, Java, Go and Rust files keep the tables in memory and check the key in the insert function in the same way.

## Consequences

**Benefits.**

- **The database can check the references.** With a foreign key constraint, "Attempting to insert a row into the track table that does not correspond to any row in the artist table will fail, as will attempting to delete a row from the artist table when there exist dependent rows in the track table."[^4]
- **Normalized tables.** Each column holds one value. A one-to-many collection needs no list column, because each child row holds one key.
- **Frameworks hide the key.** Django stores a `ForeignKey` named `manufacturer` in a column named `manufacturer_id`, and the programmer works with the object: "You’ll always deal with the field names of your model object."[^5] Entity Framework Core can keep the foreign key as a shadow property, which "exists in the EF model but does not exist on the .NET type."[^6]

**Liabilities.**

**The order of the saves matters.** The referenced row must have its key before the referring row is written. If the album is saved first, the artist has no key yet, and the album row gets a null. The constraint does not catch this, because "if the foreign key column for an entry in the track table is NULL, then no corresponding entry in the artist table is required."[^4] The following program saves the album first. The artist gets its key a moment later, but the album row keeps the null, and the program sees no error.

```python wrong id=child-saved-before-parent
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("pragma foreign_keys = on")
db.execute("create table artist (id integer primary key, name text)")
db.execute("create table album (id integer primary key, title text, artist_id integer references artist(id))")


class Artist:
    def __init__(self, name):
        self.id, self.name = None, name


artist = Artist("Miles Davis")
# The album is saved first, so the artist has no key yet.
db.execute("insert into album (title, artist_id) values (?, ?)", ("Kind of Blue", artist.id))
artist.id = db.execute("insert into artist (name) values (?)", (artist.name,)).lastrowid
print(db.execute("select * from album").fetchall())
```

```output
[(1, 'Kind of Blue', None)]
```

**Updates to a collection.** When the program adds or removes items in a collection, the mapper must find which rows to insert, which to delete and which to change. Fowler's book describes three ways: delete all the child rows and insert them again, keep a reference back to the owner in each child, or compare the collection with the rows that the database holds.[^7] The examples use the last way.

**The constraint may be off.** The database checks the keys only if the constraint is declared and enabled. In SQLite, "Foreign key constraints are disabled by default", and each connection must turn them on.[^4]

**Queries on the child key.** Each load of a collection, and each delete of a parent row, searches the child table by the foreign key. SQLite advises that "in most real systems, an index should be created on the child key columns of each foreign key constraint."[^4]

**Loading one object can load many.** If the mapper turns every key into an object at once, loading one album loads its artist, the artist's other albums and so on. A [[lazy-load]] keeps the key and loads the object only when the program uses it.

## When not to use it

**For many-to-many associations.** A foreign key column holds one key, so a track that is on many playlists cannot be a single column. Use a link table, as in [[association-table-mapping]]. Entity Framework Core describes many-to-many relationships as "composed of two one-to-many relationships, each of which is itself defined by a foreign key referencing a principal key."[^6]

**For values that belong to one owner.** A value that has no identity of its own, such as an address that only one customer uses, does not need its own table and key. It can be stored in the columns of its owner, as in [[embedded-value]].

**For data that the database never queries.** A structure that the program always reads and writes as a whole can be stored in one column, as in [[serialized-lob]].

## How to fix it

Save the referenced objects first: the artist, then the album, then its tracks. A [[unit-of-work]] can sort the inserts in this order. Declare the key column `NOT NULL` when every row must have a parent, so that a missing key fails at once. Django checks this before it saves: "If the field is nullable, allowing the save would result in silent data loss", so a save with an unsaved related object raises a `ValueError`.[^8]

## Variants and related patterns

**Bidirectional associations.** The album can hold its tracks and each track can hold its album. Both references map to the same foreign key column, and the mapper must keep the two sides in step. SQLAlchemy links the two sides with the `back_populates` argument of `relationship()`.[^3]

**Dependent Mapping.** When the children have no meaning without their owner, such as tracks without an album, the owner's mapper can save and load them, and the children need no mapper of their own. See [[dependent-mapping]].

**Association Table Mapping.** A many-to-many association uses a separate table of key pairs. See [[association-table-mapping]].

**Identity Field and Identity Map.** The foreign key is the identity field of the referenced object, and the identity map makes sure that two albums of one artist refer to one artist object. See [[identity-field]] and [[identity-map]].

## Known uses

- Django: the [ForeignKey class in related.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L958-L965) is documented as "Provide a many-to-one relation by adding a column to the local model to hold the remote value."[^9] When the program writes `child.parent = parent`, [ForwardManyToOneDescriptor.__set__](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L277-L351) copies the key: `setattr(instance, lh_field.attname, getattr(value, rh_field.attname))`.[^10] Before a save, [`_prepare_related_fields_for_save`](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1218-L1255) raises an error if a related object has no key yet.[^8]
- SQLAlchemy: `relationship()` together with a `ForeignKey` column. "Many to one places a foreign key in the parent table referencing the child."[^3]
- Entity Framework Core, for .NET: "All one-to-one and one-to-many relationships are defined by a foreign key on the dependent end that references a primary or alternate key on the principal end."[^6]

## References
