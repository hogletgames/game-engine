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

#include "renderer_2d.h"
#include "triangle_layer.h"

#include <ge/debug/profile.h>
#include <ge/ge.h>

#include <docopt.h>

#include <iostream>
#include <unordered_map>

#define EXAMPLE_OPT  "--example"
#define SHOW_GUI_OPT "--show-demo"
#define PROFILE_OPT  "--profiling"
#define CONFIG_OPT   "--config"

#define EXAMPLE_EMPTY       "empty"
#define EXAMPLE_RENDERER_2D "renderer2d"
#define EXAMPLE_TRIANGLE    "triangle"

#define PROFILE_SESSION_NAME "Sandbox Profiling"
#define PROFILE_FILE         "profile.json"

namespace {

const char* usage = R"(Sanbox.
Run one of the existing examples:
    - empty
    - renderer2d
    - triangle

Usage:
    sandbox [options] [-e <example>]
    sandbox (-h | --help)

Options:
    -h --help                   Show this help.
    -s --show-demo              Show GUI demo [default: false].
    -p --profiling              Enable profiling [default: false]
    -e --example <example>      Example [default: empty].
    -c --config <file>          Path to config.ini [default: examples/assets/config.ini]
)";

enum class LayerType : uint8_t
{
    NONE = 0,
    EMPTY,
    RENDERER_2D,
    TRIANGLE
};

struct ParseArgs {
    LayerType layer{LayerType::EMPTY};
    bool show_gui_demo{false};
    bool enable_profile{false};
    std::string config;
};

LayerType toLayerType(const std::string& example)
{
    static std::unordered_map<std::string, LayerType> example_to_layer{
        {EXAMPLE_EMPTY, LayerType::EMPTY},
        {EXAMPLE_RENDERER_2D, LayerType::RENDERER_2D},
        {EXAMPLE_TRIANGLE, LayerType::TRIANGLE}};

    return GE::toType(example_to_layer, example, LayerType::NONE);
}

ParseArgs parseArgs(int argc, char** argv)
{
    std::map<std::string, docopt::value> args;

    try {
        args = docopt::docopt_parse(usage, {argv + 1, argv + argc}, true);
    } catch (const docopt::DocoptExitHelp& e) {
        std::cout << usage << std::endl;
        exit(0);
    } catch (const docopt::DocoptArgumentError& e) {
        std::cout << usage << std::endl;
        exit(1);
    }

    ParseArgs parsed_args{};
    std::string example;

    try {
        example = args[EXAMPLE_OPT].asString();
        parsed_args.show_gui_demo = args[SHOW_GUI_OPT].asBool();
        parsed_args.enable_profile = args[PROFILE_OPT].asBool();
        parsed_args.config = args[CONFIG_OPT].asString();
    } catch (const std::exception& e) {
        std::cout << "Failed to parse arguments: " << e.what() << std::endl;
        exit(1);
    }

    parsed_args.layer = toLayerType(example);

    if (parsed_args.layer == LayerType::NONE) {
        std::cout << "Unknown example: " << example << "\n\n";
        std::cout << usage << std::endl;
        exit(1);
    }

    return parsed_args;
}

GE::Shared<GE::Layer> getLayer(const ParseArgs& args)
{
    using GE::Examples::GuiLayer;
    using GE::Examples::Renderer2DLayer;
    using GE::Examples::TriangleLayer;

    bool show_gui{args.show_gui_demo};

    switch (args.layer) {
        case LayerType::EMPTY: return GE::makeShared<GuiLayer>(show_gui);
        case LayerType::RENDERER_2D: return GE::makeShared<Renderer2DLayer>(show_gui);
        case LayerType::TRIANGLE: return GE::makeShared<TriangleLayer>(show_gui);
        default: break;
    }

    GE_ASSERT_MSG(false, "Unknown layer type: {}", static_cast<int>(args.layer));
    return nullptr;
}

} // namespace

int main(int argc, char** argv)
{
    ParseArgs args = parseArgs(argc, argv);

    if (args.enable_profile) {
        GE_PROFILE_ENABLE(true);
        GE_PROFILE_BEGIN_SESSION(PROFILE_SESSION_NAME, PROFILE_FILE);
    }

    if (!GE::Manager::initialize(args.config)) {
        return 1;
    }

    GE::Application::pushLayer(getLayer(args));
    GE::Application::run();

    return 0;
}
