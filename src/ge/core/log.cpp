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

#include "log.h"

#include "ge/core/asserts.h"
#include "ge/core/utils.h"
#include "ge/debug/profile.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

#define CORE_LOGGER_NAME   "CORE"
#define CLIENT_LOGGER_NAME "APP"
#define LOG_FTM_PATTERN    "[%H:%M:%S.%e] [%l] %n: %v%$"

#define LOGLVL_TRACE_STR "Trace"
#define LOGLVL_DBG_STR   "Debug"
#define LOGLVL_INFO_STR  "Info"
#define LOGLVL_WARN_STR  "Warning"
#define LOGLVL_ERR_STR   "Error"
#define LOGLVL_CRIT_STR  "Critical"
#define LOGLVL_NONE_STR  "None"

using SpdlogLevel = spdlog::level::level_enum;

namespace {

SpdlogLevel toSpdlogLvl(GE::Logger::Level level)
{
    switch (level) {
        case GE_LOGLVL_TRACE: return SpdlogLevel::trace;
        case GE_LOGLVL_DBG: return SpdlogLevel::debug;
        case GE_LOGLVL_INFO: return SpdlogLevel::info;
        case GE_LOGLVL_WARN: return SpdlogLevel::warn;
        case GE_LOGLVL_ERR: return SpdlogLevel::err;
        case GE_LOGLVL_CRIT:
        case GE_LOGLVL_NONE: return SpdlogLevel::critical;
        default: break;
    }

    GE_CORE_ASSERT_MSG(false, "Unknown log level: {}", static_cast<int>(level));
    return SpdlogLevel::critical;
}

} // namespace

namespace GE {

Logger::~Logger()
{
    if (m_logger) {
        shutdown();
    }
}

bool Logger::initialize(const std::string& name)
{
    GE_PROFILE_FUNC();

    try {
        m_logger = spdlog::stdout_color_mt(name);
    } catch (const spdlog::spdlog_ex& e) {
        std::cerr << "Failed to create logger '" << name << "': " << e.what()
                  << std::endl;
        return false;
    }

    m_logger_name = name;
    m_logger->set_pattern(LOG_FTM_PATTERN);

    return true;
}

void Logger::shutdown()
{
    GE_PROFILE_FUNC();

    spdlog::drop(m_logger_name);
    m_logger.reset();
    m_logger_name.clear();
}

void Logger::setLevel(Level level)
{
    GE_PROFILE_FUNC();

    m_level = level;
    m_logger->set_level(toSpdlogLvl(level));
    m_level = level;
}

bool Log::initialize()
{
    GE_PROFILE_FUNC();

    auto& core_logger = get()->m_core_logger;
    auto& client_logger = get()->m_client_logger;

    if (!core_logger.initialize(CORE_LOGGER_NAME) ||
        !client_logger.initialize(CLIENT_LOGGER_NAME)) {
        shutdown();
        return false;
    }

    core_logger.setLevel(GE_LOGLVL_TRACE);
    client_logger.setLevel(GE_LOGLVL_TRACE);

    GE_CORE_DBG("Log system has been initialized");
    return true;
}

void Log::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("Shutdown log system");
    get()->m_client_logger.shutdown();
    get()->m_core_logger.shutdown();
}

std::string toString(Logger::Level level)
{
    std::unordered_map<Logger::Level, std::string> lvl_to_str{
        {GE_LOGLVL_TRACE, LOGLVL_TRACE_STR}, {GE_LOGLVL_DBG, LOGLVL_DBG_STR},
        {GE_LOGLVL_INFO, LOGLVL_INFO_STR},   {GE_LOGLVL_WARN, LOGLVL_WARN_STR},
        {GE_LOGLVL_ERR, LOGLVL_ERR_STR},     {GE_LOGLVL_CRIT, LOGLVL_CRIT_STR},
        {GE_LOGLVL_NONE, LOGLVL_NONE_STR}};

    return toType(lvl_to_str, level, {});
}

Logger::Level toLogLvl(const std::string& level)
{
    std::unordered_map<std::string, Logger::Level> str_to_lvl{
        {LOGLVL_TRACE_STR, GE_LOGLVL_TRACE}, {LOGLVL_DBG_STR, GE_LOGLVL_DBG},
        {LOGLVL_INFO_STR, GE_LOGLVL_INFO},   {LOGLVL_WARN_STR, GE_LOGLVL_WARN},
        {LOGLVL_ERR_STR, GE_LOGLVL_ERR},     {LOGLVL_CRIT_STR, GE_LOGLVL_CRIT}};

    return toType(str_to_lvl, level, GE_LOGLVL_NONE);
}

} // namespace GE
