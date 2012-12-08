#ifndef RED_BALL_UI_EVENTLISTENER_HPP_
#define RED_BALL_UI_EVENTLISTENER_HPP_

#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

namespace red_ball {
namespace ui {

class EventListener {
public:

    struct Event {

        WPARAM wParam;

        LPARAM lParam;

        Event(WPARAM wParam_, LPARAM lParam_) :
            wParam(wParam_),
            lParam(lParam_) {
        }

    };

    typedef unsigned int EventType;

    typedef size_t ListenerId;

    typedef boost::function<bool (const Event&)> Callback;

    EventListener();

    ListenerId registerListener(EventType event, Callback callback);

    void unregisterListener(ListenerId listenerId);

    bool handle(EventType eventType, const Event& event) const;

private:

    typedef boost::unordered_multimap<EventType, Callback> CallbacksByEventType;

    typedef boost::unordered_map<ListenerId, CallbacksByEventType::iterator> CallbacksById;

    ListenerId lastListenerId_;

    CallbacksByEventType callbacksByEventType_;

    CallbacksById callbacksById_;

};

class ListenerNotRegistered : public std::logic_error {
public:

    ListenerNotRegistered() :
        std::logic_error("Access to a not registered listener id") {
    }

};

}  // namespace ui
}  // namespace red_ball

#endif /* RED_BALL_UI_EVENTLISTENER_HPP_ */
