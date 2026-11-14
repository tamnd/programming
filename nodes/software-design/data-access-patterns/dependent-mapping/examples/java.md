```java run id=owner-maps-children
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    record TrackKey(long albumId, int seq) implements Comparable<TrackKey> {
        public int compareTo(TrackKey o) {
            int c = Long.compare(albumId, o.albumId);
            return c != 0 ? c : Integer.compare(seq, o.seq);
        }
    }

    // A stand-in for the database: an album table, and a track table keyed by (album_id, seq).
    static final Map<Long, String> ALBUM = new TreeMap<>();
    static final TreeMap<TrackKey, String> TRACK = new TreeMap<>();
    static long lastAlbum = 0;

    // A dependent: no key and no mapper of its own.
    record Track(String title) {}

    static class Album {
        Long id;
        String title;
        List<Track> tracks;

        Album(String title, List<Track> tracks, Long id) {
            this.title = title;
            this.tracks = tracks;
            this.id = id;
        }
    }

    // The owner's mapper loads, saves and deletes the tracks.
    static class AlbumMapper {
        int insert(Album album) {
            ALBUM.put(++lastAlbum, album.title);
            album.id = lastAlbum;
            return insertTracks(album);
        }

        int[] update(Album album) {
            ALBUM.put(album.id, album.title);
            int deleted = deleteTracks(album.id);
            return new int[] {deleted, insertTracks(album)};
        }

        void delete(Album album) {
            deleteTracks(album.id);
            ALBUM.remove(album.id);
        }

        int insertTracks(Album album) {
            for (int i = 0; i < album.tracks.size(); i++) {
                TRACK.put(new TrackKey(album.id, i + 1), album.tracks.get(i).title());
            }
            return album.tracks.size();
        }

        int deleteTracks(long albumId) {
            Map<TrackKey, String> rows = TRACK.subMap(new TrackKey(albumId, 0), new TrackKey(albumId + 1, 0));
            int n = rows.size();
            rows.clear();
            return n;
        }

        Album find(long id) {
            List<Track> tracks = new ArrayList<>();
            TRACK.subMap(new TrackKey(id, 0), new TrackKey(id + 1, 0)).values().forEach(t -> tracks.add(new Track(t)));
            return new Album(ALBUM.get(id), tracks, id);
        }
    }

    static String showTracks() {
        List<String> parts = new ArrayList<>();
        TRACK.forEach((k, t) -> parts.add(k.albumId() + "/" + k.seq() + " " + t));
        return String.join(" | ", parts);
    }

    public static void main(String[] args) {
        AlbumMapper mapper = new AlbumMapper();
        mapper.insert(new Album("Kind of Blue", new ArrayList<>(List.of(new Track("So What"), new Track("Blue in Green"))), null));
        System.out.println("tracks: " + showTracks());

        Album album = mapper.find(1);
        List<String> titles = new ArrayList<>();
        album.tracks.forEach(t -> titles.add(t.title()));
        System.out.println("loaded: " + album.title + " | " + String.join(", ", titles));

        album.tracks.add(1, new Track("Freddie Freeloader"));
        int[] counts = mapper.update(album);
        System.out.println("update: deleted " + counts[0] + ", inserted " + counts[1]);
        System.out.println("tracks: " + showTracks());

        mapper.delete(album);
        System.out.println("delete album 1: albums " + ALBUM.size() + ", tracks " + TRACK.size());
    }
}
```

```output
tracks: 1/1 So What | 1/2 Blue in Green
loaded: Kind of Blue | So What, Blue in Green
update: deleted 2, inserted 3
tracks: 1/1 So What | 1/2 Freddie Freeloader | 1/3 Blue in Green
delete album 1: albums 0, tracks 0
```

- Verdict: idiom. The key of a track row is a `TrackKey` record that is `Comparable`, so the `TreeMap` keeps the rows in album and position order. `Track` is a record with only a title.
- With JPA or Hibernate, `@ElementCollection` with `@OrderColumn` maps the same list, and the provider can delete and insert the rows of the collection when it changes.
