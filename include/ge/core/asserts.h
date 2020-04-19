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

#ifndef GE_CORE_ASSERTS_H_
#define GE_CORE_ASSERTS_H_

#include <ge/core/log.h>

#if !defined(GE_DISABLE_ASSERTS)
    #define GE_CORE_ASSERT(x, ...) \
        ::GE::core_assert(x, #x, __FILE__, __LINE__, __VA_ARGS__)

    #define GE_ASSERT(x, ...) ::GE::client_assert(x, #x, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define GE_CORE_ASSERT(x, ...) static_cast<void>(x)
    #define GE_ASSERT(x, ...)      static_cast<void>(x)
#endif

namespace GE {

template<typename... Args>
inline void core_assert(bool expr, const char* expr_str, const char* file, size_t line,
                        Args&&... args)
{
    if (!expr) {
        GE_CORE_CRIT("assert failed: {}:{}: '{}'", file, line, expr_str);
        GE_CORE_CRIT(std::forward<Args>(args)...);
        std::terminate();
    }
}

template<typename... Args>
inline void client_assert(bool expr, const char* expr_str, const char* file, size_t line,
                          Args&&... args)
{
    if (!expr) {
        GE_CRIT("assert failed: {}:{}: '{}'", file, line, expr_str);
        GE_CRIT(std::forward<Args>(args)...);
        std::terminate();
    }
}

} // namespace GE

#endif // GE_CORE_ASSERTS_H_
