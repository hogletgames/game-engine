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

#ifndef GE_DEBUG_PROFILE_H_
#define GE_DEBUG_PROFILE_H_

#ifdef GE_PROFILING
    #include <ge/core/asserts.h>
    #include <ge/core/log.h>
    #include <ge/core/timestamp.h>
    #include <ge/thread_pool.h>

    #include <fstream>
    #include <iomanip>
    #include <sstream>

    #define GE_PROFILE_ENABLE(enabled) ::GE::Debug::Profiler::enable(enabled)
    #define GE_PROFILE_BEGIN_SESSION(name, filepath) \
        ::GE::Debug::Profiler::begin(name, filepath)
    #define GE_PROFILE_END_SESSION() ::GE::Debug::Profiler::end()
    #define GE_PROFILE_SCOPE(name)   ::GE::Debug::Timer GE_CONCAT(timer, __LINE__)(name)
    #define GE_PROFILE_FUNC()        GE_PROFILE_SCOPE(GE_FUNC_NAME)

namespace GE::Debug {

constexpr auto PROFILER_THREAD_NAME = "Profiler";
constexpr uint32_t PROFILER_THREAD_NUM{1};
constexpr auto PROFILER_FILENAME_DEFAULT = "profile.json";

class GE_API Profiler
{
public:
    struct session_t {
        std::string name;
    };

    struct profile_result_t {
        std::string name;
        Timestamp start;
        Timestamp elapsed_time;
        std::thread::id thread_id;
    };

    static void enable(bool enabled) { get()->m_enabled = enabled; }

    static void begin(const std::string& name,
                      const std::string& filepath = PROFILER_FILENAME_DEFAULT)
    {
        auto& profile_log = get()->m_profile_log;
        auto& session = get()->m_session;

        std::lock_guard lock{get()->m_session_mtx};
        GE_CORE_ASSERT_MSG(!session, "Begin profiling '{}' when '{}' already open", name,
                           session->name);

        profile_log.open(filepath);

        if (!profile_log) {
            GE_CORE_ERR("Unable to open profiling log '{}'", filepath);
            return;
        }

        session = std::make_unique<session_t>();
        session->name = name;
        get()->writeHeader();
        get()->m_thread_pool.start(PROFILER_THREAD_NUM, true);
    }

    static void end() { get()->endSession(); }

    static void enqueueData(profile_result_t result)
    {
        if (!get()->m_enabled) {
            return;
        }

        get()->m_thread_pool.enqueue(&Profiler::writeProfile, get(), std::move(result));
    }

private:
    Profiler() = default;
    ~Profiler() { endSession(); }

    static Profiler* get()
    {
        static Profiler instance;
        return &instance;
    }

    void writeProfile(const profile_result_t& result)
    {
        m_profile_log << std::setprecision(3) << std::fixed << ",{"
                      << R"("cat":"function",)"
                      << R"("dur":)" << result.elapsed_time.us() << ","
                      << R"("name":")" << result.name << "\","
                      << R"("ph":"X",)"
                      << R"("pid":0,)"
                      << R"("tid":)" << result.thread_id << ","
                      << R"("ts":)" << result.start.us() << "}" << std::endl;
    }

    void writeHeader()
    {
        m_profile_log << R"({"otherData":{},"traceEvents":[{})" << std::endl;
    }

    void writeFooter() { m_profile_log << "]}" << std::endl; }

    void endSession()
    {
        std::lock_guard lock{m_session_mtx};

        if (!m_session) {
            return;
        }

        m_thread_pool.stop();
        writeFooter();
        m_session.reset();
        m_profile_log.close();
    }

    ThreadPool m_thread_pool{PROFILER_THREAD_NAME};
    std::unique_ptr<session_t> m_session;
    std::atomic_bool m_enabled{false};
    std::ofstream m_profile_log;
    std::mutex m_session_mtx;
};

class GE_API Timer
{
public:
    explicit Timer(const char* name)
        : m_name{name}
        , m_start_time{Timestamp::now()}
    {}

    ~Timer() { stop(); }

    void stop()
    {
        if (m_stopped) {
            return;
        }

        Timestamp elapsed_time = Timestamp::now() - m_start_time;
        auto thread_id = std::this_thread::get_id();
        Profiler::enqueueData({m_name, m_start_time, elapsed_time, thread_id});
        m_stopped = true;
    }

private:
    const char* m_name{nullptr};
    Timestamp m_start_time;
    bool m_stopped{false};
};

} // namespace GE::Debug
#else
    #define GE_PROFILE_ENABLE(enabled) static_cast<void>(enabled)
    #define GE_PROFILE_BEGIN_SESSION(name, filepath) \
        do {                                         \
            static_cast<void>(name);                 \
            static_cast<void>(filepath);             \
        } while (false)
    #define GE_PROFILE_END_SESSION()
    #define GE_PROFILE_SCOPE(name) static_cast<void>(name)
    #define GE_PROFILE_FUNC()
#endif // GE_PROFILING

#endif // GE_DEBUG_PROFILE_H_
