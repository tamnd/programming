```python run id=foreign-keys-and-collections
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


class Track:
    def __init__(self, title, id=None):
        self.id, self.title = id, title


class Album:
    def __init__(self, title, artist, tracks, id=None):
        self.id, self.title, self.artist, self.tracks = id, title, artist, tracks


def insert_album(album):
    # Many-to-one: write the key of the referenced artist.
    sql = "insert into album (title, artist_id) values (?, ?)"
    album.id = db.execute(sql, (album.title, album.artist.id)).lastrowid
    save_tracks(album)


def save_tracks(album):
    # One-to-many: find the difference between the collection and the rows.
    in_db = {i for (i,) in db.execute("select id from track where album_id = ?", (album.id,))}
    in_memory = {t.id for t in album.tracks if t.id is not None}
    inserted = 0
    for t in album.tracks:
        if t.id is None:
            sql = "insert into track (title, album_id) values (?, ?)"
            t.id = db.execute(sql, (t.title, album.id)).lastrowid
            inserted += 1
    removed = sorted(in_db - in_memory)
    for i in removed:
        db.execute("delete from track where id = ?", (i,))
    return len(removed), inserted


def load_album(id):
    title, artist_id = db.execute("select title, artist_id from album where id = ?", (id,)).fetchone()
    (name,) = db.execute("select name from artist where id = ?", (artist_id,)).fetchone()
    rows = db.execute("select id, title from track where album_id = ? order by id", (id,))
    return Album(title, Artist(name, artist_id), [Track(t, i) for i, t in rows], id)


def show_tracks():
    rows = db.execute("select id, title, album_id from track order by id")
    return " | ".join(f"{i} {t} album_id={a}" for i, t, a in rows)


miles = Artist("Miles Davis")
miles.id = db.execute("insert into artist (name) values (?)", (miles.name,)).lastrowid
insert_album(Album("Kind of Blue", miles, [Track("So What"), Track("Blue in Green")]))
for i, t, a in db.execute("select id, title, artist_id from album"):
    print(f"album rows: {i} {t} artist_id={a}")
print("track rows:", show_tracks())

album = load_album(1)
titles = ", ".join(t.title for t in album.tracks)
print(f"loaded: {album.title} by {album.artist.name} | {titles}")

album.tracks = [t for t in album.tracks if t.title != "Blue in Green"]
album.tracks.append(Track("Freddie Freeloader"))
deleted, inserted = save_tracks(album)
print(f"update tracks: deleted {deleted}, inserted {inserted}")
print("track rows:", show_tracks())

try:
    db.execute("insert into album (title, artist_id) values ('Unknown', 99)")
except sqlite3.IntegrityError as e:
    print("artist_id 99:", e)
```

```output
album rows: 1 Kind of Blue artist_id=1
track rows: 1 So What album_id=1 | 2 Blue in Green album_id=1
loaded: Kind of Blue by Miles Davis | So What, Blue in Green
update tracks: deleted 1, inserted 1
track rows: 1 So What album_id=1 | 3 Freddie Freeloader album_id=1
artist_id 99: FOREIGN KEY constraint failed
```

- Verdict: idiom. The mapper writes `album.artist.id` to the `artist_id` column, and each track row gets `album.id`. On load, it reads the keys and builds the `Artist` and the list of tracks again.
- `save_tracks` compares the keys in memory with the keys in the table. It inserts before it deletes, so SQLite does not give the new track the key of the deleted one. SQLite checks the constraints only because the program runs `pragma foreign_keys = on`.
