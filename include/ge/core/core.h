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

#ifndef GE_CORE_H_
#define GE_CORE_H_

#include <functional>

#if defined(GE_PLATFORM_WINDOWS)
    #if !defined(GE_STATIC)
        #define GE_API __declspec(dllexport)
    #else
        #define GE_API __declspec(dllimport)
    #endif
#else
    #define GE_API
#endif

#if defined(GE_ENABLE_ASSERTS)
    #define GE_CORE_ASSERT(x, ...)                       \
        {                                                \
            if (!(x)) {                                  \
                GE_CORE_CRIT("assert failed: '{}'", #x); \
                GE_CORE_CRIT(__VA_ARGS__);               \
                std::terminate();                        \
            }                                            \
        }
    #define GE_ASSERT(x, ...)                       \
        {                                           \
            if (!(x)) {                             \
                GE_CRIT("assert failed: '{}'", #x); \
                GE_CRIT(__VA_ARGS__);               \
                std::terminate();                   \
            }                                       \
        }
#else
    #define GE_CORE_ASSERT(x, ...)
    #define GE_ASSERT(x, ...)
#endif

#define GE_BIND_MEM_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#endif // GE_CORE_H_
