// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#include "pool.hpp"

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

namespace pool {

namespace {

// A fixed set of threads that take jobs from one queue.
class ThreadPool {
public:
    explicit ThreadPool(std::size_t workers) {
        try {
            for (std::size_t i = 0; i < workers; i++) {
                threads_.emplace_back([this] { run(); });
            }
        } catch (...) {
            stop();  // the threads that did start must not wait forever
            throw;
        }
    }

    // Lets the threads finish the queued jobs, then joins them.
    ~ThreadPool() {
        stop();
        // threads_ is the last member, so it is destroyed first: each std::jthread joins.
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Queues a job and returns a future for its result.
    std::future<std::int32_t> submit(std::function<std::int32_t()> job) {
        std::packaged_task<std::int32_t()> task(std::move(job));
        std::future<std::int32_t> result = task.get_future();
        {
            std::lock_guard lock(mutex_);
            queue_.push_back(std::move(task));
        }
        ready_.notify_one();
        return result;
    }

private:
    void stop() {
        {
            std::lock_guard lock(mutex_);
            stopping_ = true;
        }
        ready_.notify_all();
    }

    void run() {
        for (;;) {
            std::packaged_task<std::int32_t()> task;
            {
                std::unique_lock lock(mutex_);
                ready_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
                if (queue_.empty()) {
                    return;  // stopping, and no job is left
                }
                task = std::move(queue_.front());
                queue_.pop_front();
            }
            task();  // runs without the lock; stores the value or the exception in the future
        }
    }

    std::mutex mutex_;
    std::condition_variable ready_;
    std::deque<std::packaged_task<std::int32_t()>> queue_;
    bool stopping_ = false;
    std::vector<std::jthread> threads_;
};

}  // namespace

std::int32_t collatz_steps(std::int64_t n) {
    std::int32_t steps = 0;
    while (n != 1) {
        n = n % 2 == 0 ? n / 2 : 3 * n + 1;
        steps++;
    }
    return steps;
}

std::vector<std::int32_t> steps_on_pool(std::size_t workers, const std::vector<std::int32_t>& numbers) {
    std::vector<std::future<std::int32_t>> futures;
    {
        ThreadPool threads(workers);
        for (std::int32_t n : numbers) {
            futures.push_back(threads.submit([n] { return collatz_steps(n); }));
        }
    }  // the destructor waits for every job and joins the threads
    std::vector<std::int32_t> results;
    for (auto& future : futures) {
        results.push_back(future.get());  // returns the value, or throws the exception of the job
    }
    return results;
}

}  // namespace pool
