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

#include "app_properties.h"

#include "ge/core/asserts.h"
#include "ge/core/log.h"
#include "ge/debug/profile.h"
#include "ge/renderer/renderer.h"

#include <boost/property_tree/ini_parser.hpp>

#include <filesystem>
#include <fstream>

#define PROP_RENDER_API    "general.render_api"
#define PROP_CORE_LOGLVL   "general.core_loglvl"
#define PROP_CLIENT_LOGLVL "general.client_loglvl"
#define PROP_ASSETS_DIR    "general.assets_dir"

namespace {

bool createFileIfNotExist(const std::string& filename)
{
    if (std::filesystem::exists(filename)) {
        return true;
    }

    std::ofstream file(filename.data());
    return file.is_open();
}

template<typename T>
std::string getPropString(const boost::property_tree::ptree& ptree,
                          const std::string& path, const T& def)
{
    return ptree.get<std::string>(path, GE::toString(def));
}

void dumpProperties([[maybe_unused]] const GE::AppProperties::properties_t& props)
{
    GE_CORE_INFO("Render API: {}", GE::toString(props.api));
    GE_CORE_INFO("Core log level: {}", GE::toString(props.core_log_lvl));
    GE_CORE_INFO("Client log level: {}", GE::toString(props.client_log_lvl));
    GE_CORE_INFO("Assets directory: {}", props.assets_dir);
}

} // namespace

namespace GE {

bool AppProperties::read(const std::string& filename, properties_t* props)
{
    GE_PROFILE_FUNC();
    GE_CORE_INFO("Config file: '{}'", filename);

    if (!createFileIfNotExist(filename)) {
        GE_CORE_ERR("Failed to open properties file: '{}'", filename);
        return false;
    }

    boost::property_tree::ptree ptree;

    try {
        boost::property_tree::ini_parser::read_ini(filename, ptree);
    } catch (const std::exception& e) {
        GE_CORE_ERR("Failed to load properties from '{}': {}", filename, e.what());
        return false;
    }

    std::string render_api_str = getPropString(ptree, PROP_RENDER_API, GE_OPEN_GL_API);
    std::string core_log_lvl = getPropString(ptree, PROP_CORE_LOGLVL, GE_LOGLVL_INFO);
    std::string client_log_lvl = getPropString(ptree, PROP_CLIENT_LOGLVL, GE_LOGLVL_INFO);

    props->api = toRendAPI(render_api_str);
    props->core_log_lvl = toLogLvl(core_log_lvl);
    props->client_log_lvl = toLogLvl(client_log_lvl);
    props->assets_dir = ptree.get<std::string>(PROP_ASSETS_DIR, GE_ASSETS_DIR);

    GE_CORE_INFO("Reading app properties: Succeed", filename);
    dumpProperties(*props);

    return true;
}

bool AppProperties::write(const std::string& filename, const properties_t& props)
{
    GE_PROFILE_FUNC();
    GE_CORE_INFO("Config file: '{}'", filename);

    boost::property_tree::ptree ptree;

    GE_ASSERT(!filename.empty());

    try {
        ptree.put<std::string>(PROP_RENDER_API, toString(props.api));
        ptree.put<std::string>(PROP_CORE_LOGLVL, toString(props.core_log_lvl));
        ptree.put<std::string>(PROP_CLIENT_LOGLVL, toString(props.client_log_lvl));
        ptree.put<std::string>(PROP_ASSETS_DIR, props.assets_dir);

        boost::property_tree::ini_parser::write_ini(filename, ptree);
    } catch (const std::exception& e) {
        GE_CORE_ERR("Failed to write properties to '{}", filename);
        return false;
    }

    GE_CORE_INFO("Writing app properties: Succeed", filename);
    dumpProperties(props);

    return true;
}

} // namespace GE
