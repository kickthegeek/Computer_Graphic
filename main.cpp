
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //setting opengl version 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */ 
    glfwMakeContextCurrent(window);
    //synchronise the swap
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) 
        std::cout << "Error !" << std::endl ;
    
    // FIRST OBJECT PARAMETERS
    float positions[] = {
       -0.5f, 0.5f, //0
        0.5f,  0.5f, // 1
        -0.5f, -0.5f, // 3
        0.5f,  -0.5f, // 4

    };
    unsigned int indices[] = {
        0,1,2,
        1,3,2

    };
    float positions2[] = {
       -0.75f, 0.75f, //0
        0.75f,  0.75f, // 1
        -0.75f, -0.75f, // 3
        0.75f,  -0.75f, // 4

    };
    unsigned int indices2[] = {
        0,1,2,
        1,3,2

    };
    //converting these to 
    //unsigned int vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //these 
    VertexArray va;


    //replacing the old code 
    //creating first object
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    //converting these 
    /*glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    *///to these
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.addBUffer(vb, layout);
    IndexBuffer ib(indices, 6);
    
    


    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f,-1.0f,1.0f );

    //shader declar
    
    Shader shader("res/shaders/shad.shader");
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    // #vertex array 
    //unbounding just make the values 0 in the bind functions go to BAHLOUL
    va.Unbind();
    shader.UnBind();
    vb.Bind();
    ib.Unbind();
    

    float r = 0.2f;
    float incr = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

   
        shader.Bind();
        shader.SetUniform4f("u_Color",r, 0.3f, 0.8f, 1.0f);
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        va.Bind();
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.addBUffer(vb, layout);
        IndexBuffer ib(indices, 6);
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        r += incr;
        if (r > 1.0f || r < 0.0f)
            incr =  - incr ;
        
        


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    

    glfwTerminate();
    return 0;
}