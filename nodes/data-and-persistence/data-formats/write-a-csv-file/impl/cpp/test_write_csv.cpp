// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-csv-file/.

#include "write_csv.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using rows = std::vector<std::vector<std::string>>;

struct test_case {
    const char* name;
    rows records;
    std::string want;
};

int main() {
    const std::vector<test_case> cases = {
        {"simple", {{"name", "age"}, {"Ada", "36"}, {"Alan", "41"}}, "name,age\r\nAda,36\r\nAlan,41\r\n"},
        {"comma", {{"name", "born"}, {"Lovelace, Ada", "1815"}}, "name,born\r\n\"Lovelace, Ada\",1815\r\n"},
        {"double-quote", {{"id", "quote"}, {"1", "She said \"hi\"."}}, "id,quote\r\n1,\"She said \"\"hi\"\".\"\r\n"},
        {"only-a-quote", {{"q"}, {"\""}}, "q\r\n\"\"\"\"\r\n"},
        {"header-only", {{"name", "age"}}, "name,age\r\n"},
        {"newline-in-field", {{"id", "address"}, {"1", "12 High St\nLeeds"}}, "id,address\r\n1,\"12 High St\nLeeds\"\r\n"},
        {"crlf-in-field", {{"id", "address"}, {"1", "12 High St\r\nLeeds"}}, "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n"},
        {"cr-in-field", {{"id", "text"}, {"1", "a\rb"}}, "id,text\r\n1,\"a\rb\"\r\n"},
        {"empty-fields", {{"a", "b", "c"}, {"", "", ""}}, "a,b,c\r\n,,\r\n"},
        {"one-empty-field", {{"note"}, {""}, {"x"}}, "note\r\n\"\"\r\nx\r\n"},
        {"spaces", {{"a", "b"}, {" x ", " y"}}, "a,b\r\n x , y\r\n"},
        {"no-records", {}, ""},
        {"non-ascii", {{"city"}, {"Z\xc3\xbcrich"}, {"\xe6\x9d\xb1\xe4\xba\xac"}}, "city\r\nZ\xc3\xbcrich\r\n\xe6\x9d\xb1\xe4\xba\xac\r\n"},
    };
    const std::vector<std::pair<const char*, rows>> errors = {
        {"too-many-fields", {{"a", "b"}, {"1", "2", "3"}}},
        {"too-few-fields", {{"a", "b"}, {"1"}}},
    };
    const std::string path = (std::filesystem::temp_directory_path() / "write-csv-test.csv").string();
    int failed = 0;
    for (const auto& t : cases) {
        bool ok = format_csv(t.records) == t.want;
        write_csv(path, t.records);
        std::ifstream in(path, std::ios::binary);
        std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        ok = ok && data == t.want;
        if (!ok) {
            std::cout << "FAIL " << t.name << "\n";
            failed++;
        }
    }
    for (const auto& [name, records] : errors) {
        std::remove(path.c_str());
        bool ok = false;
        try {
            format_csv(records);
        } catch (const csv_error&) {
            ok = true;
        }
        try {
            write_csv(path, records);
            ok = false;
        } catch (const csv_error&) {
        }
        ok = ok && !std::filesystem::exists(path);
        if (!ok) {
            std::cout << "FAIL " << name << "\n";
            failed++;
        }
    }
    std::remove(path.c_str());
    std::cout << (failed ? "FAILED" : "all tests passed") << "\n";
    return failed != 0;
}
