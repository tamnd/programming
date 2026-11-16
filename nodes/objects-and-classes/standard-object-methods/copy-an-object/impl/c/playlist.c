// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#include "playlist.h"

#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *s) {
    size_t n = strlen(s) + 1;
    char *c = malloc(n);
    if (c != NULL) {
        memcpy(c, s, n);
    }
    return c;
}

bool playlist_make(const char *const *songs, size_t count, Playlist *out) {
    out->count = count;
    out->songs = calloc(count > 0 ? count : 1, sizeof *out->songs);
    if (out->songs == NULL) {
        return false;
    }
    for (size_t i = 0; i < count; i++) {
        out->songs[i] = copy_string(songs[i]);
        if (out->songs[i] == NULL) {
            playlist_free(out);
            return false;
        }
    }
    return true;
}

Playlist playlist_shallow_copy(const Playlist *p) { return *p; }

bool playlist_deep_copy(const Playlist *p, Playlist *out) {
    return playlist_make((const char *const *)p->songs, p->count, out);
}

bool playlist_set(Playlist *p, size_t index, const char *title) {
    if (index >= p->count) {
        return false;
    }
    char *c = copy_string(title);
    if (c == NULL) {
        return false;
    }
    free(p->songs[index]);
    p->songs[index] = c;
    return true;
}

void playlist_free(Playlist *p) {
    if (p->songs != NULL) {
        for (size_t i = 0; i < p->count; i++) {
            free(p->songs[i]);
        }
        free(p->songs);
    }
    p->songs = NULL;
    p->count = 0;
}

bool copy_and_change(const char *const *songs, size_t count, size_t index, const char *title, Playlist results[3]) {
    if (index >= count || !playlist_make(songs, count, &results[0])) {
        return false;
    }
    results[1] = playlist_shallow_copy(&results[0]);
    if (!playlist_deep_copy(&results[0], &results[2])) {
        playlist_free(&results[0]);
        return false;
    }
    if (!playlist_set(&results[0], index, title)) {
        playlist_free(&results[0]);
        playlist_free(&results[2]);
        return false;
    }
    return true;
}
