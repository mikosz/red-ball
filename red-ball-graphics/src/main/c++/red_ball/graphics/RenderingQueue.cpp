#include "RenderingQueue.hpp"

#include <algorithm>

#include <boost/bind.hpp>

#include "red_ball/utils/pointee.hpp"
#include "GraphicsContext.hpp"
#include "MatrixBuffer.hpp"
#include "Shader.hpp"

using namespace red_ball;
using namespace red_ball::graphics;

namespace {

void renderModel(GraphicsContext* graphicsContextPtr, Shader* shaderPtr, boost::shared_ptr<Model> model) {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);
    Shader& shader = utils::pointee(shaderPtr);

    shader.bindWorldMatrix(&graphicsContext, model->worldMatrix());
    model->render(&graphicsContext.deviceContext());
}

} // anonymous namespace

void RenderingQueue::render(GraphicsContext* graphicsContextPtr, Shader* shaderPtr) const {
    GraphicsContext& graphicsContext = utils::pointee(graphicsContextPtr);
    Shader& shader = utils::pointee(shaderPtr);

    std::for_each(
            models_.begin(),
            models_.end(),
            boost::bind(&renderModel, &graphicsContext, &shader, _1)
    );
}

RenderingQueue::ModelId RenderingQueue::add(boost::shared_ptr<Model> model) {
    models_.push_front(model);
    return models_.begin();
}

void RenderingQueue::remove(ModelId modelId) {
    models_.erase(modelId);
}
