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

#ifndef GE_CORE_UTILS_H_
#define GE_CORE_UTILS_H_

#include <memory>
#include <thread>
#include <unordered_map>

#include <ge/core/core.h>
#include <ge/core/timestamp.h>

namespace GE {

template<typename Type, typename... Args>
inline Scoped<Type> makeScoped(Args&&... args)
{
    return std::make_unique<Type>(std::forward<Args>(args)...);
}

template<typename Type, typename... Args>
inline Shared<Type> makeShared(Args&&... args)
{
    return std::make_shared<Type>(std::forward<Args>(args)...);
}

inline void sleep(Timestamp duration)
{
    std::this_thread::sleep_for(Timestamp::DurationSec{duration});
}

template<typename FromType, typename ToType>
inline ToType toType(const std::unordered_map<FromType, ToType>& cont,
                     const FromType& from_value, ToType&& def_ret)
{
    if (auto it = cont.find(from_value); it != cont.end()) {
        return it->second;
    }

    return std::forward<ToType>(def_ret);
}

} // namespace GE

#endif // GE_CORE_UTILS_H_
