---
tag: GV1SJG
kind: pattern
slug: dependent-mapping
name: Dependent Mapping
aliases: [dependent mapping pattern, dependent object mapping, owned collection mapping]
area: software-design/data-access-patterns
level: core
summary: >-
  In Dependent Mapping, the mapper of an owner class also loads, saves and deletes the child objects that belong only to that owner, such as the tracks of an album.
family: architectural
polarity: positive
intent: "Has one class perform the database mapping for a child class."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). ORMs give it as owned entity types and as cascades that delete orphans.
see_also: [foreign-key-mapping, embedded-value, association-table-mapping, identity-field, unit-of-work, data-mapper]
sources:
  - url: https://martinfowler.com/eaaCatalog/dependentMapping.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Dependent Mapping
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Dependent Mapping
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/owned-entities
    title: Entity Framework Core documentation, Owned Entity Types
    locator: introduction; Collections of owned types; Querying owned types
  - url: https://docs.sqlalchemy.org/en/20/orm/cascades.html
    title: SQLAlchemy 2.0 documentation, Cascades
    locator: delete-orphan
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/dependency.py#L466-L520
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/dependency.py
    locator: OneToManyDP.presort_saves, lines 466 to 520
---

## Intent

In a Dependent Mapping, one class "Has one class perform the database mapping for a child class."[^1] The child objects, the dependents, have no mapper of their own. The mapper of their owner loads, saves and deletes them with the owner.

## Problem

"Some objects naturally appear in the context of other objects. Tracks on an album may be loaded or saved whenever the underlying album is loaded or saved."[^1] A program that gives each track its own mapper, its own key in memory and its own place in the [[identity-map]] does work that buys nothing: no code finds a track except through its album. The program must also keep the two mappers in step, so that a track is not saved before its album or left behind when its album is deleted.

## Solution

Fowler gives the condition and the step: "If they aren't referenced to by any other table in the database, you can simplify the mapping procedure by having the album mapper perform the mapping for the tracks as well".[^1] The owner's mapper loads the tracks when it loads the album, writes them when it saves the album, and deletes them when it deletes the album. A dependent has one owner, and nothing outside the owner refers to it.[^2]

```text
  memory                                  database
  Album(1, "Kind of Blue")                album(1, Kind of Blue)
    tracks = [So What, Blue in Green]     track(album_id 1, seq 1, So What)
                                          track(album_id 1, seq 2, Blue in Green)
  AlbumMapper.find(1)   -> album row + track rows of album 1
  AlbumMapper.update(a) -> update album row; delete track rows of album 1; insert them again
  AlbumMapper.delete(a) -> delete track rows of album 1; delete album row
```

The participants are the **owner**, which has an identity and a mapper; the **dependents**, which have no key in memory and no mapper; and the **owner's mapper**, which does all the database work for both. The track table has a key in the database, but it is the key of the owner and the position of the track, and the class does not need it.

The following program saves an album with two tracks, adds a track in the middle, and saves the album again.

```python run id=owner-saves-tracks
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table album (id integer primary key, title text);
create table track (album_id integer references album(id), seq integer, title text,
                    primary key (album_id, seq));
""")


class Track:  # A dependent: no key and no mapper of its own.
    def __init__(self, title):
        self.title = title


class Album:
    def __init__(self, title, tracks, id=None):
        self.id, self.title, self.tracks = id, title, tracks


class AlbumMapper:  # The owner's mapper also maps the tracks.
    def insert(self, album):
        album.id = db.execute("insert into album (title) values (?)", (album.title,)).lastrowid
        self._insert_tracks(album)

    def update(self, album):
        db.execute("update album set title = ? where id = ?", (album.title, album.id))
        db.execute("delete from track where album_id = ?", (album.id,))  # Delete all, then insert.
        self._insert_tracks(album)

    def _insert_tracks(self, album):
        rows = [(album.id, i + 1, t.title) for i, t in enumerate(album.tracks)]
        db.executemany("insert into track values (?, ?, ?)", rows)

    def find(self, id):
        (title,) = db.execute("select title from album where id = ?", (id,)).fetchone()
        sql = "select title from track where album_id = ? order by seq"
        return Album(title, [Track(t) for (t,) in db.execute(sql, (id,))], id)


mapper = AlbumMapper()
mapper.insert(Album("Kind of Blue", [Track("So What"), Track("Blue in Green")]))
album = mapper.find(1)
album.tracks.insert(1, Track("Freddie Freeloader"))
mapper.update(album)
print(db.execute("select * from track order by seq").fetchall())
```

```output
[(1, 1, 'So What'), (1, 2, 'Freddie Freeloader'), (1, 3, 'Blue in Green')]
```

