#include "Actor.hpp"

#include "red_ball/graphics/ObjFileLoader.hpp"
#include "red_ball/graphics/BasicModel.hpp"

using namespace red_ball;
using namespace red_ball::game;

Actor::Actor(graphics::Direct3DDisplay* display) :
    renderingQueue_(display->renderingQueue())
{
    graphics::ObjFileLoader loader("red-ball-graphics/src/main/resources/cube.objfile");
    model_.reset(new graphics::BasicModel(&display->device(), loader));
    modelId_ = renderingQueue_->add(model_);
}

Actor::~Actor() {
    renderingQueue_->remove(modelId_);
}

void Actor::update(float dt) {

}
