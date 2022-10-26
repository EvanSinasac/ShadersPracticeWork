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
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GLLog.h"

namespace RSS {
	class GLShader
	{
	public:
		GLShader();
		virtual ~GLShader();

		GLuint ID;		
		bool loadFromFile(std::string vertexShaderFile, std::string fragmentShaderfile);
		bool loadFromString(std::string vertexShaderString, std::string fragmentShaderString);
		

		void use();
	private:
		bool isProgrammeValid();
		void printProgrammeInfo();
		void printShaderInfo(GLuint shaderID);

	};


}