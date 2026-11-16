// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#include "playlist.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using playlist::Playlist;
using Lists = std::vector<std::vector<std::string>>;

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string>& songs, std::size_t index,
                  const std::string& title, const Lists& want) {
    if (playlist::copy_and_change(songs, index, title) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

static void expect(std::string_view name, bool ok) {
    if (!ok) {
        std::cerr << "check " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("middle", {"Intro", "Song", "Outro"}, 1, "Hit",
          {{"Intro", "Hit", "Outro"}, {"Intro", "Hit", "Outro"}, {"Intro", "Song", "Outro"}});
    check("two", {"A", "B"}, 0, "C", {{"C", "B"}, {"C", "B"}, {"A", "B"}});
    check("single", {"Solo"}, 0, "Duet", {{"Duet"}, {"Duet"}, {"Solo"}});
    check("first", {"a", "b", "c"}, 0, "z", {{"z", "b", "c"}, {"z", "b", "c"}, {"a", "b", "c"}});
    check("last", {"a", "b", "c"}, 2, "z", {{"a", "b", "z"}, {"a", "b", "z"}, {"a", "b", "c"}});
    check("same-title", {"a", "b"}, 1, "b", {{"a", "b"}, {"a", "b"}, {"a", "b"}});
    check("duplicates", {"x", "x", "x"}, 1, "y", {{"x", "y", "x"}, {"x", "y", "x"}, {"x", "x", "x"}});

    Playlist original{"mix", std::make_shared<std::vector<std::string>>(std::vector<std::string>{"a"})};
    Playlist shallow = original;
    Playlist deep = original.deep_copy();
    expect("shared", shallow.songs == original.songs && deep.songs != original.songs);
    shallow.name = "other";
    expect("own name", original.name == "mix");

    bool threw = false;
    try {
        playlist::copy_and_change({"a"}, 1, "b");
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect("bad index", threw);

    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
