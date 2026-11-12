```java run id=remove-the-middle
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Song(String title, int seconds) {}

    // Before: four methods forward to the list, and one has a rule.
    static final class PlaylistBefore {
        private final List<Song> songs = new ArrayList<>();
        void add(Song s) { songs.add(s); }
        int size() { return songs.size(); }
        Song get(int i) { return songs.get(i); }
        boolean contains(Song s) { return songs.contains(s); }
        int totalSeconds() { return songs.stream().mapToInt(Song::seconds).sum(); }
    }

    // After: the clients use the list; the rule is a record around an unmodifiable copy.
    record Playlist(List<Song> songs) {
        Playlist {
            songs = List.copyOf(songs);
        }

        int totalSeconds() { return songs.stream().mapToInt(Song::seconds).sum(); }
    }

    public static void main(String[] args) {
        List<Song> songs = List.of(new Song("Intro", 90), new Song("Theme", 200), new Song("Outro", 60));
        PlaylistBefore before = new PlaylistBefore();
        songs.forEach(before::add);
        Playlist after = new Playlist(songs);

        if (before.size() != after.songs().size()) throw new AssertionError("size");
        if (!before.get(2).equals(after.songs().get(2))) throw new AssertionError("get");
        if (before.contains(songs.get(1)) != after.songs().contains(songs.get(1))) throw new AssertionError("contains");
        if (before.totalSeconds() != after.totalSeconds() || after.totalSeconds() != 350) throw new AssertionError("total");
        System.out.println("playlist: tests pass");
    }
}
```

```output
playlist: tests pass
```

- `after.songs()` gives the clients the whole `List` interface. `List.copyOf` makes the list unmodifiable, so the clients can read it but cannot change it behind the record's back.
- The record keeps `totalSeconds`, the one method that is a rule and not a forward.
