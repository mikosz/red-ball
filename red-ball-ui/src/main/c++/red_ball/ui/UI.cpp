#include "UI.hpp"

#include <boost/bind.hpp>

using namespace red_ball::ui;

const std::string UI::MAIN_WINDOW_CLASS_NAME("RedBallUIMainWindow");

UI::UI(HINSTANCE hInstance, int cmdShow) :
        quit_(false) {
    Window::registerWindowClass(MAIN_WINDOW_CLASS_NAME, hInstance);
    window_.reset(new Window(MAIN_WINDOW_CLASS_NAME, hInstance, cmdShow));
    window_->eventListener().registerListener(WM_DESTROY, boost::bind(&UI::onDestroy, this, _1));
}

int UI::loop() {
    while (!quit_) {
        dispatchMessages();
        window_->render();
    }

    return 0;
}

void UI::dispatchMessages() const {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool UI::onDestroy(const EventListener::Event&) {
    quit_ = true;
    PostQuitMessage(0);
    return true;
}
