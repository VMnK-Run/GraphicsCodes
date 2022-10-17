#ifndef __SHADER_H__
#define __SHADER_H__
#include <glad/glad.h>// 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	// 着色器程序ID
	unsigned int ID;

	//构造函数 读取点着色器和片段着色器
	Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. 从文件路径打开文件
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// 确保ifstream流能够抛出异常
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// 从文件流中读入到buffer中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// 关闭文件流
			vShaderFile.close();
			fShaderFile.close();
			// 把流转到string里面
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR!：Shader读取文件失败" << std::endl;
		}
		//转换成参数要求char *类型，
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();

		// 2. 编译着色器
		unsigned int vertex, fragment;
		// 点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// 片段着色器
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// 着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// 删除着色器，因为它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//使用着色器类
	void use() {
		glUseProgram(ID);
	}

	void setMat4(const char* mattype, glm::mat4 mat) {
		int Loc = glGetUniformLocation(ID, mattype);
		glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(mat));
	}
private:
	// 用于检查着色器编译 / 链接错误的实用程序函数。
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024]; //存储信息日志
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "错误：Shader编译错误: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "错误：Shader链接错误: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif
