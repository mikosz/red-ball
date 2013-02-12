#ifndef RED_BALL_CORE_ACTIONS_TIMELINE_HPP_
#define RED_BALL_CORE_ACTIONS_TIMELINE_HPP_

#include <map>

#include "Action.hpp"

namespace red_ball {
namespace core {
namespace actions {

class Timeline {
public:

    typedef std::multimap<utils::Timer::Seconds, ActionPtr> Schedule;

    typedef Schedule::iterator TimelineId;

    Timeline();

    void update(utils::Timer::Seconds time);

    TimelineId schedule(utils::Timer::Seconds delay, ActionPtr action);

private:

    utils::Timer::Seconds time_;

    Schedule timeline_;

};

} // namespace actions
} // namespace core
} // namespace red_ball

#endif /* RED_BALL_CORE_ACTIONS_TIMELINE_HPP_ */
