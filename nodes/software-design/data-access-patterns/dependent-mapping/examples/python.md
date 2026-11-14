```python run id=owner-maps-children
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


class AlbumMapper:  # The owner's mapper loads, saves and deletes the tracks.
    def insert(self, album):
        album.id = db.execute("insert into album (title) values (?)", (album.title,)).lastrowid
        return self._insert_tracks(album)

    def update(self, album):
        db.execute("update album set title = ? where id = ?", (album.title, album.id))
        deleted = db.execute("delete from track where album_id = ?", (album.id,)).rowcount
        return deleted, self._insert_tracks(album)

    def delete(self, album):
        db.execute("delete from track where album_id = ?", (album.id,))
        db.execute("delete from album where id = ?", (album.id,))

    def _insert_tracks(self, album):
        rows = [(album.id, i + 1, t.title) for i, t in enumerate(album.tracks)]
        db.executemany("insert into track values (?, ?, ?)", rows)
        return len(rows)

    def find(self, id):
        (title,) = db.execute("select title from album where id = ?", (id,)).fetchone()
        sql = "select title from track where album_id = ? order by seq"
        return Album(title, [Track(t) for (t,) in db.execute(sql, (id,))], id)


def show_tracks():
    rows = db.execute("select album_id, seq, title from track order by album_id, seq")
    return " | ".join(f"{a}/{s} {t}" for a, s, t in rows)


mapper = AlbumMapper()
mapper.insert(Album("Kind of Blue", [Track("So What"), Track("Blue in Green")]))
print("tracks:", show_tracks())

album = mapper.find(1)
print(f"loaded: {album.title} | {', '.join(t.title for t in album.tracks)}")

album.tracks.insert(1, Track("Freddie Freeloader"))
deleted, inserted = mapper.update(album)
print(f"update: deleted {deleted}, inserted {inserted}")
print("tracks:", show_tracks())

mapper.delete(album)
(albums,) = db.execute("select count(*) from album").fetchone()
(tracks,) = db.execute("select count(*) from track").fetchone()
print(f"delete album 1: albums {albums}, tracks {tracks}")
```

```output
tracks: 1/1 So What | 1/2 Blue in Green
loaded: Kind of Blue | So What, Blue in Green
update: deleted 2, inserted 3
tracks: 1/1 So What | 1/2 Freddie Freeloader | 1/3 Blue in Green
delete album 1: albums 0, tracks 0
```

- Verdict: idiom. `Track` has no key and no mapper. `AlbumMapper` writes the tracks with the album, with `(album_id, seq)` as the key of the track table, so the position in the list is part of the key.
- `update` deletes all the track rows of the album and inserts the list again, and `delete` removes the tracks before the album.
