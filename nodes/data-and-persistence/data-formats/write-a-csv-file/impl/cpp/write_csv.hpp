// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#ifndef WRITE_CSV_HPP
#define WRITE_CSV_HPP

#include <stdexcept>
#include <string>
#include <vector>

// A record has more or fewer fields than the first record (the header).
class csv_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Returns the CSV text of the records. The first record is the header.
std::string format_csv(const std::vector<std::vector<std::string>>& records);

// Writes the records to the file at path as UTF-8, with no byte order mark.
// Throws csv_error for a bad record and std::ios_base::failure if the file cannot be written.
void write_csv(const std::string& path, const std::vector<std::vector<std::string>>& records);

#endif
