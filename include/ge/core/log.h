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

#define _GE_LOG_IF_EXIST(logger, level, ...)   \
    do {                                       \
        if (logger) {                          \
            (logger)->log(level, __VA_ARGS__); \
        }                                      \
    } while (false)

#define GE_CORE_TRACE(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::trace, __VA_ARGS__)
#define GE_CORE_DBG(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::debug, __VA_ARGS__)
#define GE_CORE_INFO(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::info, __VA_ARGS__)
#define GE_CORE_WARN(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::warn, __VA_ARGS__)
#define GE_CORE_ERR(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::err, __VA_ARGS__)
#define GE_CORE_CRIT(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getCoreLogger(), spdlog::level::critical, __VA_ARGS__)

#define GE_TRACE(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::trace, __VA_ARGS__)
#define GE_DBG(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::debug, __VA_ARGS__)
#define GE_INFO(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::info, __VA_ARGS__)
#define GE_WARN(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::warn, __VA_ARGS__)
#define GE_ERR(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::err, __VA_ARGS__)
#define GE_CRIT(...) \
    _GE_LOG_IF_EXIST(::GE::Log::getClientLogger(), spdlog::level::critical, __VA_ARGS__)

namespace GE {

class GE_API Log
{
public:
    Log() = delete;

    static void initialize();
    static void shutdown();

    static Shared<spdlog::logger> getCoreLogger() { return s_core_logger; }
    static Shared<spdlog::logger> getClientLogger() { return s_client_logger; }

private:
    static Shared<spdlog::logger> s_core_logger;
    static Shared<spdlog::logger> s_client_logger;
};

} // namespace GE

#endif // GE_CORE_LOG_H_
