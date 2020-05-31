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

#ifndef GE_CORE_TIMESTAMP_H_
#define GE_CORE_TIMESTAMP_H_

#include <ge/core/core.h>

#include <chrono>

#define GE_MS_IN_SEC 1e3
#define GE_US_IN_SEC 1e6
#define GE_NS_IN_SEC 1e9

namespace GE {

class GE_API Timestamp
{
public:
    using DurationSec = std::chrono::duration<double>;
    using DurationMilli = std::chrono::duration<double, std::milli>;
    using DurationMicro = std::chrono::duration<double, std::micro>;
    using DurationNano = std::chrono::duration<double, std::nano>;

    Timestamp(double time_sec = 0.0) // NOLINT
        : m_time_sec{time_sec}
    {}

    static Timestamp now()
    {
        using std::chrono::duration_cast;
        auto now = std::chrono::steady_clock::now();
        return duration_cast<DurationSec>(now.time_since_epoch()).count();
    }

    double sec() const { return m_time_sec; }
    double ms() const { return m_time_sec * GE_MS_IN_SEC; }
    double us() const { return m_time_sec * GE_US_IN_SEC; }
    double ns() const { return m_time_sec * GE_NS_IN_SEC; }

    operator double() const { return m_time_sec; } // NOLINT
    Timestamp operator-(Timestamp rhs) const { return m_time_sec - rhs.m_time_sec; }
    Timestamp operator+(Timestamp rhs) const { return m_time_sec + rhs.m_time_sec; }

    Timestamp& operator-=(Timestamp rhs)
    {
        m_time_sec -= rhs.m_time_sec;
        return *this;
    }

    Timestamp& operator+=(Timestamp rhs)
    {
        m_time_sec += rhs.m_time_sec;
        return *this;
    }

private:
    double m_time_sec{};
};

} // namespace GE

#endif // GE_CORE_TIMESTAMP_H_
