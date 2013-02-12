#include "Timeline.hpp"

#include <algorithm>

#include <boost/bind.hpp>

using namespace red_ball;
using namespace red_ball::core::actions;

Timeline::Timeline() :
        time_(0)
{
}

void Timeline::update(utils::Timer::Seconds time) {
    time_ += time;
    Schedule::iterator it = timeline_.begin(), activeEnd = timeline_.upper_bound(time_);
    while (it != activeEnd) {
        if (it->second->act(time)) {
            timeline_.erase(it++);
        } else {
            ++it;
        }
    }
}

Timeline::TimelineId Timeline::schedule(utils::Timer::Seconds delay, ActionPtr action) {
    return timeline_.insert(std::make_pair(time_ + delay, action));
}
