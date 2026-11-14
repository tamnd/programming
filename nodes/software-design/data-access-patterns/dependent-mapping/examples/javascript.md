```javascript run id=owner-maps-children
// A stand-in for the database: an album table, and a track table keyed by "albumId/seq".
const albumTable = new Map();
const trackTable = new Map();
let lastAlbum = 0;

class Track {
  // A dependent: no key and no mapper of its own.
  constructor(title) {
    this.title = title;
  }
}

class Album {
  constructor(title, tracks, id = null) {
    this.id = id;
    this.title = title;
    this.tracks = tracks;
  }
}

class AlbumMapper {
  // The owner's mapper loads, saves and deletes the tracks.
  insert(album) {
    lastAlbum += 1;
    albumTable.set(lastAlbum, album.title);
    album.id = lastAlbum;
    return this.insertTracks(album);
  }

  update(album) {
    albumTable.set(album.id, album.title);
    return [this.deleteTracks(album.id), this.insertTracks(album)];
  }

  delete(album) {
    this.deleteTracks(album.id);
    albumTable.delete(album.id);
  }

  insertTracks(album) {
    album.tracks.forEach((t, i) => trackTable.set(`${album.id}/${i + 1}`, t.title));
    return album.tracks.length;
  }

  deleteTracks(albumId) {
    const keys = [...trackTable.keys()].filter((k) => k.startsWith(`${albumId}/`));
    for (const k of keys) trackTable.delete(k);
    return keys.length;
  }

  find(id) {
    const tracks = trackRows()
      .filter(([a]) => a === id)
      .map(([, , title]) => new Track(title));
    return new Album(albumTable.get(id), tracks, id);
  }
}

function trackRows() {
  return [...trackTable]
    .map(([k, title]) => [...k.split("/").map(Number), title])
    .sort((x, y) => x[0] - y[0] || x[1] - y[1]);
}

function showTracks() {
  return trackRows()
    .map(([a, s, t]) => `${a}/${s} ${t}`)
    .join(" | ");
}

const mapper = new AlbumMapper();
mapper.insert(new Album("Kind of Blue", [new Track("So What"), new Track("Blue in Green")]));
console.log(`tracks: ${showTracks()}`);

const album = mapper.find(1);
console.log(`loaded: ${album.title} | ${album.tracks.map((t) => t.title).join(", ")}`);

album.tracks.splice(1, 0, new Track("Freddie Freeloader"));
const [deleted, inserted] = mapper.update(album);
console.log(`update: deleted ${deleted}, inserted ${inserted}`);
console.log(`tracks: ${showTracks()}`);

mapper.delete(album);
console.log(`delete album 1: albums ${albumTable.size}, tracks ${trackTable.size}`);
```

```output
tracks: 1/1 So What | 1/2 Blue in Green
loaded: Kind of Blue | So What, Blue in Green
update: deleted 2, inserted 3
tracks: 1/1 So What | 1/2 Freddie Freeloader | 1/3 Blue in Green
delete album 1: albums 0, tracks 0
```

- Verdict: idiom. The track table is a `Map` keyed by the string `"albumId/seq"`, and `Track` holds only a title.
- `deleteTracks` collects the keys of the album first and then deletes them, so the loop does not change the map that it reads.
