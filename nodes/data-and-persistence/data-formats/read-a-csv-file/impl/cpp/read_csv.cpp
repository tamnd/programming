// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

// The C++ standard library has no CSV reader, so this file has a small parser of RFC 4180.

#include "read_csv.hpp"

#include <fstream>
#include <iterator>

namespace {

// Returns the length of the line break at i: 1 for LF, 2 for CRLF, 0 for none.
std::size_t line_break(std::string_view s, std::size_t i) {
    if (i < s.size() && s[i] == '\n') {
        return 1;
    }
    return i + 1 < s.size() && s[i] == '\r' && s[i + 1] == '\n' ? 2 : 0;
}

}  // namespace

std::vector<std::vector<std::string>> parse_csv(std::string_view s) {
    std::vector<std::vector<std::string>> records;
    std::size_t i = 0;
    while (i < s.size()) {
        if (std::size_t empty = line_break(s, i); empty > 0) {
            i += empty;
            continue;
        }
        std::vector<std::string> record;
        for (;;) {
            std::string field;
            if (i < s.size() && s[i] == '"') {
                ++i;
                for (;;) {
                    if (i >= s.size()) {
                        throw csv_error(csv_error::kind::bad_quote, "a quoted field has no closing quote");
                    }
                    if (s[i] == '"') {
                        if (i + 1 >= s.size() || s[i + 1] != '"') {
                            break;
                        }
                        field += '"';
                        i += 2;
                    } else if (line_break(s, i) == 2) {
                        field += '\n';  // CRLF inside quotes becomes LF.
                        i += 2;
                    } else {
                        field += s[i++];
                    }
                }
                ++i;  // the closing quote
                if (i < s.size() && s[i] != ',' && line_break(s, i) == 0) {
                    throw csv_error(csv_error::kind::bad_quote, "text after the closing quote of a field");
                }
            } else {
                std::size_t start = i;
                while (i < s.size() && s[i] != ',' && line_break(s, i) == 0) {
                    ++i;
                }
                field = s.substr(start, i - start);
            }
            record.push_back(std::move(field));
            if (i >= s.size() || s[i] != ',') {
                break;
            }
            ++i;
        }
        i += line_break(s, i);
        records.push_back(std::move(record));
    }
    return records;
}

std::vector<std::map<std::string, std::string>> read_csv(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::ios_base::failure("cannot open " + path.string());
    }
    std::string text(std::istreambuf_iterator<char>(in), {});
    std::string_view view = text;
    if (view.starts_with("\xEF\xBB\xBF")) {  // the UTF-8 byte order mark
        view.remove_prefix(3);
    }
    auto rows = parse_csv(view);
    std::vector<std::map<std::string, std::string>> records;
    for (std::size_t n = 1; n < rows.size(); ++n) {
        if (rows[n].size() != rows[0].size()) {
            throw csv_error(csv_error::kind::field_count, "record " + std::to_string(n) + " has " +
                                                              std::to_string(rows[n].size()) + " fields, want " +
                                                              std::to_string(rows[0].size()));
        }
        std::map<std::string, std::string> record;
        for (std::size_t k = 0; k < rows[0].size(); ++k) {
            record.emplace(rows[0][k], std::move(rows[n][k]));
        }
        records.push_back(std::move(record));
    }
    return records;
}
