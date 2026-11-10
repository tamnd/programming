// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#ifndef RUN_HPP
#define RUN_HPP

#include <string>
#include <vector>

struct RunResult {
    int status;
    std::string out;
    std::string err;

    bool operator==(const RunResult &) const = default;
};

// run starts argv[0] with the arguments argv. It searches PATH for argv[0]
// and starts no shell. The standard input of the child is /dev/null. A
// missing program throws std::system_error with the code
// std::errc::no_such_file_or_directory. A child that a signal stops throws
// std::runtime_error.
RunResult run(const std::vector<std::string> &argv);

#endif
