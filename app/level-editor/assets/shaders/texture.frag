#version 330 core

const int MAX_TEXUTRES = 32;

uniform sampler2D u_Textures[MAX_TEXUTRES];

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

void main()
{
    vec4 tex_color = v_Color;

    switch (int(v_TexIndex))
    {
        case 0: tex_color *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;
        case 1: tex_color *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;
        case 2: tex_color *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;
        case 3: tex_color *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;
        case 4: tex_color *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;
        case 5: tex_color *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;
        case 6: tex_color *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;
        case 7: tex_color *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;
        case 8: tex_color *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;
        case 9: tex_color *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;
        case 10: tex_color *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
        case 11: tex_color *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
        case 12: tex_color *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
        case 13: tex_color *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
        case 14: tex_color *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
        case 15: tex_color *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
        case 16: tex_color *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
        case 17: tex_color *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
        case 18: tex_color *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
        case 19: tex_color *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
        case 20: tex_color *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
        case 21: tex_color *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
        case 22: tex_color *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
        case 23: tex_color *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
        case 24: tex_color *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
        case 25: tex_color *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
        case 26: tex_color *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
        case 27: tex_color *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
        case 28: tex_color *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
        case 29: tex_color *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
        case 30: tex_color *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
        case 31: tex_color *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
    }

    color = tex_color;
}
