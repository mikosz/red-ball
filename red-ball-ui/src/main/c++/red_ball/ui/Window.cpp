#include "Window.hpp"

#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include "red_ball/core/actions/Change.hpp"
#include "red_ball/core/actions/Sequence.hpp"

using namespace red_ball::ui;

void Window::registerWindowClass(const std::string& windowClass, HINSTANCE hInstance) {
    WNDCLASSEX wndClass;
    ZeroMemory(&wndClass, sizeof(wndClass));
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = &callbackWindowProc;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = 0;
    wndClass.lpszClassName = windowClass.c_str();

    if (!RegisterClassEx(&wndClass)) {
        throw WindowClassRegisteringFailed();
    }
}

Window::Window(const std::string& windowClass, HINSTANCE hInstance, int cmdShow) {
    RECT rc = { 0, 0, 800, 600 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    hWnd_ = CreateWindowA(windowClass.c_str(), "RedBall", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInstance, this);

    if (!hWnd_) {
        throw WindowCreationFailed();
    }

    ShowWindow(hWnd_, cmdShow);

    RECT dimensions;
    GetClientRect(hWnd_, &dimensions);
    clientWidth_ = dimensions.right - dimensions.left;
    clientHeight_ = dimensions.bottom - dimensions.top;

    display_.reset(new graphics::Direct3DDisplay(hWnd_));

    boost::shared_ptr<game::Actor> actor(new game::Actor(display_.get()));
    game_.addActor(actor);

    core::actions::ActionPtr rotateY(
            new core::actions::Change<float>(
                    3.14f,
                    31.4f,
                    boost::bind(&graphics::Model::rotateBy, boost::ref(actor->model()), 0.0f, _1, 0.0f)
                    )
            );
    core::actions::ActionPtr up(
            new core::actions::Change<float>(
                    1.0f,
                    3.0f,
                    boost::bind(&graphics::Model::translateBy, boost::ref(actor->model()), 0.0f, _1, 0.0f)
                    )
            );
    core::actions::ActionPtr down(
            new core::actions::Change<float>(
                    -1.0f,
                    -3.0f,
                    boost::bind(&graphics::Model::translateBy, boost::ref(actor->model()), 0.0f, _1, 0.0f)
                    )
            );

    boost::shared_ptr<core::actions::Sequence> moveAround(new core::actions::Sequence);
    moveAround->enqueue(rotateY);
    moveAround->enqueue(up);
    moveAround->enqueue(down);

    actor->scheduleAction(3.0f, moveAround);

    core::actions::ActionPtr rotateX(
            new core::actions::Change<float>(
                    3.14f / 2,
                    31.4f / 2,
                    boost::bind(&graphics::Model::rotateBy, boost::ref(actor->model()), _1, 0.0f, 0.0f)
                    )
            );
    actor->scheduleAction(5.0f, rotateX);
}

Window::~Window() {
    DestroyWindow(hWnd_);
}

void Window::render() {
    game_.update();
    display_->render();
}

LRESULT CALLBACK Window::callbackWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* instance = reinterpret_cast<Window*> (GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (!instance) {
        if (message == WM_NCCREATE) {
            LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT> (lParam);
            instance = reinterpret_cast<Window*> (createStruct->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR> (instance));
        } else {
            throw WindowInstanceMissing();
        }
    }

    if (instance->eventListener_.handle(message, EventListener::Event(wParam, lParam))) {
        return 0;
    } else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
