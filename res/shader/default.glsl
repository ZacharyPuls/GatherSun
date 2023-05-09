#stage vertex
#version 460 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord;
out vec2 texCoords;
layout (location = 0) uniform mat4 projection;

void main() {
   gl_Position = projection * vec4(vertex, 0.0, 1.0);
   texCoords = texCoord;
}

#stage fragment
#version 460 core

in vec2 texCoords;
out vec4 FragColor;
layout(location = 1) uniform sampler2D fragTexture;

in vec2 fragTexCoord;

out vec4 outColor;

void main()
{
    FragColor = texture(fragTexture, texCoords);
}