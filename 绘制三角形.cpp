#include <iostream>
#include <GLES3/gl3.h>
#include <SDL/SDL.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

EM_JS(void , _EM_LOG , (int log) , {
    console.log(log);
})

#include "shaders.h"

using namespace std;

GLuint m_ProgramObj;
GLuint m_VertexShader;
GLuint m_FragmentShader;

Texture m_texture;
GLuint m_TextureId;

SDL_Surface* screen;

GLfloat vVertices[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

GLint uniformOriginX, uniformOriginY, uniformZoom;
extern "C" int init()
{
	  m_TextureId = GL_NONE;
}

extern "C" int initGL(int width, int height)
{
	_EM_LOG(1);
	//initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) 
	{
		screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
		if (screen == NULL)
		{
			cerr << "Could not set video mode: " << SDL_GetError() << endl;
			return 0;
		}
	}
	else 
	{
		cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
		return 0;
	}

	// glGenTextures(1, &m_TextureId) ;
    // glBindTexture(GL_TEXTURE_2D, m_TextureId);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D, GL_NONE);

	char vShaderStr[] =
			"attribute vec4 a_position;  \n"
            "void main () {              \n"
            "    gl_Position = a_position;\n"
            "    gl_PointSize = 10.0;     \n"   
            "}                            \n";

	char fShaderStr[] =
		"precision mediump float;                       \n"
        "void main () {                                 \n"
        "    gl_FragColor = vec4(1.0, 0.5, 1.0, 1.0);   \n"
        "}                                              \n";

	m_ProgramObj = CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
	glClearColor(1.0, 1.0, 0.0, 1.0);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	glViewport(0, 0, width, height);
	return 1;
}

extern "C" void drawTriangle()
{

	// if(m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) return;
    if(m_ProgramObj == GL_NONE) return;

	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 1.0);

	GLfloat verticesCoords[] = {
            -0.5, 0.0,
            0.5, 0.0,
            0.0, 0.5
    };

    // GLfloat textureCoords[] = {
    //         0.0f,  0.0f,        // TexCoord 0
    //         0.0f,  1.0f,        // TexCoord 1
    //         1.0f,  1.0f,        // TexCoord 2
    //         1.0f,  0.0f         // TexCoord 3
    // };

	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glUseProgram (m_ProgramObj);
    _EM_LOG(1000);
    // if( m_texture.data == nullptr)
    // {
    //     _EM_LOG(1001);
    // }
    // else
    // {
    //     _EM_LOG(1002);
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, m_TextureId);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture.width, m_texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture.data);
    //     glBindTexture(GL_TEXTURE_2D, GL_NONE);
    // }
	
	// 上传顶点数据
    glVertexAttribPointer (0, 2, GL_FLOAT,
                           GL_FALSE, 2 * sizeof (GLfloat), verticesCoords);
    // // 上传纹理数据
    // glVertexAttribPointer (1, 2, GL_FLOAT,
    //                        GL_FALSE, 2 * sizeof (GLfloat), textureCoords);

    glEnableVertexAttribArray (0);
    // glEnableVertexAttribArray (1);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, m_TextureId);
    
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapBuffers();
}


extern "C" void setTexture(int format, int width, int height, uint8_t *pData ,int index)
{
    m_texture.format = format;
    m_texture.width = width;
    m_texture.height = height;

	m_texture.data = static_cast<uint8_t *>(malloc(width * height * 4));
	memcpy(m_texture.data, pData, width * height * 4);
    
	_EM_LOG(width);
	_EM_LOG(height);
}