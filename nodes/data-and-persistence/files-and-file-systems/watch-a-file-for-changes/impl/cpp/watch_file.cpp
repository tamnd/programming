// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#include "watch_file.hpp"

#include <fstream>
#include <thread>

namespace fs = std::filesystem;

namespace files {

namespace {

// The signature of the file, or nothing when it does not exist.
std::optional<Signature> signature(const fs::path& path, std::error_code& ec) {
    fs::file_status st = fs::status(path, ec);
    if (ec || !fs::exists(st)) {
        if (st.type() == fs::file_type::not_found) ec.clear();
        return std::nullopt;
    }
    if (fs::is_directory(st)) {
        ec = std::make_error_code(std::errc::is_a_directory);
        return std::nullopt;
    }
    Signature sig{fs::file_size(path, ec), {}};
    if (ec) return std::nullopt;
    sig.mtime = fs::last_write_time(path, ec);
    if (ec) return std::nullopt;
    return sig;
}

bool put(const fs::path& path, const std::string& text, std::ios::openmode mode) {
    std::ofstream out(path, std::ios::binary | mode);
    out << text;
    out.close();
    return static_cast<bool>(out);
}

// Makes the change of one step of a case (the entry of the cases uses it).
void apply_step(const fs::path& path, const std::string& step, std::int64_t number, std::error_code& ec) {
    auto colon = step.find(':');
    std::string kind = step.substr(0, colon);
    std::string text = colon == std::string::npos ? "" : step.substr(colon + 1);
    bool ok = true;
    if (kind == "create" || kind == "write") {
        ok = put(path, text, std::ios::trunc);
    } else if (kind == "append") {
        ok = fs::exists(path, ec) && put(path, text, std::ios::app);
    } else if (kind == "replace") {
        fs::path tmp = path;
        tmp += ".tmp";
        ok = put(tmp, text, std::ios::trunc);
        if (ok) fs::rename(tmp, path, ec);
    } else if (kind == "recreate") {
        if (fs::remove(path, ec)) ok = put(path, text, std::ios::trunc);
    } else if (kind == "delete") {
        if (!fs::remove(path, ec) && !ec) ec = std::make_error_code(std::errc::no_such_file_or_directory);
    } else if (kind == "mkdir") {
        fs::create_directory(path, ec);
    } else if (kind != "touch" && kind != "none") {
        ec = std::make_error_code(std::errc::invalid_argument);
    }
    if (!ok && !ec) ec = std::make_error_code(std::errc::io_error);
    if (ec) return;
    std::error_code ignore;
    if (kind != "none" && fs::is_regular_file(path, ignore)) {
        // The clock of file_time_type has no fixed epoch, so the time goes
        // through the system clock.
        auto sys = std::chrono::system_clock::time_point{std::chrono::seconds{base_mtime + number}};
        auto t = fs::file_time_type::clock::from_sys(sys);
        fs::last_write_time(path, t, ec);
    }
}

}  // namespace

FileWatcher::FileWatcher(fs::path path, std::error_code& ec) : path_(std::move(path)), last_(signature(path_, ec)) {}

std::string FileWatcher::poll(std::error_code& ec) {
    std::optional<Signature> now = signature(path_, ec);
    if (ec) return "";
    std::optional<Signature> before = last_;
    last_ = now;
    if (!before && !now) return "none";
    if (!before) return "created";
    if (!now) return "deleted";
    return *before == *now ? "none" : "modified";
}

void watch(const fs::path& path, const std::function<void(const fs::path&)>& on_change,
           std::chrono::milliseconds interval, std::chrono::milliseconds quiet, const std::function<bool()>& stop,
           std::error_code& ec) {
    FileWatcher watcher(path, ec);
    if (ec) return;
    std::optional<std::chrono::steady_clock::time_point> pending;  // a change that on_change has not seen
    while (!stop()) {
        std::string change = watcher.poll(ec);
        if (ec) return;
        auto now = std::chrono::steady_clock::now();
        if (change != "none") {
            pending = now;
        } else if (pending && now - *pending >= quiet) {
            pending.reset();
            on_change(path);
        }
        std::this_thread::sleep_for(interval);
    }
}

std::vector<std::string> watch_changes(const fs::path& path, const std::vector<std::string>& steps,
                                       std::error_code& ec) {
    std::vector<std::string> results;
    FileWatcher watcher(path, ec);
    if (ec) return {};
    for (std::size_t i = 0; i < steps.size(); ++i) {
        apply_step(path, steps[i], static_cast<std::int64_t>(i) + 1, ec);
        if (ec) return {};
        results.push_back(watcher.poll(ec));
        if (ec) return {};
    }
    return results;
}

}  // namespace files
