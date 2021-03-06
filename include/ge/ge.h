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

#ifndef GE_GE_H_
#define GE_GE_H_

#include <ge/app_properties.h>
#include <ge/application.h>
#include <ge/empty_layer.h>
#include <ge/layer.h>
#include <ge/layer_stack.h>
#include <ge/manager.h>

#include <ge/core/asserts.h>
#include <ge/core/begin.h>
#include <ge/core/interface.h>
#include <ge/core/log.h>
#include <ge/core/non_copyable.h>
#include <ge/core/timestamp.h>
#include <ge/core/utils.h>

#include <ge/ecs/camera_controller_script.h>
#include <ge/ecs/components.h>
#include <ge/ecs/entity.h>
#include <ge/ecs/scene.h>
#include <ge/ecs/scene_camera.h>
#include <ge/ecs/scriptable_entity.h>

#include <ge/gui/gui.h>

#include <ge/renderer/buffer_layout.h>
#include <ge/renderer/buffers.h>
#include <ge/renderer/framebuffer.h>
#include <ge/renderer/graphics_context.h>
#include <ge/renderer/ortho_camera_controller.h>
#include <ge/renderer/orthographic_camera.h>
#include <ge/renderer/render_command.h>
#include <ge/renderer/renderer.h>
#include <ge/renderer/renderer_2d.h>
#include <ge/renderer/renderer_api.h>
#include <ge/renderer/shader.h>
#include <ge/renderer/shader_program.h>
#include <ge/renderer/texture.h>
#include <ge/renderer/vertex_array.h>

#include <ge/window/input.h>
#include <ge/window/key_codes.h>
#include <ge/window/key_event.h>
#include <ge/window/mouse_button_codes.h>
#include <ge/window/mouse_event.h>
#include <ge/window/window.h>
#include <ge/window/window_event.h>

#endif // GE_GE_H_
