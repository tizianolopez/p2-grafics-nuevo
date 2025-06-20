#version 330

in vec3 fragNormal;
out vec4 fragColor;

void main()
{
  fragColor = vec4(fragNormal * 0.5 + 0.5,1.0);
}
