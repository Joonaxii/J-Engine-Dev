#shader vertex
#blend SrcAlpha OneMinusSrcAlpha Add
#version 330 core

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertUV;

uniform mat4 _MVP;

out vec4 _VertexColor;
out vec2 _VertexUV;
void main()
{
    gl_Position = _MVP * vec4(vertPos.x, vertPos.y, 0.0, 1.0);
    _VertexColor = vertColor;
    _VertexUV = vertUV;
};

#shader fragment
#version 330 core

uniform sampler2D _MainTex;

in vec4 _VertexColor;
in vec2 _VertexUV;
out vec4 _FragColor;
void main()
{
    vec4 texCol = texture2D(_MainTex, _VertexUV);
    _FragColor = texCol * _VertexColor;
};