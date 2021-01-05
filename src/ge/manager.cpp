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
#include "app_properties.h"

#include "ge/application.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/gui/gui.h"
#include "ge/renderer/renderer.h"
#include "ge/renderer/renderer_2d.h"
#include "ge/window/window.h"

namespace GE {

Manager::~Manager()
{
    if (m_initialized) {
        shutdown();
    }
}

bool Manager::initialize(std::string props_file)
{
    GE_PROFILE_FUNC();

    AppProperties::properties_t props{};

    if (!Log::initialize()) {
        return false;
    }

    if (!AppProperties::read(props_file, &props)) {
        GE_CORE_ERR("Failed to load application conf");
        return false;
    }

    Log::core()->setLevel(props.core_log_lvl);
    Log::client()->setLevel(props.client_log_lvl);

    if (!Renderer::initialize(props.api) || !Window::initialize() ||
        !Application::initialize(props.window) ||
        !Renderer2D::initialize(props.assets_dir) || !Gui::initialize(props.assets_dir)) {
        return false;
    }

    GE_CORE_DBG("GameEngine: has been initialized");
    get()->m_props_file = std::move(props_file);
    get()->m_initialized = true;
    return true;
}

void Manager::shutdown()
{
    GE_PROFILE_FUNC();

    GE_CORE_DBG("GameEngine: scheduling shutdown");
    get()->saveProperties();

    Gui::shutdown();
    Renderer2D::shutdown();
    Application::shutdown();
    Window::shutdown();
    Renderer::shutdown();
    Log::shutdown();

    get()->m_props_file.clear();
    get()->m_initialized = false;
}

void Manager::saveProperties() const
{
    GE_PROFILE_FUNC();

    AppProperties::properties_t props{};

    props.api = Renderer::getAPI();
    props.core_log_lvl = Log::core()->getLvel();
    props.client_log_lvl = Log::client()->getLvel();
    props.assets_dir = Renderer2D::getAssetsDir();
    props.window = Application::getWindow().getProps();

    AppProperties::write(m_props_file, props);
}

} // namespace GE
