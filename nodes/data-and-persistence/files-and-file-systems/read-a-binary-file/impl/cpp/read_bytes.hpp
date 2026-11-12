// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.
// since: C++23 (std::expected)

#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <vector>

enum class ReadError { not_found, not_a_file, cannot_open, read_failed, unexpected_eof };

// Reads all of the bytes of the file at path, with no change to them.
std::expected<std::vector<std::byte>, ReadError> read_bytes(const std::filesystem::path& path);

// Reads the first n bytes of the file at path. If the file has fewer than n
// bytes, the error is ReadError::unexpected_eof.
std::expected<std::vector<std::byte>, ReadError> read_exact(const std::filesystem::path& path,
                                                            std::size_t n);
