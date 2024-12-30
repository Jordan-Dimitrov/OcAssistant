#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec2 aOffset;

out vec3 color;
out vec2 texCoord;

uniform float scale;

void main()
{
    vec3 scaledPos = vec3(aPos.x * scale, aPos.y * scale, aPos.z * scale);
    gl_Position = vec4(scaledPos + vec3(aOffset, 0.0), 1.0);
    color = aColor;
    texCoord = aTex;
}