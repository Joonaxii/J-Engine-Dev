#shader vertex
#version 330 core

layout(location = 0) in vec2 gizmoPos;
layout(location = 1) in vec4 gizmoColor;
layout(location = 2) in vec2 gizmoUV;
layout(location = 3) in vec3 gizmoInfo;

uniform mat4 _MVP;

out vec4 _GizmoColor;
out vec2 _GizmoUV;
out uint _GizmoMode;
out vec2 _GizmoEdge;
void main()
{
    gl_Position = _MVP * vec4(circlePos.x, circlePos.y, 0.0f, 1.0);
    _VertexColor = _CircleColor;
    _VertexUV = (vertUV * 2.0) - 1.0;

    _GizmoMode = uint(gizmoInfo.x);
    _GizmoEdge = gizmoInfo.yz;
};

#shader fragment
#blend SrcAlpha OneMinusSrcAlpha Add
#version 330 core

in vec4 _GizmoColor;
in vec2 _GizmoUV;
in uint _GizmoMode;
in vec2 _GizmoEdge;
out vec4 _FragColor;

float invlerp(float a, float b, float v) {
    return (v - a) / (b - a);
}

void main()
{
    _FragColor = _VertexColor;
    if (_GizmoMode == 0) { //Is a line
        return;
    }

    float dist = length(_CircleUV);

    switch (_GizmoMode) {
        case 1: //Box
            float edX = abs(_CircleUV.x);
            float edY = abs(_CircleUV.y);

            float edAvg = ((edX + edY) * 0.5);
            if (edAvg <= _CircleEdge.x) {
                _FragColor.a *= invlerp(_CircleEdge.x, _CircleEdge.y, edAvg);
                if (_FragColor.a <= 0) { discard; }
            }
            break;
        case 2: //Circle
            if (dist > 1.0) { discard; }
            if (dist <= _CircleEdge.x) {
                _FragColor.a *= invlerp(_CircleEdge.x, _CircleEdge.y, dist);
                if (_FragColor.a <= 0) { discard; }
            }
            break;
    }

};