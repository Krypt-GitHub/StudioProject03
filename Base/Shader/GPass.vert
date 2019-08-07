#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec2 vertexTexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 lightDepthMVP;

out vec2 texCoord;

void main(){
	gl_Position = lightDepthMVP * vec4(vertexPosition_modelspace, 1);

	texCoord = vertexTexCoord;
}

