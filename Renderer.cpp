#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);

}
// GLCheckError
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[Open GL Error] (" << error << "): " << function << " file: " << file <<
            "line: " << line << std::endl;
        return false;
    }
    return true;
}