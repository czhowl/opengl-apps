#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 1) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform float uTime;
uniform mat4 uTransform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
//	Color = 1.0 - (smoothstep(sin(uTime), 1.0, 0.0) * aColor + smoothstep(sin(uTime), -1.0, 0.0) * (1.0 - aColor));
	Color = vec3(1.0);
//	TexCoord = aTexCoord * (sin(uTime) * 0.5 + 1.5);
	TexCoord = aTexCoord;
}
