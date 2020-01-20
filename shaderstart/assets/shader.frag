#version 330 core
out vec4 FragColor;
in vec3 Color;

in vec2 TexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
void main()
{
	vec4 pngtex = texture(uTexture2, TexCoord);
	vec4 jpgtex = texture(uTexture1, TexCoord);
    FragColor = (step(pngtex, vec4(0.0)) * jpgtex + step(vec4(0.0), pngtex) * mix(pngtex, jpgtex, 0.3));
}
