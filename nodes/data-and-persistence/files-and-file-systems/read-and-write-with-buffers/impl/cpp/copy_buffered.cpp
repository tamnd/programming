// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.
// since: C++23 (std::expected)

#include "copy_buffered.hpp"

#include <fstream>
#include <ios>
#include <system_error>
#include <vector>

namespace {

// A file that is not there gives not_found. For dst, a missing parent
// directory gives not_found too; a missing dst is no error.
bool missing(const std::filesystem::path& path) {
    std::error_code ec;
    return std::filesystem::status(path, ec).type() == std::filesystem::file_type::not_found;
}

}  // namespace

std::expected<std::uintmax_t, CopyError> copy_buffered(const std::filesystem::path& src,
                                                       const std::filesystem::path& dst,
                                                       std::streamsize size) {
    if (size < 1) return std::unexpected(CopyError::invalid_size);
    // The buffers are made before the filebufs, so they live longer.
    std::vector<char> in_buf(size), out_buf(size), chunk(size);
    std::filebuf in, out;
    // pubsetbuf has an effect only before the file is open.
    in.pubsetbuf(in_buf.data(), size);
    if (!in.open(src, std::ios::in | std::ios::binary)) {
        return std::unexpected(missing(src) ? CopyError::not_found : CopyError::cannot_open);
    }
    out.pubsetbuf(out_buf.data(), size);
    if (!out.open(dst, std::ios::out | std::ios::trunc | std::ios::binary)) {
        auto dir = dst.has_parent_path() ? dst.parent_path() : std::filesystem::path(".");
        return std::unexpected(missing(dir) ? CopyError::not_found : CopyError::cannot_open);
    }
    std::uintmax_t total = 0;
    for (;;) {
        // A filebuf gives a short count at the end of the file and also on a
        // read error; it does not tell the two apart.
        std::streamsize n = in.sgetn(chunk.data(), size);
        if (n <= 0) break;
        if (out.sputn(chunk.data(), n) != n) return std::unexpected(CopyError::write_failed);
        total += static_cast<std::uintmax_t>(n);
    }
    // close writes the bytes that are still in the buffer; it gives a null
    // pointer if that write or the close fails.
    if (out.close() == nullptr) return std::unexpected(CopyError::write_failed);
    return total;
}
