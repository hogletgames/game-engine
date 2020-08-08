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

#include "triangle_layer.h"

#include <ge/debug/profile.h>
#include <ge/ge.h>

#include <docopt.h>

#include <iostream>

#define EXAMPLE_OPT  "--example"
#define SHOW_GUI_OPT "--show-demo"
#define PROFILE_OPT  "--profiling"

#define EXAMPLE_EMPTY    "empty"
#define EXAMPLE_TRIANGLE "triangle"

namespace {

const char* usage = R"(Sanbox.
Run one of the existing examples:
    - empty
    - triangle

Usage:
    sandbox [options] [-e <example>]
    sandbox (-h | --help)

Options:
    -h --help                   Show this help.
    -s --show-demo              Show GUI demo [default: false].
    -p --profiling              Enable profiling [default: false]
    -e --example <example>      Example [default: empty].
)";

enum class LayerType : uint8_t
{
    EMPTY = 0,
    TRIANGLE
};

struct ParseArgs {
    LayerType layer{LayerType::EMPTY};
    bool show_gui_demo{false};
    bool enable_profile{false};
};

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
    std::string example = args[EXAMPLE_OPT].asString();

    if (example == EXAMPLE_EMPTY) {
        parsed_args.layer = LayerType::EMPTY;
    } else if (example == EXAMPLE_TRIANGLE) {
        parsed_args.layer = LayerType::TRIANGLE;
    } else {
        std::cout << "Unknown example: " << example << "\n\n";
        std::cout << usage << std::endl;
        exit(1);
    }

    parsed_args.show_gui_demo = args[SHOW_GUI_OPT].asBool();
    parsed_args.enable_profile = args[PROFILE_OPT].asBool();

    return parsed_args;
}

GE::Shared<GE::Layer> getLayer(const ParseArgs& args)
{
    using GE::Examples::GuiLayer;
    using GE::Examples::TriangleLayer;

    bool show_gui{args.show_gui_demo};

    switch (args.layer) {
        case LayerType::EMPTY: return GE::makeShared<GuiLayer>(show_gui);
        case LayerType::TRIANGLE: return GE::makeShared<TriangleLayer>(show_gui);
        default: break;
    }

    GE_ASSERT_MSG(false, "Unknown layer type: {}", static_cast<int>(args.layer));
    return nullptr;
}

} // namespace

int main(int argc, char** argv) // NOLINT
{
    ParseArgs args = parseArgs(argc, argv);

    if (args.enable_profile) {
        GE_PROFILE_ENABLE(true);
        GE_PROFILE_BEGIN_SESSION("Sandbox Run", "profile.json");
    }

    if (!GE::Manager::initialize(GE_OPEN_GL_API)) {
        return 1;
    }

    GE::Application app{};
    app.pushLayer(getLayer(args));
    app.run();
    return 0;
}
