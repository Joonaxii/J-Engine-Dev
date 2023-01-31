#shader vertex
#version 330 core

layout(location = 0) in vec2 circlePos;
layout(location = 1) in vec4 circleColor;
layout(location = 2) in vec2 circleUV;
layout(location = 3) in vec2 circleEdge;

uniform mat4 _MVP;

out vec4 _CircleColor;
out vec2 _CircleUV;
out vec2 _CircleEdge;
void main()
{
    gl_Position = _MVP * vec4(circlePos.x, circlePos.y, 0.0, 1.0);
    _VertexColor = _CircleColor;
    _VertexUV = (vertUV * 2.0) - 1.0;
};

#shader fragment
#blend SrcAlpha OneMinusSrcAlpha Add
#version 330 core

in vec4 _CircleColor;
in vec2 _CircleUV;
in vec2 _CircleEdge;
out vec4 _FragColor;

#include "JEngine-GLSL.gli" -> float invlerp(float, float, float);
#include "JEngine-GLSL.gli" -> #Clamp;

void main()
{
    float dist = length(_CircleUV);

    if (dist > 1.0) { discard; }
    _FragColor = _VertexColor;

    if(dist <= _CircleEdge.x){
        _FragColor.a* invlerp(_CircleEdge.x, _CircleEdge.y, dist);
    }

    
};