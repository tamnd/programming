// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#include "write_csv.hpp"

#include <fstream>

namespace {

// A record with one empty field needs quotes, or the line is empty and a reader skips it.
bool needs_quotes(const std::string& field, bool alone) {
    return (alone && field.empty()) || field.find_first_of(",\"\r\n") != std::string::npos;
}

}  // namespace

std::string format_csv(const std::vector<std::vector<std::string>>& records) {
    std::string out;
    for (std::size_t n = 0; n < records.size(); n++) {
        const auto& record = records[n];
        if (record.size() != records[0].size()) {
            throw csv_error("record " + std::to_string(n) + " has " + std::to_string(record.size()) +
                            " fields, the header has " + std::to_string(records[0].size()));
        }
        for (std::size_t i = 0; i < record.size(); i++) {
            if (i > 0) {
                out += ',';
            }
            const std::string& field = record[i];
            if (needs_quotes(field, record.size() == 1)) {
                out += '"';
                for (char c : field) {
                    if (c == '"') {
                        out += '"';
                    }
                    out += c;
                }
                out += '"';
            } else {
                out += field;
            }
        }
        out += "\r\n";
    }
    return out;
}

void write_csv(const std::string& path, const std::vector<std::vector<std::string>>& records) {
    std::string text = format_csv(records);
    // Binary mode: on Windows, text mode would write each LF as CRLF, also inside a field.
    std::ofstream f(path, std::ios::binary);
    f.exceptions(std::ios::failbit | std::ios::badbit);
    f.write(text.data(), static_cast<std::streamsize>(text.size()));
    f.close();
}
