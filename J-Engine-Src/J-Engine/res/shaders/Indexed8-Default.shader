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
#blend One OneMinusSrcAlpha Add

uniform sampler2D _MainTex;
uniform sampler1D _MainTex_Pal;

in vec4 _VertexColor;
in vec2 _VertexUV;
out vec4 _FragColor;
void main()
{
    vec4 texU  = texture(_MainTex, _VertexUV);
    vec4 color = texture(_MainTex_Pal, texU.x);
    _FragColor = _VertexColor * vec4(color.x, color.y, color.z, color.w);
    _FragColor.rgb *= _FragColor.a;
};

