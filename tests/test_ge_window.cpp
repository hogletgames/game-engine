#include <ge/ge.h>

#include "gtest/gtest.h"

namespace {

TEST(EventTest, Key)
{
    constexpr uint16_t pressed_key_code{12};
    constexpr uint32_t repeat_count{49};
    constexpr uint16_t released_key_code{62};

    GE::KeyPressedEvent key_pressed{pressed_key_code, repeat_count};
    GE::KeyReleasedEvent key_released{released_key_code};

    EXPECT_EQ(GE::KeyPressedEvent::getStaticType(), GE::Event::Type::KEY_PRESSED);
    EXPECT_EQ(GE::KeyReleasedEvent::getStaticType(), GE::Event::Type::KEY_RELEASED);

    EXPECT_EQ(key_pressed.getType(), GE::Event::Type::KEY_PRESSED);
    EXPECT_EQ(key_released.getType(), GE::Event::Type::KEY_RELEASED);

    EXPECT_EQ(key_pressed.getKeyCode(), pressed_key_code);
    EXPECT_EQ(key_pressed.getRepeatCount(), repeat_count);
    EXPECT_EQ(key_released.getKeyCode(), released_key_code);
}

TEST(EventTest, Mouse)
{
    constexpr float mouse_pos_x{78.0f};
    constexpr float mouse_pos_y{46.0f};
    constexpr float mouse_offset_x{53.0f};
    constexpr float mouse_offset_y{90.0f};
    constexpr uint8_t pressed_button{1};
    constexpr uint8_t released_button{3};

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

    EXPECT_EQ(GE::WindowResizedEvent::getStaticType(), GE::Event::Type::WINDOW_RESIZED);
    EXPECT_EQ(GE::WindowClosedEvent::getStaticType(), GE::Event::Type::WINDOW_CLOSED);

    EXPECT_EQ(win_resized.getType(), GE::Event::Type::WINDOW_RESIZED);
    EXPECT_EQ(win_closed.getType(), GE::Event::Type::WINDOW_CLOSED);

    EXPECT_EQ(win_resized.getWidth(), win_resize_width);
    EXPECT_EQ(win_resized.getHeight(), win_resize_height);
}

template<typename EventType>
class EventDispatcherTest: public ::testing::Test
{};

using EventTypeList = ::testing::Types<
    // Key events
    GE::KeyPressedEvent, GE::KeyReleasedEvent,
    // Mouse events
    GE::MouseMovedEvent, GE::MouseScrolledEvent, GE::MouseButtonPressedEvent,
    GE::MouseButtonReleasedEvent,
    // Window events
    GE::WindowResizedEvent, GE::WindowClosedEvent>;

TYPED_TEST_SUITE(EventDispatcherTest, EventTypeList);

TYPED_TEST(EventDispatcherTest, SuccessfulDispatch)
{
    TypeParam event{};
    GE::EventDispatcher dispatcher{event};
    GE::EventCallback<TypeParam> callback = [](auto&) { return true; };

    EXPECT_FALSE(event.handled());
    EXPECT_TRUE(dispatcher.dispatch(callback));
    EXPECT_TRUE(event.handled());
}

} // namespace
