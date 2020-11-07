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

#include "editor_layer.h"

#include <ge/debug/profile.h>
#include <ge/ge.h>

#include <docopt.h>

#include <iostream>

namespace {

constexpr auto USAGE = R"(Level Editor

Usage:
    level_editor [options] -c <config-file>
    level_editor (-h | --help)

Options:
    -h, --help                  Show this help.
    -p, --profile-file <path>   Enable profiling. [default: empty]
    -c, --config-file <path>    Path to config file.
)";

const auto OPT_PROFILE_FILE = "--profile-file";
const auto OPT_CONFIG_FILE = "--config-file";

const auto PROFILE_SESSION_NAME = "Level Editor Profiling";

struct app_args_t {
    std::string profile_file;
    std::string config_file;
};

app_args_t parseArgs(int argc, char** argv)
{
    std::map<std::string, docopt::value> args;

    try {
        args = docopt::docopt_parse(USAGE, {argv + 1, argv + argc}, true);
    } catch (const docopt::DocoptExitHelp& e) {
        std::cout << USAGE << std::endl;
        exit(EXIT_SUCCESS);
    } catch (const docopt::DocoptArgumentError& e) {
        std::cout << USAGE << std::endl;
        exit(EXIT_FAILURE);
    }

    app_args_t app_args{};

    try {
        app_args.profile_file = args[OPT_PROFILE_FILE].asString();
        app_args.config_file = args[OPT_CONFIG_FILE].asString();
    } catch (const std::exception& e) {
        std::cout << "Failed to parse arguments: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return app_args;
}

} // namespace

int main(int argc, char** argv)
{
    app_args_t args = parseArgs(argc, argv);

    if (!args.profile_file.empty()) {
        GE_PROFILE_ENABLE(true);
        GE_PROFILE_BEGIN_SESSION(PROFILE_SESSION_NAME, args.profile_file);
    }

    if (!GE::Manager::initialize(args.config_file)) {
        return EXIT_FAILURE;
    }

    GE::Application::pushLayer(GE::makeShared<LE::EditorLayer>());
    GE::Application::run();

    return EXIT_SUCCESS;
}
