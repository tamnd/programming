```rust run id=owner-maps-children
use std::collections::BTreeMap;

// A dependent: no key and no mapper of its own.
struct Track {
    title: String,
}

struct Album {
    id: Option<i64>,
    title: String,
    tracks: Vec<Track>,
}

fn track(title: &str) -> Track {
    Track { title: title.to_string() }
}

// A stand-in for the database: an album table, and a track table keyed by (album_id, seq).
#[derive(Default)]
struct Db {
    album: BTreeMap<i64, String>,
    track: BTreeMap<(i64, usize), String>,
    last_album: i64,
}

// The owner's mapper loads, saves and deletes the tracks.
struct AlbumMapper;

impl AlbumMapper {
    fn insert(&self, db: &mut Db, album: &mut Album) -> usize {
        db.last_album += 1;
        db.album.insert(db.last_album, album.title.clone());
        album.id = Some(db.last_album);
        self.insert_tracks(db, db.last_album, album)
    }

    fn update(&self, db: &mut Db, album: &Album) -> (usize, usize) {
        let id = album.id.expect("the album is saved");
        db.album.insert(id, album.title.clone());
        let deleted = self.delete_tracks(db, id);
        (deleted, self.insert_tracks(db, id, album))
    }

    fn delete(&self, db: &mut Db, album: &Album) {
        if let Some(id) = album.id {
            self.delete_tracks(db, id);
            db.album.remove(&id);
        }
    }

    fn insert_tracks(&self, db: &mut Db, id: i64, album: &Album) -> usize {
        for (i, t) in album.tracks.iter().enumerate() {
            db.track.insert((id, i + 1), t.title.clone());
        }
        album.tracks.len()
    }

    fn delete_tracks(&self, db: &mut Db, id: i64) -> usize {
        let before = db.track.len();
        db.track.retain(|(album_id, _), _| *album_id != id);
        before - db.track.len()
    }

    fn find(&self, db: &Db, id: i64) -> Album {
        let tracks = db.track.range((id, 0)..(id + 1, 0)).map(|(_, t)| track(t)).collect();
        Album { id: Some(id), title: db.album[&id].clone(), tracks }
    }
}

fn show_tracks(db: &Db) -> String {
    let parts: Vec<String> = db.track.iter().map(|((a, s), t)| format!("{}/{} {}", a, s, t)).collect();
    parts.join(" | ")
}

fn main() {
    let mut db = Db::default();
    let mapper = AlbumMapper;
    let mut kind_of_blue = Album {
        id: None,
        title: "Kind of Blue".to_string(),
        tracks: vec![track("So What"), track("Blue in Green")],
    };
    mapper.insert(&mut db, &mut kind_of_blue);
    println!("tracks: {}", show_tracks(&db));

    let mut album = mapper.find(&db, 1);
    let titles: Vec<&str> = album.tracks.iter().map(|t| t.title.as_str()).collect();
    println!("loaded: {} | {}", album.title, titles.join(", "));

    album.tracks.insert(1, track("Freddie Freeloader"));
    let (deleted, inserted) = mapper.update(&mut db, &album);
    println!("update: deleted {}, inserted {}", deleted, inserted);
    println!("tracks: {}", show_tracks(&db));

    mapper.delete(&mut db, &album);
    println!("delete album 1: albums {}, tracks {}", db.album.len(), db.track.len());
}
```

```output
tracks: 1/1 So What | 1/2 Blue in Green
loaded: Kind of Blue | So What, Blue in Green
update: deleted 2, inserted 3
tracks: 1/1 So What | 1/2 Freddie Freeloader | 1/3 Blue in Green
delete album 1: albums 0, tracks 0
```

- Verdict: idiom. The track table is a `BTreeMap<(i64, usize), String>`, so the rows iterate in album and position order.
- `delete_tracks` uses `retain` to remove the rows of one album and returns the number of removed rows.
