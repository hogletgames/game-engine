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

#include "thread_pool.h"

#include "ge/core/asserts.h"

namespace GE {

ThreadPool::ThreadPool(const char* name)
    : m_name{name}
{}

ThreadPool::~ThreadPool()
{
    if (!m_terminated) {
        stop();
    }
}

void ThreadPool::start(uint32_t threads_num)
{
    GE_CORE_ASSERT(m_terminated, "Thread pool '{}' has already ran", m_name);

    m_terminated = false;
    m_workers.clear();

    std::lock_guard lock{m_queue_mtx};
    m_queue = {};

    for (uint32_t i{0}; i < threads_num; i++) {
        m_workers.emplace_back(&ThreadPool::workerThread, this);
    }

    GE_CORE_TRACE("Thread pool '{}' has been started", m_name);
}

void ThreadPool::stop()
{
    m_terminated = true;
    m_condition.notify_all();

    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    GE_CORE_TRACE("Thread pool '{}' has been stopped", m_name);
}

void ThreadPool::workerThread()
{
    auto wake_up_cond = [this] { return m_terminated || !m_queue.empty(); };

    while (true) {
        std::unique_lock lock{m_queue_mtx};
        m_condition.wait(lock, wake_up_cond);

        if (m_terminated) {
            break;
        }

        auto task = m_queue.front();
        m_queue.pop();
        lock.unlock();

        task();
    }
}

} // namespace GE
