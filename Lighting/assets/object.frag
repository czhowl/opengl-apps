#version 330 core
out vec4 FragColor;
in vec3 Color;

in vec2 TexCoord;
in vec3 Position;  
in vec3 Normal;
in vec3 LightPos;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - Position);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(-Position);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	vec4 pngtex = texture(uTexture2, TexCoord);
	vec4 jpgtex = texture(uTexture1, TexCoord);
    vec4 objColor = (step(pngtex, vec4(0.0)) * jpgtex + step(vec4(0.0), pngtex) * mix(pngtex, jpgtex, 0.3));
	FragColor = (ambientStrength + vec4(diffuse + specular, 1.0)) * objColor;
}
