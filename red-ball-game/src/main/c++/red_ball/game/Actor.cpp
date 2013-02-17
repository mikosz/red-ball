#include "Actor.hpp"

#include "red_ball/graphics/ObjFileLoader.hpp"
#include "red_ball/graphics/BasicModel.hpp"
#include "red_ball/utils/pointee.hpp"

using namespace red_ball;
using namespace red_ball::game;

Actor::Actor(graphics::GraphicsContext* graphicsContextPtr) :
    renderingQueue_(graphicsContextPtr->renderingQueue())
{
    graphics::GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);

    graphics::ObjFileLoader loader("red-ball-graphics/src/main/resources/cube.objfile");
    model_.reset(new graphics::BasicModel(&graphicsContext.device(), loader));
    modelId_ = renderingQueue_->add(model_);
}

Actor::~Actor() {
    renderingQueue_->remove(modelId_);
}

void Actor::update(utils::Timer::Seconds time) {
    actionTimeline_.update(time);
}

void Actor::scheduleAction(utils::Timer::Seconds delay, core::actions::ActionPtr action) {
    actionTimeline_.schedule(delay, action);
}
