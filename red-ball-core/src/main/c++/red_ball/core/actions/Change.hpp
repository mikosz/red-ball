#ifndef RED_BALL_CORE_ACTIONS_CHANGE_HPP_
#define RED_BALL_CORE_ACTIONS_CHANGE_HPP_

#include <algorithm>

#include <boost/function.hpp>

#include "red_ball/utils/pointee.hpp"
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
        changeRemaining_(std::abs(targetChange)),
        changer_(changer)
    {
    }

    bool act(utils::Timer::Seconds* timeLeftPtr) {
        utils::Timer::Seconds& timeLeft = utils::pointee(timeLeftPtr);

        T change = changePerSecond_ * timeLeft;
        if (std::abs(changeRemaining_) < std::abs(change)) {
            change = changeRemaining_;
        }
        timeLeft -= change / changePerSecond_;
        changer_(change);
        if (changeRemaining_ <= std::abs(change)) {
            changeRemaining_ = 0;
        } else {
            changeRemaining_ -= std::abs(change);
        }
        return changeRemaining_ == 0;
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
