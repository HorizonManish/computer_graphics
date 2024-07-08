#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);				//flip texture upside down
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);		//GL_TEXTURE0 is a texture unit. Can hold upto 16 textures 
	glBindTexture(texType, ID);

	//adjusting settings of texture
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		//when zoomed, how should it look
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_NEAREST);			//how texture should repeat
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_NEAREST);

	glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);							//generating texture
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(texType, 0);			//Unbinding the texture
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}