// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <memory>
#include <string>
#include <vector>

namespace playlist {

// The songs field is a shared pointer, so the implicit copy constructor copies the pointer and
// the copy shares the list. A plain std::vector field would be copied element by element.
struct Playlist {
    std::string name;
    std::shared_ptr<std::vector<std::string>> songs;

    // A deep copy: a new vector with the same songs.
    Playlist deep_copy() const;
};

// Returns the songs of the original, of a shallow copy and of a deep copy after the original
// sets the song at index to title; throws std::out_of_range for a bad index.
std::vector<std::vector<std::string>> copy_and_change(const std::vector<std::string>& songs, std::size_t index,
                                                      const std::string& title);

}  // namespace playlist

#endif
