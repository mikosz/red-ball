#include "RenderingQueue.hpp"

#include <algorithm>

#include <boost/bind.hpp>

#include "red_ball/utils/pointee.hpp"
#include "MatrixBuffer.hpp"

using namespace red_ball::graphics;

void RenderingQueue::render(ID3D11DeviceContext* context, MatrixBuffer* worldMatrixBufferPtr) const {
    MatrixBuffer& worldMatrixBuffer = utils::pointee(worldMatrixBufferPtr);

    std::for_each(
            models_.begin(),
            models_.end(),
            boost::bind(&Model::render, _1, context, &worldMatrixBuffer)
    );
}

RenderingQueue::ModelId RenderingQueue::add(boost::shared_ptr<Model> model) {
    models_.push_front(model);
    return models_.begin();
}

void RenderingQueue::remove(ModelId modelId) {
    models_.erase(modelId);
}
