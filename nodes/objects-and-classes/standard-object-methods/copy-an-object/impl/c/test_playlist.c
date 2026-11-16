// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#include "playlist.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

// want holds 3 * count strings: the songs of the original, the shallow copy and the deep copy.
static void check(const char *name, const char *const *songs, size_t count, size_t index, const char *title,
                  const char *const *want) {
    Playlist r[3];
    if (!copy_and_change(songs, count, index, title, r)) {
        fprintf(stderr, "case %s failed: error\n", name);
        failures++;
        return;
    }
    for (size_t k = 0; k < 3; k++) {
        for (size_t i = 0; i < count; i++) {
            if (r[k].count != count || strcmp(r[k].songs[i], want[k * count + i]) != 0) {
                fprintf(stderr, "case %s failed at list %zu item %zu\n", name, k, i);
                failures++;
                k = 3;
                break;
            }
        }
    }
    playlist_free(&r[0]);
    playlist_free(&r[2]);
}

static void expect(const char *name, bool ok) {
    if (!ok) {
        fprintf(stderr, "check %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("middle", (const char *[]){"Intro", "Song", "Outro"}, 3, 1, "Hit",
          (const char *[]){"Intro", "Hit", "Outro", "Intro", "Hit", "Outro", "Intro", "Song", "Outro"});
    check("two", (const char *[]){"A", "B"}, 2, 0, "C", (const char *[]){"C", "B", "C", "B", "A", "B"});
    check("single", (const char *[]){"Solo"}, 1, 0, "Duet", (const char *[]){"Duet", "Duet", "Solo"});
    check("first", (const char *[]){"a", "b", "c"}, 3, 0, "z",
          (const char *[]){"z", "b", "c", "z", "b", "c", "a", "b", "c"});
    check("last", (const char *[]){"a", "b", "c"}, 3, 2, "z",
          (const char *[]){"a", "b", "z", "a", "b", "z", "a", "b", "c"});
    check("same-title", (const char *[]){"a", "b"}, 2, 1, "b", (const char *[]){"a", "b", "a", "b", "a", "b"});
    check("duplicates", (const char *[]){"x", "x", "x"}, 3, 1, "y",
          (const char *[]){"x", "y", "x", "x", "y", "x", "x", "x", "x"});

    Playlist original;
    expect("make", playlist_make((const char *[]){"a"}, 1, &original));
    Playlist shallow = playlist_shallow_copy(&original);
    Playlist deep;
    expect("deep", playlist_deep_copy(&original, &deep));
    expect("shared array", shallow.songs == original.songs && deep.songs != original.songs);
    expect("own strings", deep.songs[0] != original.songs[0] && strcmp(deep.songs[0], "a") == 0);
    playlist_free(&original);
    playlist_free(&deep);

    Playlist r[3];
    expect("bad index", !copy_and_change((const char *[]){"a"}, 1, 1, "b", r));

    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
