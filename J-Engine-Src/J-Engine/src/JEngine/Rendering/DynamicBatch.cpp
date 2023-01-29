//#include <JEngine/rendering/SpriteBatch.h>
//#include <JEngine/rendering/IRenderable.h>
//#include <Core/JGraphics.h>

namespace JEngine {

    //SpriteBatch::SpriteBatch() : _vertCount(0), _material(nullptr), _vertBuffer(nullptr), _vertCapacity(0) { }

    //SpriteBatch::SpriteBatch(const int vertCount, Material* material, IRenderable** rends, const int rendCount) : _vertCount(0), _material(material) {
    //    _vertBuffer = new JVertex[_vertCapacity = vertCount];

    //    int v = 0;
    //    for (size_t i = 0; i < rendCount; i++) {
    //        auto rend = rends[i];
    //        v += rend->drawToBatch(*this);
    //    }
    //}

    //SpriteBatch::~SpriteBatch() {
    //    if (_vertBuffer) {
    //        delete[] _vertBuffer;
    //    }
    //}

    //void SpriteBatch::setup(const int vertCount, Material* material, IRenderable** rends, const int rendCount) {
    //    _vertCount = 0;
    //    _material = material;

    //    if (_vertCapacity < vertCount) {
    //        _vertCapacity = vertCount;

    //        delete[] _vertBuffer;
    //        _vertBuffer = new JVertex[_vertCapacity];
    //    }

    //    int v = 0;
    //    for (size_t i = 0; i < rendCount; i++) {
    //        auto rend = rends[i];
    //        v += rend->drawToBatch(*this);
    //    }
    //}

    //const int SpriteBatch::getBatchCapacity() const {
    //    return _vertCapacity;
    //}

    //const int SpriteBatch::getBatchSize() const {
    //    return _vertCount;
    //}

    //void SpriteBatch::draw(JRenderTarget& target, JRenderStates& states) const {
    //    if (_material) {
    //        states.texture = &_material->getTexture()->getTextureRef();
    //        target.draw(_vertBuffer, _vertCount, sf::Quads, states);
    //    }
    //}

    //void SpriteBatch::addVerts(const JVertex* verts, const int count) {
    //    memcpy(_vertBuffer + _vertCount, verts, count * sizeof(JVertex));
    //    _vertCount += count;
    //}

    //void SpriteBatch::addVerts(const JVertex* verts, const int count, const JMatrixf& matrix) {
    //    memcpy(_vertBuffer + _vertCount, verts, count * sizeof(JVertex));
    //    int newVerts = _vertCount + count;

    //    for (size_t i = _vertCount; i < newVerts; i++) {
    //        auto& vert = _vertBuffer[i];
    //        vert.position = matrix.transformPoint(vert.position);
    //    }
    //    _vertCount = newVerts;
    //}
}