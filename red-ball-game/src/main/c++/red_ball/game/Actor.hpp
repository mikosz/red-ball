#ifndef RED_BALL_GAME_ACTOR_HPP_
#define RED_BALL_GAME_ACTOR_HPP_

#include <boost/shared_ptr.hpp>

#include <d3d11.h>

#include "red_ball/graphics/Model.hpp"
#include "red_ball/graphics/RenderingQueue.hpp"
#include "red_ball/graphics/Direct3DDisplay.hpp"

namespace red_ball {
namespace game {

class Actor {
public:

    Actor(graphics::Direct3DDisplay* display);

    ~Actor();

    void update(float dt);

private:

    boost::shared_ptr<graphics::RenderingQueue> renderingQueue_;

    boost::shared_ptr<graphics::Model> model_;

    graphics::RenderingQueue::ModelId modelId_;

};

} // namespace game
} // namespace red_ball

#endif /* RED_BALL_GAME_ACTOR_HPP_ */
