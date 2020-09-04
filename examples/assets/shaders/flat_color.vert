#version 330 core

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
