```javascript run id=foreign-keys-and-collections
// A stand-in for the database: three tables, each keyed by id, and one row counter per table.
const db = { artist: new Map(), album: new Map(), track: new Map() };
const lastId = { artist: 0, album: 0, track: 0 };

function insert(table, row) {
  // Like a FOREIGN KEY constraint: a key that is not null must find its parent row.
  if (table === "album" && row.artistId !== null && !db.artist.has(row.artistId)) {
    throw new Error("FOREIGN KEY constraint failed");
  }
  if (table === "track" && row.albumId !== null && !db.album.has(row.albumId)) {
    throw new Error("FOREIGN KEY constraint failed");
  }
  lastId[table] += 1;
  db[table].set(lastId[table], row);
  return lastId[table];
}

class Artist {
  constructor(name, id = null) {
    this.id = id;
    this.name = name;
  }
}

class Track {
  constructor(title, id = null) {
    this.id = id;
    this.title = title;
  }
}

class Album {
  constructor(title, artist, tracks, id = null) {
    this.id = id;
    this.title = title;
    this.artist = artist;
    this.tracks = tracks;
  }
}

function insertAlbum(album) {
  // Many-to-one: write the key of the referenced artist.
  album.id = insert("album", { title: album.title, artistId: album.artist.id });
  saveTracks(album);
}

function saveTracks(album) {
  // One-to-many: find the difference between the collection and the rows.
  const inDb = [...db.track].filter(([, r]) => r.albumId === album.id).map(([id]) => id);
  const inMemory = new Set(album.tracks.filter((t) => t.id !== null).map((t) => t.id));
  let inserted = 0;
  for (const t of album.tracks) {
    if (t.id === null) {
      t.id = insert("track", { title: t.title, albumId: album.id });
      inserted += 1;
    }
  }
  const removed = inDb.filter((id) => !inMemory.has(id));
  for (const id of removed) db.track.delete(id);
  return [removed.length, inserted];
}

function loadAlbum(id) {
  const row = db.album.get(id);
  const artist = new Artist(db.artist.get(row.artistId).name, row.artistId);
  const tracks = [...db.track]
    .filter(([, r]) => r.albumId === id)
    .sort(([a], [b]) => a - b)
    .map(([tid, r]) => new Track(r.title, tid));
  return new Album(row.title, artist, tracks, id);
}

function showTracks() {
  return [...db.track]
    .sort(([a], [b]) => a - b)
    .map(([id, r]) => `${id} ${r.title} album_id=${r.albumId}`)
    .join(" | ");
}

const miles = new Artist("Miles Davis");
miles.id = insert("artist", { name: miles.name });
insertAlbum(new Album("Kind of Blue", miles, [new Track("So What"), new Track("Blue in Green")]));
for (const [id, r] of db.album) console.log(`album rows: ${id} ${r.title} artist_id=${r.artistId}`);
console.log(`track rows: ${showTracks()}`);

const album = loadAlbum(1);
const titles = album.tracks.map((t) => t.title).join(", ");
console.log(`loaded: ${album.title} by ${album.artist.name} | ${titles}`);

album.tracks = album.tracks.filter((t) => t.title !== "Blue in Green");
album.tracks.push(new Track("Freddie Freeloader"));
const [deleted, inserted] = saveTracks(album);
console.log(`update tracks: deleted ${deleted}, inserted ${inserted}`);
console.log(`track rows: ${showTracks()}`);

try {
  insert("album", { title: "Unknown", artistId: 99 });
} catch (e) {
  console.log(`artist_id 99: ${e.message}`);
}
```

```output
album rows: 1 Kind of Blue artist_id=1
track rows: 1 So What album_id=1 | 2 Blue in Green album_id=1
loaded: Kind of Blue by Miles Davis | So What, Blue in Green
update tracks: deleted 1, inserted 1
track rows: 1 So What album_id=1 | 3 Freddie Freeloader album_id=1
artist_id 99: FOREIGN KEY constraint failed
```

- Verdict: idiom. The tables are `Map` objects keyed by id, and `insert` checks each key that is not `null` against the parent table, like a foreign key constraint.
- `saveTracks` filters the track table by `albumId` to find the rows of the collection, and uses a `Set` of the keys in memory to find the rows to delete.
