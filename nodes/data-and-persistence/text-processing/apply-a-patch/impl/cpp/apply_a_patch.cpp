// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

#include "apply_a_patch.hpp"

#include <charconv>
#include <fstream>
#include <iterator>
#include <optional>

std::vector<std::string_view> split_lines(std::string_view text) {
    std::vector<std::string_view> lines;
    while (!text.empty()) {
        std::size_t end = text.find('\n');
        end = end == std::string_view::npos ? text.size() : end + 1;
        lines.push_back(text.substr(0, end));
        text.remove_prefix(end);
    }
    return lines;
}

namespace {

struct Hunk {
    std::size_t old_start, old_count, new_count;
};

// Reads a number at the start of s. It fails if there is no digit or the
// number is too large.
bool number(std::string_view& s, std::size_t& v) {
    auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
    if (ec != std::errc{}) {
        return false;
    }
    s.remove_prefix(static_cast<std::size_t>(p - s.data()));
    return true;
}

bool prefix(std::string_view& s, std::string_view t) {
    if (!s.starts_with(t)) {
        return false;
    }
    s.remove_prefix(t.size());
    return true;
}

// Reads a start and an optional comma and count. A missing count is 1.
bool range(std::string_view& s, std::size_t& start, std::size_t& count) {
    count = 1;
    return number(s, start) && (!prefix(s, ",") || number(s, count));
}

// Reads a line such as @@ -1,3 +1,4 @@ and the text after it.
std::optional<Hunk> hunk_header(std::string_view s) {
    Hunk h{};
    std::size_t new_start = 0;
    if (prefix(s, "@@ -") && range(s, h.old_start, h.old_count) && prefix(s, " +") &&
        range(s, new_start, h.new_count) && prefix(s, " @@")) {
        return h;
    }
    return std::nullopt;
}

struct BodyLine {
    char mark;
    std::string_view text;
    std::size_t number;
    bool lf;
};

std::unexpected<PatchError> error(PatchErrorKind kind, std::size_t line) {
    return std::unexpected(PatchError{kind, line});
}

}  // namespace

std::expected<std::string, PatchError> apply_patch(std::string_view old_text, std::string_view patch) {
    using enum PatchErrorKind;
    const auto lines = split_lines(old_text);
    // The lines of the patch, with no LF. A CR stays in its line.
    auto p = split_lines(patch);
    for (auto& l : p) {
        if (l.ends_with('\n')) {
            l.remove_suffix(1);
        }
    }
    std::string out;
    std::size_t cur = 0;  // the index of the next line of old_text to use
    std::size_t hunks = 0;
    std::size_t i = 0;
    while (i < p.size()) {
        if (!p[i].starts_with("@@")) {
            i++;  // a header line, or another line outside a hunk
            continue;
        }
        auto h = hunk_header(p[i]);
        if (!h || (h->old_count > 0 && h->old_start == 0)) {
            return error(malformed, i + 1);
        }
        hunks++;
        // An empty range starts after the line old_start.
        std::size_t pos = h->old_count > 0 ? h->old_start - 1 : h->old_start;
        if (pos < cur || pos > lines.size()) {
            return error(mismatch, i + 1);
        }
        for (; cur < pos; cur++) {
            out += lines[cur];
        }
        i++;
        std::vector<BodyLine> body;
        std::size_t old_left = h->old_count, new_left = h->new_count;
        while (old_left > 0 || new_left > 0) {
            if (i == p.size()) {
                return error(malformed, i);
            }
            std::string_view line = p[i];
            char mark = line.empty() ? ' ' : line[0];
            if (mark == '\\') {
                if (body.empty()) {
                    return error(malformed, i + 1);
                }
                body.back().lf = false;  // the line before has no LF
                i++;
                continue;
            }
            if (mark != ' ' && mark != '-' && mark != '+') {
                return error(malformed, i + 1);
            }
            if ((mark != '+' && old_left-- == 0) || (mark != '-' && new_left-- == 0)) {
                return error(malformed, i + 1);
            }
            body.push_back({mark, line.empty() ? line : line.substr(1), i + 1, true});
            i++;
        }
        if (i < p.size() && p[i].starts_with('\\') && !body.empty()) {
            body.back().lf = false;
            i++;
        }
        for (const auto& b : body) {
            if (b.mark != '+') {
                std::string_view want = cur < lines.size() ? lines[cur] : std::string_view{};
                bool same = cur < lines.size() && want.size() == b.text.size() + (b.lf ? 1 : 0) &&
                            want.starts_with(b.text) && (!b.lf || want.ends_with('\n'));
                if (!same) {
                    return error(mismatch, b.number);
                }
                cur++;
            }
            if (b.mark != '-') {
                out += b.text;
                if (b.lf) {
                    out += '\n';
                }
            }
        }
    }
    if (hunks == 0) {
        return error(malformed, p.size());
    }
    for (; cur < lines.size(); cur++) {
        out += lines[cur];
    }
    return out;
}

namespace {

// Reads the whole file in binary mode, so that each CR stays.
std::expected<std::string, PatchError> read_file(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(PatchError{PatchErrorKind::cannot_open, 0});
    }
    std::string data{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    if (in.bad()) {
        return std::unexpected(PatchError{PatchErrorKind::read_failed, 0});
    }
    return data;
}

}  // namespace

std::expected<std::string, PatchError> apply_patch_file(const std::filesystem::path& path,
                                                        const std::filesystem::path& patch_path) {
    auto old_text = read_file(path);
    if (!old_text) {
        return std::unexpected(old_text.error());
    }
    auto patch = read_file(patch_path);
    if (!patch) {
        return std::unexpected(patch.error());
    }
    return apply_patch(*old_text, *patch);
}
