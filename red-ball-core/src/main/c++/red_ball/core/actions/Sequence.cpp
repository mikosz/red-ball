#include "Sequence.hpp"

#include "red_ball/utils/pointee.hpp"

using namespace red_ball;
using namespace red_ball::core::actions;

bool Sequence::act(utils::Timer::Seconds* timeLeftPtr) {
    utils::Timer::Seconds& timeLeft = utils::pointee(timeLeftPtr);

    while (!actions_.empty() && actions_.front()->act(&timeLeft)) {
        actions_.pop();
    }

    return actions_.empty();
}

void Sequence::enqueue(ActionPtr action) {
    actions_.push(action);
}
