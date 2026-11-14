```java run id=foreign-keys-and-collections
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class Main {
    record ArtistRow(String name) {}
    record AlbumRow(String title, Long artistId) {}
    record TrackRow(String title, Long albumId) {}

    // A stand-in for the database: three tables, each keyed by id, and one row counter per table.
    static final Map<Long, ArtistRow> ARTIST = new TreeMap<>();
    static final Map<Long, AlbumRow> ALBUM = new TreeMap<>();
    static final Map<Long, TrackRow> TRACK = new TreeMap<>();
    static long lastArtist = 0, lastAlbum = 0, lastTrack = 0;

    static long insertArtistRow(ArtistRow row) {
        ARTIST.put(++lastArtist, row);
        return lastArtist;
    }

    static long insertAlbumRow(AlbumRow row) {
        // Like a FOREIGN KEY constraint: a key that is not null must find its parent row.
        if (row.artistId() != null && !ARTIST.containsKey(row.artistId())) {
            throw new IllegalStateException("FOREIGN KEY constraint failed");
        }
        ALBUM.put(++lastAlbum, row);
        return lastAlbum;
    }

    static long insertTrackRow(TrackRow row) {
        if (row.albumId() != null && !ALBUM.containsKey(row.albumId())) {
            throw new IllegalStateException("FOREIGN KEY constraint failed");
        }
        TRACK.put(++lastTrack, row);
        return lastTrack;
    }

    static class Artist {
        Long id;
        String name;

        Artist(String name, Long id) {
            this.name = name;
            this.id = id;
        }
    }

    static class Track {
        Long id;
        String title;

        Track(String title, Long id) {
            this.title = title;
            this.id = id;
        }
    }

    static class Album {
        Long id;
        String title;
        Artist artist;
        List<Track> tracks;

        Album(String title, Artist artist, List<Track> tracks, Long id) {
            this.title = title;
            this.artist = artist;
            this.tracks = tracks;
            this.id = id;
        }
    }

    static void insertAlbum(Album album) {
        // Many-to-one: write the key of the referenced artist.
        album.id = insertAlbumRow(new AlbumRow(album.title, album.artist.id));
        saveTracks(album);
    }

    static int[] saveTracks(Album album) {
        // One-to-many: find the difference between the collection and the rows.
        List<Long> inDb = new ArrayList<>();
        TRACK.forEach((id, r) -> {
            if (album.id.equals(r.albumId())) {
                inDb.add(id);
            }
        });
        Set<Long> inMemory = new HashSet<>();
        for (Track t : album.tracks) {
            if (t.id != null) {
                inMemory.add(t.id);
            }
        }
        int inserted = 0;
        for (Track t : album.tracks) {
            if (t.id == null) {
                t.id = insertTrackRow(new TrackRow(t.title, album.id));
                inserted++;
            }
        }
        int deleted = 0;
        for (Long id : inDb) {
            if (!inMemory.contains(id)) {
                TRACK.remove(id);
                deleted++;
            }
        }
        return new int[] {deleted, inserted};
    }

    static Album loadAlbum(long id) {
        AlbumRow row = ALBUM.get(id);
        Artist artist = new Artist(ARTIST.get(row.artistId()).name(), row.artistId());
        List<Track> tracks = new ArrayList<>();
        TRACK.forEach((tid, r) -> {
            if (r.albumId() == id) {
                tracks.add(new Track(r.title(), tid));
            }
        });
        return new Album(row.title(), artist, tracks, id);
    }

    static String showTracks() {
        List<String> parts = new ArrayList<>();
        TRACK.forEach((id, r) -> parts.add(id + " " + r.title() + " album_id=" + r.albumId()));
        return String.join(" | ", parts);
    }

    public static void main(String[] args) {
        Artist miles = new Artist("Miles Davis", null);
        miles.id = insertArtistRow(new ArtistRow(miles.name));
        List<Track> tracks = new ArrayList<>(List.of(new Track("So What", null), new Track("Blue in Green", null)));
        insertAlbum(new Album("Kind of Blue", miles, tracks, null));
        ALBUM.forEach((id, r) -> System.out.println("album rows: " + id + " " + r.title() + " artist_id=" + r.artistId()));
        System.out.println("track rows: " + showTracks());

        Album album = loadAlbum(1);
        List<String> titles = new ArrayList<>();
        for (Track t : album.tracks) {
            titles.add(t.title);
        }
        System.out.println("loaded: " + album.title + " by " + album.artist.name + " | " + String.join(", ", titles));

        album.tracks.removeIf(t -> t.title.equals("Blue in Green"));
        album.tracks.add(new Track("Freddie Freeloader", null));
        int[] counts = saveTracks(album);
        System.out.println("update tracks: deleted " + counts[0] + ", inserted " + counts[1]);
        System.out.println("track rows: " + showTracks());

        try {
            insertAlbumRow(new AlbumRow("Unknown", 99L));
        } catch (IllegalStateException e) {
            System.out.println("artist_id 99: " + e.getMessage());
        }
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

- Verdict: idiom. The rows are records, and the tables are `TreeMap` objects so that they iterate in key order. A key is a `Long`, so `null` can stand for a missing reference.
- With JPA or Hibernate, `@ManyToOne` and `@JoinColumn` declare the same mapping, and `@OneToMany(mappedBy = ...)` declares the collection.
