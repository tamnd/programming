```rust run id=foreign-keys-and-collections
use std::collections::{BTreeMap, HashSet};

struct AlbumRow {
    title: String,
    artist_id: Option<i64>,
}

struct TrackRow {
    title: String,
    album_id: Option<i64>,
}

// A stand-in for the database: three tables, each keyed by id, and one row counter per table.
#[derive(Default)]
struct Db {
    artist: BTreeMap<i64, String>,
    album: BTreeMap<i64, AlbumRow>,
    track: BTreeMap<i64, TrackRow>,
    last_artist: i64,
    last_album: i64,
    last_track: i64,
}

const FK_ERROR: &str = "FOREIGN KEY constraint failed";

impl Db {
    fn insert_artist(&mut self, name: &str) -> i64 {
        self.last_artist += 1;
        self.artist.insert(self.last_artist, name.to_string());
        self.last_artist
    }

    fn insert_album(&mut self, row: AlbumRow) -> Result<i64, &'static str> {
        // Like a FOREIGN KEY constraint: a key that is not None must find its parent row.
        if let Some(a) = row.artist_id {
            if !self.artist.contains_key(&a) {
                return Err(FK_ERROR);
            }
        }
        self.last_album += 1;
        self.album.insert(self.last_album, row);
        Ok(self.last_album)
    }

    fn insert_track(&mut self, row: TrackRow) -> Result<i64, &'static str> {
        if let Some(a) = row.album_id {
            if !self.album.contains_key(&a) {
                return Err(FK_ERROR);
            }
        }
        self.last_track += 1;
        self.track.insert(self.last_track, row);
        Ok(self.last_track)
    }

    fn show_tracks(&self) -> String {
        let parts: Vec<String> = self
            .track
            .iter()
            .map(|(id, r)| format!("{} {} album_id={}", id, r.title, r.album_id.unwrap()))
            .collect();
        parts.join(" | ")
    }
}

struct Artist {
    id: Option<i64>,
    name: String,
}

struct Track {
    id: Option<i64>,
    title: String,
}

struct Album {
    id: Option<i64>,
    title: String,
    artist: Artist,
    tracks: Vec<Track>,
}

fn track(title: &str) -> Track {
    Track { id: None, title: title.to_string() }
}

fn insert_album(db: &mut Db, album: &mut Album) -> Result<(), &'static str> {
    // Many-to-one: write the key of the referenced artist.
    let row = AlbumRow { title: album.title.clone(), artist_id: album.artist.id };
    album.id = Some(db.insert_album(row)?);
    save_tracks(db, album)?;
    Ok(())
}

fn save_tracks(db: &mut Db, album: &mut Album) -> Result<(usize, usize), &'static str> {
    // One-to-many: find the difference between the collection and the rows.
    let in_db: Vec<i64> = db.track.iter().filter(|(_, r)| r.album_id == album.id).map(|(id, _)| *id).collect();
    let in_memory: HashSet<i64> = album.tracks.iter().filter_map(|t| t.id).collect();
    let mut inserted = 0;
    for t in album.tracks.iter_mut() {
        if t.id.is_none() {
            t.id = Some(db.insert_track(TrackRow { title: t.title.clone(), album_id: album.id })?);
            inserted += 1;
        }
    }
    let mut deleted = 0;
    for id in in_db {
        if !in_memory.contains(&id) {
            db.track.remove(&id);
            deleted += 1;
        }
    }
    Ok((deleted, inserted))
}

fn load_album(db: &Db, id: i64) -> Album {
    let row = &db.album[&id];
    let artist_id = row.artist_id.unwrap();
    let artist = Artist { id: Some(artist_id), name: db.artist[&artist_id].clone() };
    let tracks = db
        .track
        .iter()
        .filter(|(_, r)| r.album_id == Some(id))
        .map(|(tid, r)| Track { id: Some(*tid), title: r.title.clone() })
        .collect();
    Album { id: Some(id), title: row.title.clone(), artist, tracks }
}

fn main() {
    let mut db = Db::default();
    let mut miles = Artist { id: None, name: "Miles Davis".to_string() };
    miles.id = Some(db.insert_artist(&miles.name));
    let mut kind_of_blue = Album {
        id: None,
        title: "Kind of Blue".to_string(),
        artist: miles,
        tracks: vec![track("So What"), track("Blue in Green")],
    };
    if let Err(e) = insert_album(&mut db, &mut kind_of_blue) {
        println!("{}", e);
    }
    for (id, r) in &db.album {
        println!("album rows: {} {} artist_id={}", id, r.title, r.artist_id.unwrap());
    }
    println!("track rows: {}", db.show_tracks());

    let mut album = load_album(&db, 1);
    let titles: Vec<&str> = album.tracks.iter().map(|t| t.title.as_str()).collect();
    println!("loaded: {} by {} | {}", album.title, album.artist.name, titles.join(", "));

    album.tracks.retain(|t| t.title != "Blue in Green");
    album.tracks.push(track("Freddie Freeloader"));
    match save_tracks(&mut db, &mut album) {
        Ok((deleted, inserted)) => println!("update tracks: deleted {}, inserted {}", deleted, inserted),
        Err(e) => println!("{}", e),
    }
    println!("track rows: {}", db.show_tracks());

    let bad = AlbumRow { title: "Unknown".to_string(), artist_id: Some(99) };
    if let Err(e) = db.insert_album(bad) {
        println!("artist_id 99: {}", e);
    }
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

- Verdict: idiom. A foreign key is an `Option<i64>`, and the tables are `BTreeMap` values so that they iterate in key order. The insert methods return `Result`, and `?` passes the error of the constraint to the caller.
- `save_tracks` takes `&mut Album` so that it can set the keys of the new tracks.
