#include <JEngine/Assets/Graphics/Sprite.h>
#include <algorithm>
#include <cstdlib>

namespace JEngine {

    uint32_t Sprite::DEFAULT_INDICES[6]{
    0, 1, 2,
    2, 3, 0
    };

    Sprite::Sprite() : 
        IAsset(),
        _vertices{ },
        _texture(-1),
        _textureRect(),
        _ppu(100.0f),
        _pivot(0.5f, 0.5f)
    {
        std::fill_n(_vertices, 16, JVertex2f({ 0, 0 }, { 255, 255, 255, 255 }, { 0, 0 }));
    }

    Sprite::Sprite(const Sprite& copy) : 
        IAsset(copy._name, copy.getId()),
        _vertices{},
        _textureRect(0, 0, 0, 0),
        _ppu(copy._ppu),
        _pivot(copy._pivot),
        _texture(copy._texture)
    {
        std::fill_n(_vertices, 16, JVertex2f({ 0, 0 }, { 255, 255, 255, 255 }, { 0, 0 }));
        setTexture(copy._texture, false);
        setTextureRect(copy._textureRect);
    }

    Sprite::Sprite(const std::string& name, const ObjectRef<Texture>& texture, const float ppu, const JVector2f pivot, const JRecti& rectangle) :
        IAsset(name),
        _vertices { },
        _texture(-1),
        _textureRect(),
        _ppu(ppu),
        _pivot(pivot)
       // _atlas(-1)
    {
        std::fill_n(_vertices, 16, JVertex2f({0, 0}, {255, 255, 255, 255}, {0, 0}));
        setTexture(texture);
        setTextureRect(rectangle);
    }

    Sprite& Sprite::operator=(const Sprite& copy) {
        _name = copy._name;
        _texture = ObjectRef<Texture>(nullptr);
        _textureRect = JRecti(0, 0, 0, 0);
        _ppu = copy._ppu;
        _pivot = copy._pivot;
       // _atlas = copy._atlas;

        memcpy(_vertices, copy._vertices, sizeof(_vertices));
        setTexture(_texture, false);
        setTextureRect(copy._textureRect);
        return *this;
    }

    void Sprite::setTexture(const ObjectRef<Texture>& texture, bool resetRect) {
        const Texture* texRef = texture.getPtr();

        if (resetRect || (!texRef && (_textureRect == JRecti()))) {
            setTextureRect(JRecti(0, 0, texRef->getSize().x, texRef->getSize().y));
        }

        //_texure = Game::getAssetManager().getAtlasByTexture(texture);

        // Assign the new texture
        _texture = ObjectRef<Texture>(texture);
        updateLocalBounds();
    }


    void Sprite::setTextureRect(const JRecti& rectangle) {
        if (rectangle != _textureRect) {
            _textureRect = rectangle;

            updateLocalBounds();
            updatePositions();
            updateTexCoords();
        }
    }

    void Sprite::setColor(const JColor32 color) {
        for (size_t i = 0; i < 16; i++) {
            _vertices[i].color = color;
        }
    }

    float Sprite::setPPU(const float ppu) {
        _ppu = ppu;
        updatePositions();
        return _ppu;
    }

    float Sprite::getPPU() const {
        return _ppu;
    }

    int32_t Sprite::writeToBuffer(const JMatrix4f& matrix, uint8_t flip, JVertex2f* verts) {
        flip *= 4;
        for (size_t i = 0; i < 4; i++) {
            auto vert = _vertices[flip + i];
            verts[i] = JVertex2f(matrix.transformPoint(vert.position), vert.color, vert.uv);
        }
        return 4;
    }

    void Sprite::setPivot(const JVector2f& pivot) {
        _pivot = pivot;
        updatePositions();
    }

    const JVector2f& Sprite::getPivot() const {
        return _pivot;
    }

    Texture* Sprite::getTexturePtr() { return _texture.getPtr(); }
    const Texture* Sprite::getTexturePtr() const { return _texture.getPtr(); }

    ObjectRef<Texture> Sprite::getTexture() { return _texture; }
    const ObjectRef<Texture>& Sprite::getTexture() const { return _texture; }

    const JRecti& Sprite::getTextureRect() const { return _textureRect; }
    const JRectf& Sprite::getLocalBounds() const { return _boundsLocal; }

    void Sprite::updateLocalBounds() {
        const auto size = _textureRect.getSize();
        float width = static_cast<float>(std::abs(size.x));
        float height = static_cast<float>(std::abs(size.y));
        _boundsLocal = JRectf(0.f, 0.f, width, height);
    }

    const JRectf& Sprite::getGlobalBounds() const {
        return _boundsWorld[0];
    }

    const JRectf& Sprite::getGlobalBounds(const uint8_t flip) const {
        return _boundsWorld[flip & 0x3];
    }

