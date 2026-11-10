// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.
// since: C++23 (std::expected)

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>

// A SHA-256 computation that takes the message in parts (FIPS 180-4).
class Sha256 {
public:
    using Digest = std::array<std::uint8_t, 32>;

    Sha256();
    // Adds the bytes of data to the message.
    void update(std::span<const std::byte> data);
    void update(std::string_view data) { update(std::as_bytes(std::span(data))); }
    // Pads the message and returns the digest. Call it once.
    Digest finalize();

private:
    std::array<std::uint32_t, 8> state_;
    std::array<std::uint8_t, 64> block_{};
    std::size_t used_ = 0;
    std::uint64_t len_ = 0;  // the length of the message in bytes
};

// Returns the digest as 64 lowercase hexadecimal digits.
std::string to_hex(const Sha256::Digest& digest);

// Returns the SHA-256 of the bytes of text, as lowercase hex. For text, pass
// its UTF-8 bytes.
std::string sha256_text(std::string_view text);

enum class HashError { cannot_open, read_failed };

// Returns the SHA-256 of the bytes of the file at path, as lowercase hex.
std::expected<std::string, HashError> sha256_file(const std::filesystem::path& path);
