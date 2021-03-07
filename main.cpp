
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "texture.h"



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
    window = glfwCreateWindow(640, 480, "TP 9 SI", NULL, NULL);
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
    float positions[] = { // + textures
       -0.5f, 0.5f, 0.0f,  0.0f,0.0f, //0
        0.5f,  0.5f, 0.0f, 1.0f,0.0f, //1 
        -0.5f, -0.5f, 0.0f, 1.0f,1.0f, //2
        0.5f,  -0.5f, 0.0f, 0.0f,1.0f, //3
         
    };
    unsigned int indices[] = {
        0,1,2,
        1,3,2

    };
 
    VertexArray va;
    VertexBuffer vb(positions,4*3 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.addBUffer(vb, layout);
    IndexBuffer ib(indices,6);
    //shader declar
    
    Shader shader("res/shaders/shad.shader");
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    // #vertex array 
    //unbounding just make the values 0 in the bind functions go to BAHLOUL
    va.Unbind();
    shader.UnBind();
    vb.Bind();
    ib.Unbind();


    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    float r = 0.2f;
    float incr = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.2f, 0.3f,1.0f);
   

        glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::rotate(Model, glm::radians(45.f), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(20.0f), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(-20.f), glm::vec3(0, 0, 1));
        glm::mat4 MVP = Projection * View * Model;
        shader.SetUniformMat4("u_MVP", MVP);


        Texture texture("res/textures/0.raw");
        texture.Bind();
        shader.Bind();
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f
        shader.SetUniform1i("u_Texture", 0);
      
        VertexArray va;
        va.Bind();
        VertexBuffer vb(positions, 5 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        vb.Bind();
        va.addBUffer(vb, layout);
        IndexBuffer ib(indices, 6);
        ib.Bind();
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