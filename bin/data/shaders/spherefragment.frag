#version 330 core

uniform vec3 sphereColor;

void main()
{
	gl_FragColor = vec4(sphereColor.x, sphereColor.y, sphereColor.z, 1.0f);
}
