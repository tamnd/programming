```go run id=owner-maps-children
package main

import (
	"fmt"
	"sort"
	"strings"
)

type trackKey struct {
	albumID int64
	seq     int
}

// A stand-in for the database: an album table, and a track table keyed by (album_id, seq).
var (
	albumTable = map[int64]string{}
	trackTable = map[trackKey]string{}
	lastAlbum  int64
)

// Track is a dependent: no key and no mapper of its own.
type Track struct{ Title string }

type Album struct {
	ID     int64
	Title  string
	Tracks []Track
}

// AlbumMapper is the owner's mapper: it loads, saves and deletes the tracks.
type AlbumMapper struct{}

func (m AlbumMapper) Insert(a *Album) int {
	lastAlbum++
	albumTable[lastAlbum] = a.Title
	a.ID = lastAlbum
	return m.insertTracks(a)
}

func (m AlbumMapper) Update(a *Album) (deleted, inserted int) {
	albumTable[a.ID] = a.Title
	deleted = m.deleteTracks(a.ID)
	return deleted, m.insertTracks(a)
}

func (m AlbumMapper) Delete(a *Album) {
	m.deleteTracks(a.ID)
	delete(albumTable, a.ID)
}

func (m AlbumMapper) insertTracks(a *Album) int {
	for i, t := range a.Tracks {
		trackTable[trackKey{a.ID, i + 1}] = t.Title
	}
	return len(a.Tracks)
}

func (m AlbumMapper) deleteTracks(albumID int64) int {
	n := 0
	for k := range trackTable {
		if k.albumID == albumID {
			delete(trackTable, k)
			n++
		}
	}
	return n
}

func (m AlbumMapper) Find(id int64) *Album {
	var tracks []Track
	for _, k := range trackKeys() {
		if k.albumID == id {
			tracks = append(tracks, Track{trackTable[k]})
		}
	}
	return &Album{id, albumTable[id], tracks}
}

func trackKeys() []trackKey {
	keys := make([]trackKey, 0, len(trackTable))
	for k := range trackTable {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool {
		if keys[i].albumID != keys[j].albumID {
			return keys[i].albumID < keys[j].albumID
		}
		return keys[i].seq < keys[j].seq
	})
	return keys
}

func showTracks() string {
	var parts []string
	for _, k := range trackKeys() {
		parts = append(parts, fmt.Sprintf("%d/%d %s", k.albumID, k.seq, trackTable[k]))
	}
	return strings.Join(parts, " | ")
}

func main() {
	mapper := AlbumMapper{}
	mapper.Insert(&Album{Title: "Kind of Blue", Tracks: []Track{{"So What"}, {"Blue in Green"}}})
	fmt.Println("tracks:", showTracks())

	album := mapper.Find(1)
	var titles []string
	for _, t := range album.Tracks {
		titles = append(titles, t.Title)
	}
	fmt.Printf("loaded: %s | %s\n", album.Title, strings.Join(titles, ", "))

	album.Tracks = append(album.Tracks[:1], append([]Track{{"Freddie Freeloader"}}, album.Tracks[1:]...)...)
	deleted, inserted := mapper.Update(album)
	fmt.Printf("update: deleted %d, inserted %d\n", deleted, inserted)
	fmt.Println("tracks:", showTracks())

	mapper.Delete(album)
	fmt.Printf("delete album 1: albums %d, tracks %d\n", len(albumTable), len(trackTable))
}
```

```output
tracks: 1/1 So What | 1/2 Blue in Green
loaded: Kind of Blue | So What, Blue in Green
update: deleted 2, inserted 3
tracks: 1/1 So What | 1/2 Freddie Freeloader | 1/3 Blue in Green
delete album 1: albums 0, tracks 0
```

- Verdict: idiom. The key of a track row is a `trackKey` struct of the album key and the position. Go maps have no order, so `trackKeys` sorts the keys before the program prints the rows.
- `Update` returns the counts of the deleted and inserted rows, and `Delete` removes the tracks before the album.
