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
layout (location = 1) uniform sampler2D msdfTexture;
layout (location = 2) uniform vec3 textColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

const float pxRange = 2.0;

void main() {
    vec3 msd = texture(msdfTexture, texCoords).rgb;
    float sigDist = dot(vec2(pxRange), 0.5/fwidth(texCoords))*(median(msd.r, msd.g, msd.b)-0.5);
    float alpha = clamp(sigDist + 0.5, 0.0, 1.0);
    FragColor = vec4(textColor, alpha);
}