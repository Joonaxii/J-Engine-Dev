#include <SimpleRenderer.h>
using namespace JEngine;

SimpleRenderer::SimpleRenderer() : _flip(0), _sprite(nullptr), _matrix(JMatrix4f::Identity), _layer(1) {}

SimpleRenderer::SimpleRenderer(JEngine::Sprite* sprite) : SimpleRenderer() {
    setTRS({ 0, 0 }, 0, { 1, 1 });
    setSprite(sprite);
}

void SimpleRenderer::setTRS(const JVector2f& position, const float rotation, const JVector2f& scale) {
    _matrix.setTRS(position, rotation, scale);
}


void SimpleRenderer::setFlipX(const bool flip) { _flip.setBit(FLIP_X, flip); }
const bool SimpleRenderer::getFlipX() const { return bool(_flip & FLIP_X); }

void SimpleRenderer::setFlipY(const bool flip) { _flip.setBit(FLIP_Y, flip); }
const bool SimpleRenderer::getFlipY() const { return bool(_flip & FLIP_Y); }


void SimpleRenderer::setSprite(JEngine::Sprite* sprite) {
    _sprite = sprite;
}

const bool SimpleRenderer::canRender() const {
    return IRenderer::canRender() && _sprite != nullptr;
}

void SimpleRenderer::setObjectLayer(const uint32_t layer) {
    _layer = layer;
}

const uint32_t SimpleRenderer::getObjectLayer() const {
    return _layer;
}

const int32_t SimpleRenderer::getVertexCount() const {
    return _sprite ? _sprite->getVertexCount() : 0;
}

const int32_t SimpleRenderer::getIndexCount() const {
    return _sprite ? _sprite->getIndexCount() : 0;
}

const JVertex2f* SimpleRenderer::getVertices() const {
    return _sprite ? _sprite->getVertices(_flip) : nullptr;
}

const uint32_t* SimpleRenderer::getIndices() const {
    return _sprite ? _sprite->getIndices() : nullptr;
}

const JMatrix4f& SimpleRenderer::getWorldMatrix() const {
    return _matrix;
}

void SimpleRenderer::updateRenderer() {
    IRenderer::updateRenderer();
}

const JRectf& SimpleRenderer::getLocalBounds() const {
    return _sprite ? _sprite->getGlobalBounds() : JRectf::Zero;
}

