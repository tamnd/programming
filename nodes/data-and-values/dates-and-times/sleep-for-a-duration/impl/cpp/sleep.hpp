// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sleep-for-a-duration/.

#ifndef SLEEP_FOR_A_DURATION_HPP
#define SLEEP_FOR_A_DURATION_HPP

#include <chrono>
#include <cstddef>
#include <thread>
#include <vector>

// Sleeps until clock() reaches the deadline. A sleep that ends early is
// followed by a sleep for the rest of the time.
template <class Clock, class Sleep>
void sleep_loop(long long duration_ns, Clock clock, Sleep sleep) {
    long long now = clock();
    const long long deadline = now + duration_ns;
    while (now < deadline) {
        sleep(deadline - now);
        now = clock();
    }
}

// Blocks the thread for at least duration_ns nanoseconds. steady_clock is
// monotonic, and sleep_for promises at least the duration that it gets.
inline void sleep_for_ns(long long duration_ns) {
    using namespace std::chrono;
    const auto clock = [] { return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count(); };
    sleep_loop(duration_ns, clock, [](long long ns) { std::this_thread::sleep_for(nanoseconds{ns}); });
}

// Runs sleep_loop on a fake clock and returns the durations it asks for.
inline std::vector<long long> sleep_requests(long long duration_ns, long long start_ns,
                                             const std::vector<long long>& readings) {
    std::vector<long long> times{start_ns};
    times.insert(times.end(), readings.begin(), readings.end());
    std::size_t next = 0;
    std::vector<long long> requests;
    sleep_loop(
        duration_ns, [&] { return times.at(next++); }, [&](long long ns) { requests.push_back(ns); });
    return requests;
}

#endif
