#ifndef RED_BALL_UI_WINDOW_HPP_
#define RED_BALL_UI_WINDOW_HPP_

#include <stdexcept>

#include <boost/scoped_ptr.hpp>


#include "EventListener.hpp"
#include "red_ball/graphics/GraphicsContext.hpp"
#include "red_ball/graphics/A3DView.hpp"
#include "red_ball/game/Game.hpp"
#include "red_ball/windows/windows.hpp"

namespace red_ball {
namespace ui {

class Window {
public:

    static void registerWindowClass(const std::string& windowClass, HINSTANCE hInstance);

    Window(const std::string& windowClass, HINSTANCE hInstance, int cmdShow);

    ~Window();

    void render();

    HWND hWnd() const {
        return hWnd_;
    }

    size_t clientWidth() const {
        return clientWidth_;
    }

    size_t clientHeight() const {
        return clientHeight_;
    }

    EventListener& eventListener() {
        return eventListener_;
    }

private:

    static LRESULT CALLBACK callbackWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    LRESULT windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HWND hWnd_;

    size_t clientWidth_;

    size_t clientHeight_;

    EventListener eventListener_;

    boost::scoped_ptr<graphics::GraphicsContext> graphicsContext_;

    boost::scoped_ptr<graphics::A3DView> display_;

    game::Game game_;

};

class WindowClassRegisteringFailed : public std::runtime_error {
public:

    WindowClassRegisteringFailed() :
        std::runtime_error("Failed to register window class") {
    }

};

class WindowCreationFailed : public std::runtime_error {
public:

    WindowCreationFailed() :
        std::runtime_error("Failed to create window") {
    }

};

class WindowInstanceMissing : public std::logic_error {
public:

    WindowInstanceMissing() :
        std::logic_error("Window instance not found") {
    }

};

}  // namespace ui
}  // namespace red_ball

#endif /* RED_BALL_SYSTEM_WINDOW_HPP_ */
