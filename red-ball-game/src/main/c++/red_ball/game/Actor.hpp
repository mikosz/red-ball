#ifndef RED_BALL_GAME_ACTOR_HPP_
#define RED_BALL_GAME_ACTOR_HPP_

#include <boost/shared_ptr.hpp>

#include <d3d11.h>

#include "red_ball/core/actions/Timeline.hpp"
#include "red_ball/graphics/Model.hpp"
#include "red_ball/graphics/RenderingQueue.hpp"
#include "red_ball/graphics/GraphicsContext.hpp"
#include "red_ball/utils/Timer.hpp"

namespace red_ball {
namespace game {

class Actor {
public:

    Actor(graphics::GraphicsContext* graphicsContext);

    ~Actor();

    void update(utils::Timer::Seconds time);

    void scheduleAction(utils::Timer::Seconds delay, core::actions::ActionPtr action);

    graphics::Model& model() {
        return *model_;
    }

private:

    boost::shared_ptr<graphics::RenderingQueue> renderingQueue_;

    boost::shared_ptr<graphics::Model> model_;

    graphics::RenderingQueue::ModelId modelId_;

    core::actions::Timeline actionTimeline_;

};

} // namespace game
} // namespace red_ball

#endif /* RED_BALL_GAME_ACTOR_HPP_ */
