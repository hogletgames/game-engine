#version 330 core

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TexCoord;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(u_Texture, v_TexCoord) * u_Color;
}
