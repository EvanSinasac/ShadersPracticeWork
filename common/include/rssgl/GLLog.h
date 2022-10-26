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
#include <string>
#include <stdarg.h>
#include <ctime>
#include <time.h>
#include <fstream>

namespace RSS {
	class GLLog
	{
	public:
		static GLLog& getInstance();
		GLLog(GLLog const&) = delete;
		void operator=(GLLog const&) = delete;
		virtual ~GLLog();
		void clearLogs();
		bool error(const char* text, ...);
		bool info(const char* text, ...);
		bool debug(const char* text, ...);
	private:
		GLLog();
		//std::string _errorFileName;
		//std::string _infoFileName;
		std::string _logFileName;
		bool writeToFile(std::string fileName, std::string text);
		bool fileExists(std::string fileName);
	};
}
