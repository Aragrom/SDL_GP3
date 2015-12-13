#version 150

out vec4 FragColor;

in vec4 vertexColourOut;


void main()
{
	//FragColor = vertexColourOut;
	FragColor = vec4(0.0f,1.0f,0.0f,1.0f);
}