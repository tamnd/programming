// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.
// since: C++23 (std::expected)

#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>

enum class CopyError { not_found, cannot_open, invalid_size, write_failed };

// Copies the file at src to the file at dst through std::filebuf buffers of
// size bytes, and returns the number of bytes copied. An error from the last
// write is CopyError::write_failed, from the close of dst.
std::expected<std::uintmax_t, CopyError> copy_buffered(const std::filesystem::path& src,
                                                       const std::filesystem::path& dst,
                                                       std::streamsize size);
