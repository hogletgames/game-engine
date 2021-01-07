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

#include "ge/window/key_event.h"
#include "ge/window/mouse_event.h"
#include "ge/window/window_event.h"

#include <gtest/gtest.h>

namespace GE {

void PrintTo(GE::KeyCode key_code, std::ostream* os)
{
    *os << key_code;
}

void PrintTo(GE::MouseButton button, std::ostream* os)
{
    *os << button;
}

} // namespace GE

namespace {

TEST(EventTest, Key)
{
    constexpr GE::KeyCode pressed_key_code{GE_KEY_LALT};
    constexpr uint32_t repeat_count{49};
    constexpr GE::KeyCode released_key_code{GE_KEY_S};
    const char* typed_text = "key typed event test";

    GE::KeyPressedEvent key_pressed{pressed_key_code, repeat_count};
    GE::KeyReleasedEvent key_released{released_key_code};
    GE::KeyTypedEvent key_typed{typed_text};

    EXPECT_EQ(GE::KeyPressedEvent::getStaticType(), GE::Event::Type::KEY_PRESSED);
    EXPECT_EQ(GE::KeyReleasedEvent::getStaticType(), GE::Event::Type::KEY_RELEASED);
    EXPECT_EQ(GE::KeyTypedEvent::getStaticType(), GE::Event::Type::KEY_TYPED);

    EXPECT_EQ(key_pressed.getType(), GE::Event::Type::KEY_PRESSED);
    EXPECT_EQ(key_released.getType(), GE::Event::Type::KEY_RELEASED);
    EXPECT_EQ(key_typed.getType(), GE::Event::Type::KEY_TYPED);

    EXPECT_EQ(key_pressed.getKeyCode(), pressed_key_code);
    EXPECT_EQ(key_pressed.getRepeatCount(), repeat_count);
    EXPECT_EQ(key_released.getKeyCode(), released_key_code);
    EXPECT_STREQ(key_typed.getText(), typed_text);
}

TEST(EventTest, Mouse)
{
    constexpr float mouse_pos_x{78.0f};
    constexpr float mouse_pos_y{46.0f};
    constexpr float mouse_offset_x{53.0f};
    constexpr float mouse_offset_y{90.0f};
    constexpr GE::MouseButton pressed_button{GE_BUTTON_LEFT};
    constexpr GE::MouseButton released_button{GE_BUTTON_MIDDLE};

    GE::MouseMovedEvent mouse_moved{mouse_pos_x, mouse_pos_y};
    GE::MouseScrolledEvent mouse_scrolled{mouse_offset_x, mouse_offset_y};
    GE::MouseButtonPressedEvent mouse_pressed{pressed_button};
    GE::MouseButtonReleasedEvent mouse_released{released_button};

    EXPECT_EQ(GE::MouseMovedEvent::getStaticType(), GE::Event::Type::MOUSE_MOVED);
    EXPECT_EQ(GE::MouseScrolledEvent::getStaticType(), GE::Event::Type::MOUSE_SCROLLED);
    EXPECT_EQ(GE::MouseButtonPressedEvent::getStaticType(),
              GE::Event::Type::MOUSE_BUTTON_PRESSED);
    EXPECT_EQ(GE::MouseButtonReleasedEvent::getStaticType(),
              GE::Event::Type::MOUSE_BUTTON_RELEASED);

    EXPECT_EQ(mouse_moved.getType(), GE::Event::Type::MOUSE_MOVED);
    EXPECT_EQ(mouse_scrolled.getType(), GE::Event::Type::MOUSE_SCROLLED);
    EXPECT_EQ(mouse_pressed.getType(), GE::Event::Type::MOUSE_BUTTON_PRESSED);
    EXPECT_EQ(mouse_released.getType(), GE::Event::Type::MOUSE_BUTTON_RELEASED);

    EXPECT_EQ(mouse_moved.getPosX(), mouse_pos_x);
    EXPECT_EQ(mouse_moved.getPosY(), mouse_pos_y);
    EXPECT_EQ(mouse_scrolled.getOffsetX(), mouse_offset_x);
    EXPECT_EQ(mouse_scrolled.getOffsetY(), mouse_offset_y);
    EXPECT_EQ(mouse_pressed.getMouseButton(), pressed_button);
    EXPECT_EQ(mouse_released.getMouseButton(), released_button);
}

TEST(EventTest, Window)
{
    constexpr uint32_t win_resize_width{320};
    constexpr uint32_t win_resize_height{240};

    GE::WindowResizedEvent win_resized{win_resize_width, win_resize_height};
    GE::WindowClosedEvent win_closed{};
    GE::WindowMaximizedEvent win_max{};
    GE::WindowMinimizedEvent win_min{};
    GE::WindowRestoredEvent win_restored{};

    EXPECT_EQ(GE::WindowResizedEvent::getStaticType(), GE::Event::Type::WINDOW_RESIZED);
    EXPECT_EQ(GE::WindowClosedEvent::getStaticType(), GE::Event::Type::WINDOW_CLOSED);
    EXPECT_EQ(GE::WindowMaximizedEvent::getStaticType(),
              GE::Event::Type::WINDOW_MAXIMIZED);
    EXPECT_EQ(GE::WindowMinimizedEvent::getStaticType(),
              GE::Event::Type::WINDOW_MINIMIZED);
    EXPECT_EQ(GE::WindowRestoredEvent::getStaticType(), GE::Event::Type::WINDOW_RESTORED);

    EXPECT_EQ(win_resized.getType(), GE::Event::Type::WINDOW_RESIZED);
    EXPECT_EQ(win_closed.getType(), GE::Event::Type::WINDOW_CLOSED);
    EXPECT_EQ(win_max.getType(), GE::Event::Type::WINDOW_MAXIMIZED);
    EXPECT_EQ(win_min.getType(), GE::Event::Type::WINDOW_MINIMIZED);
    EXPECT_EQ(win_restored.getType(), GE::Event::Type::WINDOW_RESTORED);

    EXPECT_EQ(win_resized.getWidth(), win_resize_width);
    EXPECT_EQ(win_resized.getHeight(), win_resize_height);
}

template<typename EventType>
class EventDispatcherTest: public ::testing::Test
{};

using EventTypeList = ::testing::Types<
    // Key events
    GE::KeyPressedEvent, GE::KeyReleasedEvent, GE::KeyTypedEvent,
    // Mouse events
    GE::MouseMovedEvent, GE::MouseScrolledEvent, GE::MouseButtonPressedEvent,
    GE::MouseButtonReleasedEvent,
    // Window events
    GE::WindowResizedEvent, GE::WindowClosedEvent, GE::WindowMaximizedEvent,
    GE::WindowMinimizedEvent, GE::WindowRestoredEvent>;

TYPED_TEST_SUITE(EventDispatcherTest, EventTypeList);

TYPED_TEST(EventDispatcherTest, SuccessfulDispatch)
{
    TypeParam event{};
    GE::EventDispatcher dispatcher{&event};
    GE::EventCallback<TypeParam> callback = [](const TypeParam&) { return true; };

    EXPECT_FALSE(event.handled());
    EXPECT_TRUE(dispatcher.dispatch(callback));
    EXPECT_TRUE(event.handled());
}

} // namespace
