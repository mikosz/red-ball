#ifndef RED_BALL_CORE_ACTIONS_CHANGE_HPP_
#define RED_BALL_CORE_ACTIONS_CHANGE_HPP_

#include <iostream>

#include <boost/function.hpp>

#include "Action.hpp"

namespace red_ball {
namespace core {
namespace actions {

template <class T>
class Change : public Action {
public:

    typedef boost::function<void (T)> Changer;

    Change(T changePerSecond, T targetChange, Changer changer) :
        changePerSecond_(changePerSecond),
        changeRemaining_(targetChange),
        changer_(changer)
    {
    }

    bool act(utils::Timer::Seconds time) {
        T change = std::min(changeRemaining_, changePerSecond_ * time);
        changer_(change);
        changeRemaining_ -= change;
        return changeRemaining_ <= 0;
    }

private:

    T changePerSecond_;

    T changeRemaining_;

    Changer changer_;

};

} // namespace actions
} // namespace core
} // namespace red_ball

#endif /* RED_BALL_CORE_ACTIONS_CHANGE_HPP_ */
