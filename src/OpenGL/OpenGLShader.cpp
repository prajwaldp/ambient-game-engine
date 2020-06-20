#include "Log.h"
#include "Renderer/Shader.h"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

namespace Ambient
{

OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
{
    // Source: https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        AM_CORE_ERROR("Vertex shader compilation failure! {}", infoLog.data());
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        AM_CORE_ERROR("Fragment shader compilation failure! {}", infoLog.data());
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_RendererID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    // Link our program
    glLinkProgram(m_RendererID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_RendererID);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        AM_CORE_ERROR("Shader Linking failure! {}", infoLog.data());
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_RendererID);
}

void OpenGLShader::UnBind() const
{
    glUseProgram(0);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1d(location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    // 1 = No. of matrices
    // GL_FALSE = no need to transpose. But for DirectX (which uses row-major
    // ordering), transposing is needed
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());

    // 1 = No. of matrices
    // GL_FALSE = no need to transpose. But for DirectX (which uses row-major
    // ordering), transposing is needed
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace Ambient