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

#ifndef GE_WINDOW_WINDOW_H_
#define GE_WINDOW_WINDOW_H_

#include <ge/core/non_copyable.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace GE {

class Event;

class GE_API Window: public NonCopyable
{
public:
    using WinEventCallback = std::function<void(Event*)>;

    struct properties_t {
        std::string title{TITLE_DEFAULT};
        uint32_t width{WIDTH_DEFAULT};
        uint32_t height{HEIGHT_DEFAULT};
        bool vsync{VSYNC_DEFAULT};

        static constexpr auto TITLE_DEFAULT = "Game Engine";
        static constexpr uint32_t WIDTH_DEFAULT{1280};
        static constexpr uint32_t HEIGHT_DEFAULT{720};
        static constexpr bool VSYNC_DEFAULT{true};
    };

    static bool initialize();
    static void shutdown();

    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;

    virtual void* getNativeWindow() const = 0;
    virtual void* getNativeContext() const = 0;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual const properties_t& getProps() const = 0;

    virtual void onUpdate() = 0;
    virtual void setEventCallback(WinEventCallback callback) = 0;

    static Scoped<Window> create(properties_t properties);
};

} // namespace GE

#endif // GE_WINDOW_WINDOW_H_
