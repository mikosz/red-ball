#ifndef RED_BALL_CORE_ACTIONS_SEQUENCE_HPP_
#define RED_BALL_CORE_ACTIONS_SEQUENCE_HPP_

#include <queue>

#include "Action.hpp"

namespace red_ball {
namespace core {
namespace actions {

class Sequence : public Action {
public:

    bool act(utils::Timer::Seconds* timeLeft);

    void enqueue(ActionPtr action);

private:

    typedef std::queue<ActionPtr> Actions;

    Actions actions_;

};

} // namespace actions
} // namespace core
} // namespace red_ball

#endif /* RED_BALL_CORE_ACTIONS_SEQUENCE_HPP_ */
