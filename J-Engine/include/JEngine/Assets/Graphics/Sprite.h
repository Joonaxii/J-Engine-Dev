#pragma once
#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <string>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Collections/ObjectRef.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Rendering/IRenderable.h>

namespace JEngine {
    class SpriteBatch;
    class Sprite : public IAsset, public IRenderable
    {
    public:
        Sprite();
        Sprite(const Sprite& copy);
        Sprite(const std::string& name, const ObjectRef<Texture>& texture, const float ppu, const JVector2f pivot, const JRecti& rectangle);

        Sprite& operator=(const Sprite& copy);

        void setTexture(const ObjectRef<Texture>& texture, bool resetRect = false);
        void setTextureRect(const JRecti& rectangle);
        void setColor(const JColor32 color);

        int32_t writeToBuffer(const JMatrix4f& matrix, uint8_t flip, JVertex* verts);

        float setPPU(const float ppu);
        float getPPU() const;

        void setPivot(const JVector2f& pivot);
        const JVector2f& getPivot() const;

        Texture* getTexturePtr();
        const Texture* getTexturePtr() const;

        ObjectRef<Texture> getTexture();
        const ObjectRef<Texture>& getTexture() const;

        const JRecti& getTextureRect() const;

        const JRectf& getLocalBounds() const;
        const JRectf& getGlobalBounds() const;
        const JRectf& getGlobalBounds(const uint8_t flip) const;

        int32_t getVertexCount() const override;
        const JVertex* getVertices(const uint8_t flip = 0) const override;

        int32_t getIndexCount() const override;
        const uint32_t* getIndices() const override;
     
        //void setSourceAtlas(ObjectRef<Atlas>& atlas);
        //ObjectRef<Atlas>& getSourceAtlas();

        bool serializeJson(json& jsonF) const;
        bool deserializeJson(json& jsonF);

        bool serializeBinary(const Stream& stream) const;
        bool deserializeBinary(const Stream& stream, const size_t size);
    private:
        static uint32_t         DEFAULT_INDICES[6];

        JRectf                  _boundsLocal;
        JRectf                  _boundsWorld[4];

        JVertex                 _vertices[16];

        JRecti                  _textureRect;
        float                   _ppu;
        JVector2f               _pivot;

        ObjectRef<Texture>      _texture;
        //ObjectRef<Atlas>   _atlas;

        void updatePositions();
        void updateTexCoords();

        void updateLocalBounds();
    };
}