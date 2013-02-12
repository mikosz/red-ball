#ifndef RED_BALL_CORE_ACTIONS_ACTION_HPP_
#define RED_BALL_CORE_ACTIONS_ACTION_HPP_

#include <boost/shared_ptr.hpp>

#include "red_ball/utils/Timer.hpp"

namespace red_ball {
namespace core {
namespace actions {

class Action {
public:

    virtual ~Action() {
    }

    virtual bool act(utils::Timer::Seconds time) = 0;

};

typedef boost::shared_ptr<Action> ActionPtr;

} // namespace actions
} // namespace core
} // namespace red_ball

#endif /* RED_BALL_CORE_ACTIONS_ACTION_HPP_ */
