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

#define GE_COMPILED_LOGLVL_CRITICAL 1
#define GE_COMPILED_LOGLVL_ERROR    2
#define GE_COMPILED_LOGLVL_WARNING  3
#define GE_COMPILED_LOGLVL_INFO     4
#define GE_COMPILED_LOGLVL_DEBUG    5
#define GE_COMPILED_LOGLVL_TRACE    6

#if !defined(_GE_ACTIVE_LOGLVL)
    #define _GE_ACTIVE_LOGLVL GE_COMPILED_LOGLVL_TRACE
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_CRITICAL
    #define GE_CORE_CRIT(...) ::GE::Log::core()->crit(__VA_ARGS__)
    #define GE_CRIT(...)      ::GE::Log::client()->crit(__VA_ARGS__)
#else
    #define GE_CORE_CRIT(...) static_cast<void>(0)
    #define GE_CRIT(...)      static_cast<void>(0)
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_ERROR
    #define GE_CORE_ERR(...) ::GE::Log::core()->error(__VA_ARGS__)
    #define GE_ERR(...)      ::GE::Log::client()->error(__VA_ARGS__)
#else
    #define GE_CORE_ERR(...) static_cast<void>(0)
    #define GE_ERR(...)      static_cast<void>(0)
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_WARNING
    #define GE_CORE_WARN(...) ::GE::Log::core()->warn(__VA_ARGS__)
    #define GE_WARN(...)      ::GE::Log::client()->warn(__VA_ARGS__)
#else
    #define GE_CORE_WARN(...) static_cast<void>(0)
    #define GE_WARN(...)      static_cast<void>(0)
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_INFO
    #define GE_CORE_INFO(...) ::GE::Log::core()->info(__VA_ARGS__)
    #define GE_INFO(...)      ::GE::Log::client()->info(__VA_ARGS__)
#else
    #define GE_CORE_INFO(...) static_cast<void>(0)
    #define GE_INFO(...)      static_cast<void>(0)
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_DEBUG
    #define GE_CORE_DBG(...) ::GE::Log::core()->debug(__VA_ARGS__)
    #define GE_DBG(...)      ::GE::Log::client()->debug(__VA_ARGS__)
#else
    #define GE_CORE_DBG(...) static_cast<void>(0)
    #define GE_DBG(...)      static_cast<void>(0)
#endif

#if _GE_ACTIVE_LOGLVL >= GE_COMPILED_LOGLVL_TRACE
    #define GE_CORE_TRACE(...) ::GE::Log::core()->trace(__VA_ARGS__)
    #define GE_TRACE(...)      ::GE::Log::client()->trace(__VA_ARGS__)
#else
    #define GE_CORE_TRACE(...) static_cast<void>(0)
    #define GE_TRACE(...)      static_cast<void>(0)
#endif

#define GE_LOGLVL_TRACE ::GE::Logger::Level::TRACE
#define GE_LOGLVL_DBG   ::GE::Logger::Level::DEBUG
#define GE_LOGLVL_INFO  ::GE::Logger::Level::INFO
#define GE_LOGLVL_WARN  ::GE::Logger::Level::WARNING
#define GE_LOGLVL_ERR   ::GE::Logger::Level::ERROR
#define GE_LOGLVL_CRIT  ::GE::Logger::Level::CRITICAL
#define GE_LOGLVL_NONE  ::GE::Logger::Level::NONE

namespace GE {

class GE_API Logger
{
public:
    enum class Level : uint8_t
    {
        CRITICAL = GE_COMPILED_LOGLVL_CRITICAL,
        ERROR = GE_COMPILED_LOGLVL_ERROR,
        WARNING = GE_COMPILED_LOGLVL_WARNING,
        INFO = GE_COMPILED_LOGLVL_INFO,
        DEBUG = GE_COMPILED_LOGLVL_DEBUG,
        TRACE = GE_COMPILED_LOGLVL_TRACE,
        NONE
    };

    ~Logger();

    bool initialize(const std::string& name);
    void shutdown();

    void setLevel(Level level);
    Level getLvel() const { return m_level; }

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
    Level m_level{GE_LOGLVL_INFO};
    std::shared_ptr<spdlog::logger> m_logger;
};

class GE_API Log
{
public:
    static bool initialize();
    static void shutdown();

    static Logger* core() { return &get()->m_core_logger; }
    static Logger* client() { return &get()->m_client_logger; }

private:
    Log() = default;

    static Log* get()
    {
        static Log instance;
        return &instance;
    }

    Logger m_core_logger;
    Logger m_client_logger;
};

std::string toString(Logger::Level level);
Logger::Level toLogLvl(const std::string& level);

} // namespace GE

#endif // GE_CORE_LOG_H_
