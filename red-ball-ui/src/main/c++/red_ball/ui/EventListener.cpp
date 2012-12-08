#include "EventListener.hpp"

using namespace red_ball::ui;

EventListener::EventListener() :
        lastListenerId_(0) {
}

EventListener::ListenerId EventListener::registerListener(EventType eventType, Callback callback) {
    CallbacksByEventType::iterator byEventTypeIt = callbacksByEventType_.insert(std::make_pair(eventType, callback));
    CallbacksById::iterator byIdIt = callbacksById_.insert(std::make_pair(++lastListenerId_, byEventTypeIt)).first;
    return byIdIt->first;
}

void EventListener::unregisterListener(ListenerId listenerId) {
    CallbacksById::iterator byIdIt = callbacksById_.find(listenerId);
    if (byIdIt == callbacksById_.end()) {
        throw ListenerNotRegistered();
    }

    callbacksByEventType_.erase(byIdIt->second);
    callbacksById_.erase(byIdIt);
}

bool EventListener::handle(EventType eventType, const Event& event) const {
    std::pair<CallbacksByEventType::const_iterator, CallbacksByEventType::const_iterator> range = callbacksByEventType_.equal_range(eventType);
    while (range.first != range.second) {
        if ((range.first->second)(event)) {
            return true;
        }
        ++range.first;
    }
    return false;
}
