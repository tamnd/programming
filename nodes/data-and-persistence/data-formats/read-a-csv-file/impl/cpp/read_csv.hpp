// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

#pragma once

#include <filesystem>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// The error for a file that is not valid CSV.
class csv_error : public std::runtime_error {
public:
    enum class kind { field_count, bad_quote };
    csv_error(kind k, const std::string& what) : std::runtime_error(what), kind_(k) {}
    [[nodiscard]] kind which() const noexcept { return kind_; }

private:
    kind kind_;
};

// Splits RFC 4180 text into records of fields. An empty line gives no record.
std::vector<std::vector<std::string>> parse_csv(std::string_view text);

// Returns one map for each record after the header, from each name to its field.
// Throws csv_error for text that is not valid CSV, and std::ios_base::failure if the file cannot be opened.
std::vector<std::map<std::string, std::string>> read_csv(const std::filesystem::path& path);
