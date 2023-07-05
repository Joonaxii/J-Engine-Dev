#shader vertex
#version 330 core

layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertUV;
out vec4 _VertexColor;
void main()
{
    gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);
    _VertexColor = vec4(vertColor.x, vertColor.y, vertColor.z, vertColor.w);
};

#shader fragment
#version 330 core

in vec4 _VertexColor;
out vec4 _FragColor;
void main()
{
    _FragColor = vec4(_VertexColor.x, _VertexColor.y, _VertexColor.z, _VertexColor.a);
};