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

#ifndef GE_LOG_H_
#define GE_LOG_H_

#include <ge/core.h>

#if !defined(SPDLOG_ACTIVE_LEVEL)
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#include <spdlog/spdlog.h>

#include <memory>

#define GE_CORE_TRACE(...) SPDLOG_LOGGER_TRACE(::GE::Log::getCoreLogger(), __VA_ARGS__)
#define GE_CORE_DBG(...)   SPDLOG_LOGGER_DEBUG(::GE::Log::getCoreLogger(), __VA_ARGS__)
#define GE_CORE_INFO(...)  SPDLOG_LOGGER_INFO(::GE::Log::getCoreLogger(), __VA_ARGS__)
#define GE_CORE_WARN(...)  SPDLOG_LOGGER_WARN(::GE::Log::getCoreLogger(), __VA_ARGS__)
#define GE_CORE_ERR(...)   SPDLOG_LOGGER_ERROR(::GE::Log::getCoreLogger(), __VA_ARGS__)
#define GE_CORE_CRIT(...)  SPDLOG_LOGGER_CRITICAL(::GE::Log::getCoreLogger(), __VA_ARGS__)

#define GE_TRACE(...) SPDLOG_LOGGER_TRACE(::GE::Log::getClientLogger(), __VA_ARGS__)
#define GE_DBG(...)   SPDLOG_LOGGER_DEBUG(::GE::Log::getClientLogger(), __VA_ARGS__)
#define GE_INFO(...)  SPDLOG_LOGGER_INFO(::GE::Log::getClientLogger(), __VA_ARGS__)
#define GE_WARN(...)  SPDLOG_LOGGER_WARN(::GE::Log::getClientLogger(), __VA_ARGS__)
#define GE_ERR(...)   SPDLOG_LOGGER_ERROR(::GE::Log::getClientLogger(), __VA_ARGS__)
#define GE_CRIT(...)  SPDLOG_LOGGER_CRITICAL(::GE::Log::getClientLogger(), __VA_ARGS__)

namespace GE {

class GE_API Log
{
public:
    static void initialize();
    static void shutdown();

    static std::shared_ptr<spdlog::logger> getCoreLogger() { return m_core_logger; }
    static std::shared_ptr<spdlog::logger> getClientLogger() { return m_client_logger; }

private:
    static std::shared_ptr<spdlog::logger> m_core_logger;
    static std::shared_ptr<spdlog::logger> m_client_logger;
};

} // namespace GE

#endif // GE_LOG_H_
