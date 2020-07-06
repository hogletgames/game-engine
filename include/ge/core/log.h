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

#ifndef GE_CORE_LOG_H_
#define GE_CORE_LOG_H_

#include <ge/core/core.h>

#define SPDLOG_COMPILED_LIB 1
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <memory>

#define GE_CORE_TRACE(...) ::GE::Log::get()->core()->trace(__VA_ARGS__)
#define GE_CORE_DBG(...)   ::GE::Log::get()->core()->debug(__VA_ARGS__)
#define GE_CORE_INFO(...)  ::GE::Log::get()->core()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)  ::GE::Log::get()->core()->warn(__VA_ARGS__)
#define GE_CORE_ERR(...)   ::GE::Log::get()->core()->error(__VA_ARGS__)
#define GE_CORE_CRIT(...)  ::GE::Log::get()->core()->crit(__VA_ARGS__)

#define GE_TRACE(...) ::GE::Log::get()->client()->trace(__VA_ARGS__)
#define GE_DBG(...)   ::GE::Log::get()->client()->debug(__VA_ARGS__)
#define GE_INFO(...)  ::GE::Log::get()->client()->info(__VA_ARGS__)
#define GE_WARN(...)  ::GE::Log::get()->client()->warn(__VA_ARGS__)
#define GE_ERR(...)   ::GE::Log::get()->client()->error(__VA_ARGS__)
#define GE_CRIT(...)  ::GE::Log::get()->client()->crit(__VA_ARGS__)

namespace GE {

class GE_API Logger
{
public:
    ~Logger();

    bool initialize(const std::string& name);
    void shutdown();

    template<typename... Args>
    void crit(const Args&... args)
    {
        log(spdlog::level::critical, args...);
    }

    template<typename... Args>
    void error(const Args&... args)
    {
        log(spdlog::level::err, args...);
    }

    template<typename... Args>
    void warn(const Args&... args)
    {
        log(spdlog::level::warn, args...);
    }

    template<typename... Args>
    void info(const Args&... args)
    {
        log(spdlog::level::info, args...);
    }

    template<typename... Args>
    void debug(const Args&... args)
    {
        log(spdlog::level::debug, args...);
    }

    template<typename... Args>
    void trace(const Args&... args)
    {
        log(spdlog::level::trace, args...);
    }

private:
    template<typename... Args>
    void log(const Args&... args)
    {
        if (m_logger) {
            m_logger->log(args...);
        }
    }

    std::string m_logger_name;
    std::shared_ptr<spdlog::logger> m_logger;
};

class GE_API Log
{
public:
    bool initialize();
    void shutdown();

    Logger* core() { return &m_core_logger; }
    Logger* client() { return &m_client_logger; }

    static Log* get();

private:
    Log() = default;

    Logger m_core_logger;
    Logger m_client_logger;
};

} // namespace GE

#endif // GE_CORE_LOG_H_