 /*   void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (_texture)
        {
            states.texture = _texture;
            sf::Vertex buf[4];

            updateWorldVerts(sf::Transform::Identity, 0, buf);
            target.draw(buf, 4, sf::TriangleStrip, states);
        }
    }*/

    void Sprite::updatePositions() {
        const auto size = _textureRect.getSize();
        float width  = static_cast<float>(std::abs(size.x));
        float height = static_cast<float>(std::abs(size.y));

        JVector2f scaled(width / _ppu, height / _ppu);

        JVector2f piv(
            _pivot.x * scaled.x, 
            (1.0f - _pivot.y) * scaled.y);
        JVector2f pivFlip(
            (1.0f - _pivot.x) * scaled.x, 
            (_pivot.y) * scaled.y);

        JVector2f minA(0, 0);
        JVector2f maxA(scaled.x, scaled.y);

        JVector2f minF(0, 0);
        JVector2f maxF(scaled.x, scaled.y);

        JVector2f off(-piv.x, -piv.y);
        JVector2f offFlip(-pivFlip.x, -pivFlip.y);

        auto vertMin = minA;
        auto vertMax = maxA;

        auto vertMinF = minF;
        auto vertMaxF = maxF;

        minA += off;
        maxA += off;

        minF += offFlip;
        maxF += offFlip;

        off.y = -pivFlip.y;
        offFlip.y = -piv.y;

        vertMin += off;
        vertMax += off;

        vertMinF += offFlip;
        vertMaxF += offFlip;

        //Default: 00
        _boundsWorld[0] = JRectf(minA, maxA);
        _vertices[0].position = JVector2f(vertMin.x, vertMin.y);
        _vertices[1].position = JVector2f(vertMin.x, vertMax.y);
        _vertices[2].position = JVector2f(vertMax.x, vertMax.y);
        _vertices[3].position = JVector2f(vertMax.x, vertMin.y);

        //Flip X: 01
        _boundsWorld[1] = JRectf(JVector2f(minF.x, minA.y), JVector2f(maxF.x, maxA.y));
        _vertices[4].position = JVector2f(vertMaxF.x, vertMin.y);
        _vertices[5].position = JVector2f(vertMaxF.x, vertMax.y);
        _vertices[6].position = JVector2f(vertMinF.x, vertMax.y);
        _vertices[7].position = JVector2f(vertMinF.x, vertMin.y);

        //Flip Y: 10
        _boundsWorld[2] = JRectf(JVector2f(minA.x, minF.y), JVector2f(maxA.x, maxF.y));
        _vertices[8].position  = JVector2f(vertMin.x, vertMaxF.y);
        _vertices[9].position  = JVector2f(vertMin.x, vertMinF.y);
        _vertices[10].position = JVector2f(vertMax.x, vertMinF.y);
        _vertices[11].position = JVector2f(vertMax.x, vertMaxF.y);

        //Flip XY: 11
        _boundsWorld[3] = JRectf(minF, maxF);
        _vertices[12].position = JVector2f(vertMaxF.x, vertMaxF.y);
        _vertices[13].position = JVector2f(vertMaxF.x, vertMinF.y);
        _vertices[14].position = JVector2f(vertMinF.x, vertMinF.y);
        _vertices[15].position = JVector2f(vertMinF.x, vertMaxF.y);
    }


    void Sprite::updateTexCoords() {
        auto tex = _texture.getPtr();
        
        if (tex) {
            const float w = float(tex->getSize().x);
            const float h = float(tex->getSize().y);

            const auto& min = _textureRect.getMin();
            const auto& max = _textureRect.getMax();

            auto bA = JVector2f(min.x / w, max.y / h);
            auto bB = JVector2f(min.x / w, min.y / h);
            auto bC = JVector2f(max.x / w, min.y / h);
            auto bD = JVector2f(max.x / w, max.y / h);

            for (size_t i = 0; i < 16; i += 4) {
                _vertices[i].uv = bA;
                _vertices[i + 1].uv = bB;
                _vertices[i + 2].uv = bC;
                _vertices[i + 3].uv = bD;
            }
        }
    }

    int32_t Sprite::getVertexCount() const {
        return 4;
    }

    const JVertex2f* Sprite::getVertices(const uint8_t flip) const {
        return _vertices + (flip * 4);
    }

    int32_t Sprite::getIndexCount() const {
        return 6;
    }

    const uint32_t* Sprite::getIndices() const {
        return DEFAULT_INDICES;
    }

  /*  void Sprite::setSourceAtlas(Atlas* atlas) {
        _texure.setPtr(atlas);
    }*/

  /*  Atlas* Sprite::getSourceAtlas() {
        return _texure.getPtr();
    }*/

    bool Sprite::serializeJson(json& jsonF) const {
        return false;
    }

    bool Sprite::deserializeJson(json& jsonF) {
        return false;
    }
    bool Sprite::serializeBinary(std::ostream& stream) const
    {
        return false;
    }
    bool Sprite::deserializeBinary(std::istream& stream, const size_t size)
    {
        return false;
    }
}