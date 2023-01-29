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

    const bool canRender() const override;
   
    void setFlipX(const bool flip);
    const bool getFlipX() const;

    void setFlipY(const bool flip);
    const bool getFlipY() const;

    void setObjectLayer(const uint32_t layer);
    const uint32_t getObjectLayer() const override;

    const int32_t getVertexCount() const override;
    const int32_t getIndexCount() const override;

    const JVertex2f* getVertices() const override;
    const uint32_t* getIndices() const override;

    const JMatrix4f& getWorldMatrix() const override;

    void updateRenderer() override; 

protected:
    const JRectf& getLocalBounds() const override;

private:
    static constexpr uint8_t FLIP_X = 0x1;
    static constexpr uint8_t FLIP_Y = 0x2;

    UI8Flags _flip;
    JEngine::Sprite* _sprite;
    JMatrix4f _matrix;
    uint32_t _layer;
};