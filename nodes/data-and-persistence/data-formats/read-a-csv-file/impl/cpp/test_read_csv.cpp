// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-csv-file/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "read_csv.hpp"

namespace {

using record = std::map<std::string, std::string>;

const std::filesystem::path input = "read_csv_input.csv";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out << data;
}

struct test_case {
    const char* name;
    std::string data;
    std::vector<record> want;
};

int failed = 0;

void check(bool ok, const char* name) {
    if (!ok) {
        std::fprintf(stderr, "FAIL %s\n", name);
        ++failed;
    }
}

}  // namespace

int main() {
    const std::vector<record> ada_alan = {{{"name", "Ada"}, {"age", "36"}}, {{"name", "Alan"}, {"age", "41"}}};
    const std::vector<test_case> cases = {
        {"simple", "name,age\nAda,36\nAlan,41\n", ada_alan},
        {"crlf", "name,age\r\nAda,36\r\nAlan,41\r\n", ada_alan},
        {"no-final-newline", "name,age\nAda,36\nAlan,41", ada_alan},
        {"quoted-comma", "name,born\n\"Lovelace, Ada\",1815\n", {{{"name", "Lovelace, Ada"}, {"born", "1815"}}}},
        {"escaped-quote", "id,quote\n1,\"She said \"\"hi\"\".\"\n", {{{"id", "1"}, {"quote", "She said \"hi\"."}}}},
        {"quoted-header", "\"first name\",\"last name\"\nAda,Lovelace\n", {{{"first name", "Ada"}, {"last name", "Lovelace"}}}},
        {"newline-in-field", "id,address\n1,\"12 High St\nLeeds\"\n", {{{"id", "1"}, {"address", "12 High St\nLeeds"}}}},
        {"crlf-in-field", "id,address\r\n1,\"12 High St\r\nLeeds\"\r\n", {{{"id", "1"}, {"address", "12 High St\nLeeds"}}}},
        {"blank-line-in-field", "id,note\n1,\"one\n\ntwo\"\n", {{{"id", "1"}, {"note", "one\n\ntwo"}}}},
        {"empty-last-line", "a,b\n1,2\n\n", {{{"a", "1"}, {"b", "2"}}}},
        {"blank-lines", "a,b\n\n1,2\n\n\n3,4\n", {{{"a", "1"}, {"b", "2"}}, {{"a", "3"}, {"b", "4"}}}},
        {"empty-fields", "a,b,c\n,,\n", {{{"a", ""}, {"b", ""}, {"c", ""}}}},
        {"quoted-empty", "name\n\"\"\n", {{{"name", ""}}}},
        {"spaces", "a,b\n x , y \n", {{{"a", " x "}, {"b", " y "}}}},
        {"header-only", "a,b\n", {}},
        {"empty-file", "", {}},
        {"bom", "\xEF\xBB\xBFname,age\nAda,36\n", {{{"name", "Ada"}, {"age", "36"}}}},
        {"non-ascii", "city,country\nZürich,Schweiz\n東京,日本\n",
         {{{"city", "Zürich"}, {"country", "Schweiz"}}, {{"city", "東京"}, {"country", "日本"}}}},
    };
    for (const auto& tc : cases) {
        write(tc.data);
        check(read_csv(input) == tc.want, tc.name);
    }
    const std::vector<std::pair<std::string, csv_error::kind>> invalid = {
        {"a,b\n1\n", csv_error::kind::field_count},
        {"a,b\n1,2,3\n", csv_error::kind::field_count},
        {"a,b\n1,\"two\n", csv_error::kind::bad_quote},
        {"a,b\n1,\"two\"x\n", csv_error::kind::bad_quote},
    };
    for (const auto& [data, kind] : invalid) {
        write(data);
        try {
            read_csv(input);
            check(false, data.c_str());
        } catch (const csv_error& e) {
            check(e.which() == kind, data.c_str());
        }
    }
    std::filesystem::remove(input);
    try {
        read_csv(input);
        check(false, "missing");
    } catch (const std::ios_base::failure&) {
    }
    if (failed > 0) {
        return 1;
    }
    std::puts("ok");
    return 0;
}