Because the tracks have no identity, the mapper does not need to find out which track changed. Fowler's book describes this way to save them: delete all the rows of the dependents and insert them again.[^2] The order of the list becomes the `seq` column.

## Example

Each example file inserts an album with two tracks and prints the track rows as album, position and title. It loads the album, and the tracks come with it. It then adds a track in the middle of the list and saves the album: the mapper deletes the two old rows and inserts three. The last step deletes the album, and the mapper deletes its tracks first, so no track row is left without its album. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the two tables in memory, with the track table keyed by the album key and the position.

## Consequences

**Benefits.**

- **Less mapping code.** One mapper does the work, and the dependents need no mapper, no key field and no entry in the identity map.
- **The owner and its dependents stay consistent.** They load together, save together and are deleted together, so the program never sees an album with part of its tracks.
- **A simple save.** Delete all and insert again needs no tracking of changes in the collection.

**Liabilities.**

**The rule breaks when something else refers to a dependent.** Delete all and insert again gives the rows new keys. If another table refers to a dependent by its key, that reference now points to a row with other data, or to no row at all. In the following program, a playlist refers to the track Blue in Green by its key 2. The user moves that track to the top of the album, the mapper saves the tracks by delete and insert, and the playlist now points to So What.

```python wrong id=outside-reference
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table track (id integer primary key, album_id integer, title text);
create table playlist_entry (playlist text, track_id integer references track(id));
insert into track (album_id, title) values (1, 'So What'), (1, 'Blue in Green');
-- A second table refers to a track, so the track is no longer a dependent.
insert into playlist_entry values ('Evening', 2);
""")

# The user moves Blue in Green to the top. The album mapper saves the tracks
# of album 1 by delete all, then insert, and the rows get new keys.
db.execute("delete from track where album_id = 1")
db.executemany("insert into track (album_id, title) values (1, ?)", [("Blue in Green",), ("So What",)])

sql = "select p.playlist, p.track_id, t.title from playlist_entry p left join track t on t.id = p.track_id"
print(db.execute(sql).fetchall())
```

```output
[('Evening', 2, 'So What')]
```

If something outside the owner must refer to the child, the child is no longer a dependent. Give it an identity, a mapper and a [[foreign-key-mapping]] of its own, and save changes to it row by row.

**Many writes for a small change.** Changing the title of one track rewrites all the rows of the album. For large collections, compare the collection with the rows instead, as a [[unit-of-work]] does.

**All dependents load with the owner.** The owner's mapper loads the whole collection each time. Entity Framework Core does the same: "When querying the owner the owned types will be included by default."[^3]

## When not to use it

**When the child is referenced from outside.** This is the condition in Fowler's summary. A track that a playlist or a sales table refers to is an entity, not a dependent.

**When the child is shared.** A dependent has one owner. A child that two owners hold, such as a skill of many employees, needs its own table and an [[association-table-mapping]].

**When the child is a single small value.** A value with a fixed set of fields fits in columns of the owner's row. Use an [[embedded-value]].

## Variants and related patterns

**Owned entity types.** In Entity Framework Core, "Owned entities are essentially a part of the owner and cannot exist without it, they are conceptually similar to aggregates ."[^3] "To configure a collection of owned types use OwnsMany in OnModelCreating ." By default, the key of an owned collection is the owner's key plus another property, and so "the relationship between the owner and the owned entity becomes immutable and reflects aggregate semantics better."[^3]

**Delete-orphan cascades.** An ORM that maps the child as a class of its own can still give it the life cycle of a dependent. In SQLAlchemy, "delete-orphan cascade adds behavior to the delete cascade, such that a child object will be marked for deletion when it is de-associated from the parent, not just when the parent is marked for deletion." It "implies that each child object can only have one parent at a time".[^4]

**Aggregates.** In domain-driven design, an aggregate is a group of objects that change together through one root. The root's mapper and its dependents are a way to save an aggregate.

**Embedded Value.** An embedded value is a dependent that is stored in the owner's row. See [[embedded-value]].

## Known uses

- SQLAlchemy: a `relationship()` with the `delete-orphan` cascade. In [OneToManyDP.presort_saves in dependency.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/dependency.py#L466-L520), each child that was removed from the collection and has no parent is registered for deletion with `isdelete=True` when `self.cascade.delete_orphan` is set.[^5]
- Entity Framework Core, for .NET: `OwnsMany` maps a collection of owned types to a table whose key includes the owner's key, and the owner's queries include it.[^3]
- Java Persistence: `@ElementCollection` maps a collection of embeddable objects to a table that the owner's entity manages, and `orphanRemoval = true` on a one-to-many association deletes the children that leave the collection.

## References
