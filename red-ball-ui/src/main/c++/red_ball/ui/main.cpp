#include <string>

#include "red_ball/ui/UI.hpp"

namespace red_ball {
namespace ui {

int redBallMain(HINSTANCE hInstance, int cmdShow) {
    try {
        UI userInterface(hInstance, cmdShow);
        return userInterface.loop();
    } catch (const std::exception& e) {
        MessageBox(0, ("Caught error: " + std::string(e.what())).c_str(), "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}

}  // namespace ui
}  // namespace red_ball

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int cmdShow) {
    return red_ball::ui::redBallMain(hInstance, cmdShow);
}
