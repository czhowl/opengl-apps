#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 Color;
out vec2 TexCoord;
out vec3 Position;  
out vec3 Normal;
out vec3 LightPos;

uniform float uTime;
uniform mat4 uTransform;
uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	Position = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
//	Color = 1.0 - (smoothstep(sin(uTime), 1.0, 0.0) * aColor + smoothstep(sin(uTime), -1.0, 0.0) * (1.0 - aColor));
	Color = vec3(1.0);
//	TexCoord = aTexCoord * (sin(uTime) * 0.5 + 1.5);
	TexCoord = aTexCoord;
}
