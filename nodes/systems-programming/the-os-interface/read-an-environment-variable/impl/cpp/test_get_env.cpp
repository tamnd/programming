// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

#include "get_env.hpp"

#include <array>
#include <cstdio>
#include <optional>
#include <print>
#include <string>
#include <utility>

// setenv and unsetenv are POSIX functions in <stdlib.h>.
#include <stdlib.h>

struct Case {
    const char* label;
    std::optional<std::pair<const char*, const char*>> var;  // the variable to set
    const char* name;
    const char* def;
    std::string want;
};

int main() {
    const std::array<Case, 12> cases{{
        {"present", {{"APP_PORT", "8080"}}, "APP_PORT", "80", "8080"},
        {"path-value", {{"APP_HOME", "/srv/app"}}, "APP_HOME", "/opt/app", "/srv/app"},
        {"missing", std::nullopt, "APP_PORT", "80", "80"},
        {"empty-value", {{"APP_MODE", ""}}, "APP_MODE", "production", ""},
        {"empty-default", std::nullopt, "APP_MODE", "", ""},
        {"spaces", {{"APP_TITLE", "  two  spaces  "}}, "APP_TITLE", "none", "  two  spaces  "},
        {"equals", {{"APP_QUERY", "key=value=more"}}, "APP_QUERY", "none", "key=value=more"},
        {"newline", {{"APP_TEXT", "first\nsecond"}}, "APP_TEXT", "none", "first\nsecond"},
        {"non-ascii", {{"APP_GREETING", "xin chào, 日本語, 🙂"}}, "APP_GREETING", "none",
         "xin chào, 日本語, 🙂"},
        {"case", {{"APP_PORT", "8080"}}, "app_port", "80", "80"},
        {"inherited-name", std::nullopt, "toString", "none", "none"},
        // A std::string holds bytes, so a value that is not UTF-8 comes back unchanged.
        {"not-utf8", {{"APP_NAME", "caf\xe9"}}, "APP_NAME", "none", "caf\xe9"},
    }};
    int failures = 0;
    for (const Case& c : cases) {
        unsetenv(c.name);
        if (c.var && setenv(c.var->first, c.var->second, 1) != 0) {
            std::perror("setenv");
            return 1;
        }
        std::string got = get_env(c.name, c.def);
        if (got != c.want) {
            std::println(stderr, "case {}: got \"{}\", want \"{}\"", c.label, got, c.want);
            ++failures;
        }
        if (c.var) {
            unsetenv(c.var->first);
        }
    }
    if (failures != 0) {
        return 1;
    }
    std::println("all cases pass");
    return 0;
}
