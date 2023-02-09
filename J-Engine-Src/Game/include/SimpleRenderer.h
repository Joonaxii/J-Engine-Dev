#pragma once 
#include <JEngine/Math/Graphics/JVertex.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Rendering/IRenderer.h>
#include <JEngine/Assets/Graphics/Sprite.h>
#include <JEngine/Utility/Flags.h>
#include <vector>

class SimpleRenderer : public JEngine::IRenderer<JVertex2f> {
public:
    SimpleRenderer();
    SimpleRenderer(JEngine::Sprite* sprite);

    void setTRS(const JVector2f& position, const float rotation, const JVector2f& scale);

    void setSprite(JEngine::Sprite* sprite);

    bool canRender() const override;
   
    void setFlipX(const bool flip);
    bool getFlipX() const;

    void setFlipY(const bool flip);
    bool getFlipY() const;

    void setObjectLayer(const uint32_t layer);
    uint32_t getObjectLayer() const override;

    int32_t getVertexCount() const override;
    int32_t getIndexCount() const override;

    const JVertex2f* getVertices() const override;
    const uint32_t* getIndices() const override;

    const JEngine::JMatrix4f& getWorldMatrix() const override;

    void updateRenderer() override; 

protected:
    const JRectf& getLocalBounds() const override;
    virtual bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override { return false; }

private:
    static constexpr uint8_t FLIP_X = 0x1;
    static constexpr uint8_t FLIP_Y = 0x2;

    UI8Flags _flip;
    JEngine::Sprite* _sprite;
    JEngine::JMatrix4f _matrix;
    uint32_t _layer;
};