// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdbool.h>
#include <stddef.h>

// A playlist owns an array of count strings.
typedef struct {
    char **songs;
    size_t count;
} Playlist;

// playlist_make copies each string into a new playlist; it returns false when memory runs out.
bool playlist_make(const char *const *songs, size_t count, Playlist *out);

// playlist_shallow_copy copies the struct: the copy shares the array and the strings.
// Do not free a shallow copy; free the original once.
Playlist playlist_shallow_copy(const Playlist *p);

// playlist_deep_copy makes a new array and new strings; free it with playlist_free.
bool playlist_deep_copy(const Playlist *p, Playlist *out);

// playlist_set replaces the song at index with a copy of title.
bool playlist_set(Playlist *p, size_t index, const char *title);

void playlist_free(Playlist *p);

// copy_and_change fills results with the original, a shallow copy and a deep copy after the
// original sets the song at index to title. Free results[0] and results[2] with playlist_free.
bool copy_and_change(const char *const *songs, size_t count, size_t index, const char *title, Playlist results[3]);

#endif
