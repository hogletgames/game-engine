/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, Dmitry Shilnenkov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GE_THREAD_POOL_H_
#define GE_THREAD_POOL_H_

#include <ge/core/core.h>

#include <functional>
#include <future>
#include <queue>

namespace GE {

class GE_API ThreadPool
{
public:
    explicit ThreadPool(const char* name);
    ~ThreadPool();

    void start(uint32_t threads_num, bool wait_for_query_end);
    void stop();

    template<typename Func, typename... Args>
    void enqueue(Func&& func, Args&&... args)
    {
        if (m_terminated) {
            return;
        }

        std::unique_lock lock{m_queue_mtx};
        m_queue.emplace(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        lock.unlock();
        m_condition.notify_one();
    }

private:
    using Task = std::function<void()>;

    void workerThread();

    std::atomic_bool m_terminated{true};
    bool m_wait_for_query_end{false};
    std::condition_variable m_condition;
    std::vector<std::thread> m_workers;
    const char* m_name{nullptr};
    std::queue<Task> m_queue;
    std::mutex m_queue_mtx;
};

} // namespace GE

#endif // GE_THREAD_POOL_H_
