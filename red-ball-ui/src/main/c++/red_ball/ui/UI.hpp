#ifndef RED_BALL_UI_UI_HPP_
#define RED_BALL_UI_UI_HPP_

#include <boost/scoped_ptr.hpp>

#include "Window.hpp"

namespace red_ball {
namespace ui {

class UI {
public:

    UI(HINSTANCE hInstance, int cmdShow);

    int loop();

private:

    static const std::string MAIN_WINDOW_CLASS_NAME;

    bool quit_;

    boost::scoped_ptr<Window> window_;

    void dispatchMessages() const;

    bool onDestroy(const EventListener::Event& event);

};

}  // namespace ui
}  // namespace red_ball

#endif /* RED_BALL_UI_UI_HPP_ */
