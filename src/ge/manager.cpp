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

#include "manager.h"

#include "ge/application.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/gui/gui.h"
#include "ge/renderer/renderer.h"
#include "ge/window/window.h"

namespace GE {

Manager::~Manager()
{
    if (m_initialized) {
        shutdown();
    }
}

bool Manager::initialize(RendererAPI::API api)
{
    if (!Log::initialize() || !Renderer::initialize(api) || !Window::initialize() ||
        !Application::initialize() || !Gui::initialize()) {
        return false;
    }

    GE_CORE_DBG("GameEngine: has been initialized");
    get()->m_initialized = true;
    return true;
}

void Manager::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("GameEngine: scheduling shutdown");
    Gui::shutdown();
    Application::shutdown();
    Window::shutdown();
    Renderer::shutdown();
    Log::shutdown();

    get()->m_initialized = false;
}

} // namespace GE
