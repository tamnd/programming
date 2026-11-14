```go run id=foreign-keys-and-collections
package main

import (
	"errors"
	"fmt"
	"sort"
	"strings"
)

type artistRow struct{ name string }
type albumRow struct {
	title    string
	artistID *int64
}
type trackRow struct {
	title   string
	albumID *int64
}

// A stand-in for the database: three tables, each keyed by id, and one row counter per table.
var (
	artistTable                      = map[int64]artistRow{}
	albumTable                       = map[int64]albumRow{}
	trackTable                       = map[int64]trackRow{}
	lastArtist, lastAlbum, lastTrack int64
)

var errFK = errors.New("FOREIGN KEY constraint failed")

func insertArtistRow(r artistRow) int64 {
	lastArtist++
	artistTable[lastArtist] = r
	return lastArtist
}

func insertAlbumRow(r albumRow) (int64, error) {
	// Like a FOREIGN KEY constraint: a key that is not nil must find its parent row.
	if r.artistID != nil {
		if _, ok := artistTable[*r.artistID]; !ok {
			return 0, errFK
		}
	}
	lastAlbum++
	albumTable[lastAlbum] = r
	return lastAlbum, nil
}

func insertTrackRow(r trackRow) (int64, error) {
	if r.albumID != nil {
		if _, ok := albumTable[*r.albumID]; !ok {
			return 0, errFK
		}
	}
	lastTrack++
	trackTable[lastTrack] = r
	return lastTrack, nil
}

type Artist struct {
	ID   *int64
	Name string
}

type Track struct {
	ID    *int64
	Title string
}

type Album struct {
	ID     *int64
	Title  string
	Artist *Artist
	Tracks []*Track
}

func sortedKeys[V any](m map[int64]V) []int64 {
	keys := make([]int64, 0, len(m))
	for k := range m {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	return keys
}

func insertAlbum(a *Album) error {
	// Many-to-one: write the key of the referenced artist.
	id, err := insertAlbumRow(albumRow{a.Title, a.Artist.ID})
	if err != nil {
		return err
	}
	a.ID = &id
	_, _, err = saveTracks(a)
	return err
}

func saveTracks(a *Album) (deleted, inserted int, err error) {
	// One-to-many: find the difference between the collection and the rows.
	var inDB []int64
	for _, id := range sortedKeys(trackTable) {
		if r := trackTable[id]; r.albumID != nil && *r.albumID == *a.ID {
			inDB = append(inDB, id)
		}
	}
	inMemory := map[int64]bool{}
	for _, t := range a.Tracks {
		if t.ID != nil {
			inMemory[*t.ID] = true
		}
	}
	for _, t := range a.Tracks {
		if t.ID == nil {
			id, err := insertTrackRow(trackRow{t.Title, a.ID})
			if err != nil {
				return deleted, inserted, err
			}
			t.ID = &id
			inserted++
		}
	}
	for _, id := range inDB {
		if !inMemory[id] {
			delete(trackTable, id)
			deleted++
		}
	}
	return deleted, inserted, nil
}

func loadAlbum(id int64) *Album {
	r := albumTable[id]
	artist := &Artist{ID: r.artistID, Name: artistTable[*r.artistID].name}
	var tracks []*Track
	for _, tid := range sortedKeys(trackTable) {
		if t := trackTable[tid]; t.albumID != nil && *t.albumID == id {
			tid := tid
			tracks = append(tracks, &Track{ID: &tid, Title: t.title})
		}
	}
	return &Album{ID: &id, Title: r.title, Artist: artist, Tracks: tracks}
}

func showTracks() string {
	var parts []string
	for _, id := range sortedKeys(trackTable) {
		r := trackTable[id]
		parts = append(parts, fmt.Sprintf("%d %s album_id=%d", id, r.title, *r.albumID))
	}
	return strings.Join(parts, " | ")
}

func main() {
	miles := &Artist{Name: "Miles Davis"}
	id := insertArtistRow(artistRow{miles.Name})
	miles.ID = &id
	if err := insertAlbum(&Album{Title: "Kind of Blue", Artist: miles, Tracks: []*Track{{Title: "So What"}, {Title: "Blue in Green"}}}); err != nil {
		fmt.Println(err)
	}
	for _, id := range sortedKeys(albumTable) {
		r := albumTable[id]
		fmt.Printf("album rows: %d %s artist_id=%d\n", id, r.title, *r.artistID)
	}
	fmt.Println("track rows:", showTracks())

	album := loadAlbum(1)
	var titles []string
	for _, t := range album.Tracks {
		titles = append(titles, t.Title)
	}
	fmt.Printf("loaded: %s by %s | %s\n", album.Title, album.Artist.Name, strings.Join(titles, ", "))

	var kept []*Track
	for _, t := range album.Tracks {
		if t.Title != "Blue in Green" {
			kept = append(kept, t)
		}
	}
	album.Tracks = append(kept, &Track{Title: "Freddie Freeloader"})
	deleted, inserted, err := saveTracks(album)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("update tracks: deleted %d, inserted %d\n", deleted, inserted)
	fmt.Println("track rows:", showTracks())

	bad := int64(99)
	if _, err := insertAlbumRow(albumRow{"Unknown", &bad}); err != nil {
		fmt.Println("artist_id 99:", err)
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

- Verdict: idiom. A foreign key that can be null is a `*int64`. Go maps have no order, so `sortedKeys` sorts the keys before each scan of a table.
- The insert functions return an `error` for a key that has no parent row, as a database driver returns the error of the constraint.
