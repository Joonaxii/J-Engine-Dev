#shader vertex
#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertUV;

uniform mat4 _MVP;

out vec4 _VertexColor;
out vec2 _VertexUV;
void main()
{
    gl_Position = _MVP * vec4(vertPos.x, vertPos.y, vertPos.z, 1.0);
    _VertexColor = vertColor;
    _VertexUV = vertUV;
};

#shader fragment
#version 330 core

uniform sampler2D _MainTex;
#blend One OneMinusSrcAlpha Add

in vec4 _VertexColor;
in vec2 _VertexUV;
out vec4 _FragColor;
void main()
{
    vec4 texCol = texture2D(_MainTex, _VertexUV);
    _FragColor = texCol * _VertexColor;
    _FragColor.rgb *= _FragColor.a;
};