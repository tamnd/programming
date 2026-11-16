// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

#include "playlist.hpp"

#include <stdexcept>

namespace playlist {

Playlist Playlist::deep_copy() const { return Playlist{name, std::make_shared<std::vector<std::string>>(*songs)}; }

std::vector<std::vector<std::string>> copy_and_change(const std::vector<std::string>& songs, std::size_t index,
                                                      const std::string& title) {
    if (index >= songs.size()) {
        throw std::out_of_range("no song at this index");
    }
    Playlist original{"mix", std::make_shared<std::vector<std::string>>(songs)};
    Playlist shallow = original;  // the implicit copy constructor: a shallow copy
    Playlist deep = original.deep_copy();
    (*original.songs)[index] = title;
    return {*original.songs, *shallow.songs, *deep.songs};
}

}  // namespace playlist
