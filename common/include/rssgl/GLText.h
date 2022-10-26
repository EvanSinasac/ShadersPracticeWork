/*******************************************************************************************************************************\
| Copyright (c) Oscar Lara 													   													|
| Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation    |
| files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,    |
| modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software|
| is furnished to do so, subject to the following conditions:					   											    |
|																			   													|
| The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.|
|																			   													|
| THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 	        |
| WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR         |
| COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,   |
| ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.							|
| * See individual libraries for separate legal notices.					   													|
\*******************************************************************************************************************************/
#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <assert.h>
#include <string>
#include "GLShader.h"
#include "GLLog.h"


namespace RSS {

	struct Character {
		unsigned int TextureID;
		glm::ivec2   Size;
		glm::ivec2   Bearing;
		unsigned int Advance;
	};

	class GLText
	{
	public:
		GLText(std::string applicationName, GLuint windowWidth, GLuint windowHeight, GLuint pixelSize = 24);
		virtual ~GLText();
		void setProjection(GLuint windowWidth, GLuint windowHeight);
		void addLine(std::string text, GLuint rowIndex);
		void render();
		void setFooter(std::string text);

	private:
		std::string _applicationName;
		GLuint _pixelSize;
		GLfloat _startY, _startX;
		GLfloat _initialRowsOffset;
		std::map<GLchar, Character> _characters;
		GLuint _vao, _vbo;
		GLShader _shader;
		GLuint _charsToLoad;
		GLuint _windowWidth, _windowHeight;
		void writeLine(std::string text, glm::vec2 position, glm::vec3 color, float scale);
		GLuint _maxLines;
		GLuint _maxColumns;
		GLuint _lineIndex;
		std::string _footer;
		std::map<GLuint, std::string> _lines;
		void initInformationBox();
		void renderMap(std::map<GLuint, std::string>textMap, glm::vec2 textPosition, glm::vec3 textColor, float scale = 1.0f);
		std::map<GLuint, std::string> _infoBox;
		void setStartYOffset(GLfloat offset);
		void setStartXOffset(GLfloat offset);

	};
}
