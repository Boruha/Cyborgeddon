#include <Engine/util/shaders/Shader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>

#include <Engine/util/glad/glad.h>

Shader::Shader(const std::string_view path [ SHADER_TYPES ])
{
    
    std::string content [ SHADER_TYPES ];
    std::ifstream file  [ SHADER_TYPES ];

    unsigned shader_ID [ SHADER_TYPES ];

    int successOperation;
    char infoLog [ 512 ];

    const int opengl_shader_type [] { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

    ID = glCreateProgram();

    for (unsigned i = 0; i < SHADER_TYPES; ++i)
    {
    	if (path[i].empty())
    		continue;

    	file[i].open(path[i].data());

    	std::stringstream stream;
    	stream << file[i].rdbuf();

    	file[i].close();

    	content[i] = stream.str();

        const char * source = content[i].c_str();

        shader_ID[i] = glCreateShader(opengl_shader_type[i]);
        glShaderSource(shader_ID[i], 1, &source, nullptr);
        glCompileShader(shader_ID[i]);

        glGetShaderiv(shader_ID[i], GL_COMPILE_STATUS, &successOperation);

        if (!successOperation)
        {
            glGetShaderInfoLog(shader_ID[i], 512, nullptr, infoLog);
            std::cerr << "Error al compilar el shader " << path[i] << "\n" << infoLog << std::endl;
            exit(-1);
        }

        glAttachShader(ID, shader_ID[i]);
        glDeleteShader(shader_ID[i]);
    }

    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &successOperation);

    if (!successOperation)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Error al enlazar el programa\n" << infoLog << std::endl;
        exit(-1);
    }

}

void Shader::enable() const {
    glUseProgram(ID);
}

void Shader::boolUniform(const std::string_view name, const bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.data()), int(value));
}

void Shader::intUniform(const std::string_view name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}

void Shader::floatUniform(const std::string_view name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.data()), value);
}

void Shader::vec2Uniform(const std::string_view name, const glm::vec2 &vec) const {
	glUniform2fv(glGetUniformLocation(ID, name.data()), 1, &vec[0]);
}

void Shader::vec3Uniform(const std::string_view name, const glm::vec3 &vec) const {
	glUniform3fv(glGetUniformLocation(ID, name.data()), 1, &vec[0]);
}

void Shader::vec4Uniform(const std::string_view name, const glm::vec4 &vec) const {
	glUniform4fv(glGetUniformLocation(ID, name.data()), 1, &vec[0]);
}

void Shader::mat2Uniform(const std::string_view name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::mat3Uniform(const std::string_view name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::mat4Uniform(const std::string_view name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, &mat[0][0]);
}