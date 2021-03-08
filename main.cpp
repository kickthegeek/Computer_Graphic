
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "texture.h"
#define PI 3.14


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);


struct STRVertex
{
    glm::vec3 position;
    glm::vec3 couleur;
}; 

float int_float_color(int x)
{
    return float((x * 100) / 255);
}
glm::vec3 getColor(int r,int g, int b)
{
    return glm::vec3(int_float_color(r), int_float_color(g), int_float_color(b));
}


float camera_posx = 0.0f;
float scale = 1.0f;
float angle = 0.1f;

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

float rotate_x = 0.0f;
float rotate_y = 0.0f;
float rotate_z = 0.0f;

glm::vec4 background_color = glm::vec4 (getColor(213,50,50),1.0f);

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
    window = glfwCreateWindow(640, 480, "Projet OpenGL SI MIV 2021 GUENAOUI + NAIT KACI ", NULL, NULL);
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
    

    // LINES OF 3 AXES 
    float x_axe[] = {
         -1000.0f, 0.0f, 0.0f,  //0
          1000.0f, 0.0f, 0.0f,  //1
    };
    float y_axe[] = {
          0.0f,-1000.0f, 0.0f,  //0
          0.0f, 1000.0f, 0.0f,  //1
    };
    float z_axe[] = {
          0.0f, 0.0f,-1000.0f,  //0
          0.0f, 0.0f, 1000.0f,  //1
    };
    unsigned int axe_ind[] = {
        0,1
    };
    // SQUARE
    float positions[] = { // color
       -0.5f, 0.5f, 0.0f,  //0
        0.5f,  0.5f, 0.0f, //1 
       -0.5f, -0.5f, 0.0f, //2
        0.5f,  -0.5f, 0.0f, //3
         
    };
    unsigned int indices[] = {
        0,1,2,
        1,3,2
    };

    // second object sphere
    //generates all vertices of the sphere for the given radius, sectors and stacks. 
    float vertices[10000];
    float normals[10000];
    float texCoords[10000];
    int vertices_length = 0;
    int normals_length = 0;
    int texCoords_length = 0;

    int radius = 10;
    int sectorCount = 10;
    int stackCount = 10;
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices[vertices_length++] = x;
            vertices[vertices_length++] = y;
            vertices[vertices_length++] = z;

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals[normals_length++] = nx;
            normals[normals_length++] = ny;
            normals[normals_length++] = nz;
            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords[texCoords_length++] = s;
            texCoords[texCoords_length++] = t;
        }
    }
    //indices
    unsigned int s_indices[10000];
    unsigned int s_lineIndices[10000];
    int s_indices_length = 0;
    int s_lineIndices_length = 0;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                s_indices[s_indices_length++] = k1;
                s_indices[s_indices_length++] = k2; 
                s_indices[s_indices_length++] = k1+1;
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                s_lineIndices[s_lineIndices_length++] = k1 + 1;
                s_lineIndices[s_lineIndices_length++] = k2;
                s_lineIndices[s_lineIndices_length++] = k2 + 1;

            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            s_lineIndices[s_lineIndices_length++] = k1;
            s_lineIndices[s_lineIndices_length++] = k2 ;

            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                s_lineIndices[s_lineIndices_length++] = k1;

                s_lineIndices[s_lineIndices_length++] = k1 + 1;

            }
        }
    }

    //---------end of sphere

 
    VertexArray va;
    VertexBuffer vb(positions, 4 * 3 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
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
    
    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.005f;
    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);//getting mouse positions//
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, background_color[3]);
   

        //interaction functions call back 
         //keyboard and mouse call 
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);

        //camera and matrix
        glm::vec3 camX = normalize(cross(camUp, camDirection));


        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.0f);
        //glm::mat4 View = glm::lookAt(glm::vec3(camX, 0, camZ), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 View = glm::lookAt(camPos, camX, camUp);
        glm::mat4 Model = glm::mat4(1.0f);
        //translate
        Model = glm::translate(Model, glm::vec3(-2.0f, 0.0f, -5.0f));
        //scale
        Model = glm::scale(Model, glm::vec3(1.0f * scale, 1.0f * scale, scale * 1.0f));


        /*glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);*/
        Model = glm::rotate(Model, glm::radians(rotate_x), glm::vec3(1, 0, 0));
        Model = glm::rotate(Model, glm::radians(rotate_y), glm::vec3(0, 1, 0));
        Model = glm::rotate(Model, glm::radians(rotate_z), glm::vec3(0, 0, 1));
        glm::mat4 MVP = Projection * View * Model;
        shader.SetUniformMat4("u_MVP", MVP);


        //Texture texture("res/textures/0.raw");
        //texture.Bind();
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        //shader.SetUniform1i("u_Texture", 0);
      
        VertexArray va;
        va.Bind();
        //drawing the axes
        //x_axis
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        VertexBuffer vbxa(x_axe, 2 * 3 * sizeof(float));
        VertexBufferLayout layout_xaxis;
        layout_xaxis.Push<float>(3);
        vbxa.Bind();
        va.addBUffer(vbxa, layout_xaxis);
        IndexBuffer ibxa(axe_ind, 2);
        ibxa.Bind();
        GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));
        //y_axis
        shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
        VertexBuffer vbya(y_axe, 2 * 3 * sizeof(float));
        VertexBufferLayout layout_yaxis;
        layout_yaxis.Push<float>(3);
        vbxa.Bind();
        va.addBUffer(vbya, layout_yaxis);
        IndexBuffer ibya(axe_ind, 2);
        ibya.Bind();
        GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));
        //z_axis
        shader.SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
        VertexBuffer vbza(z_axe, 2 * 3 * sizeof(float));
        VertexBufferLayout layout_zaxis;
        layout_zaxis.Push<float>(3);
        vbza.Bind();
        va.addBUffer(vbza, layout_yaxis);
        IndexBuffer ibza(axe_ind, 2);
        ibza.Bind();
        GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr));


        //drawing the objects
        shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);
        //drawing sphere
        VertexBuffer vbsphere(vertices, vertices_length * sizeof(float));
        VertexBufferLayout layout_sphere;
        layout_sphere.Push<float>(3);
        vbsphere.Bind();
        va.addBUffer(vbsphere, layout_sphere);
        IndexBuffer ibsphere(s_indices, s_indices_length);
        ibsphere.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, s_indices_length, GL_UNSIGNED_INT, nullptr));
        

        r += incr;
        if (r > 1.0f || r < 0.0f)
            incr =  - incr ;
        //drawing square
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        VertexBuffer vb(positions, 6 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        vb.Bind();
        va.addBUffer(vb, layout);
        IndexBuffer ib(indices, 6);
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    

    glfwTerminate();
    return 0;





}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float camSpeed = 0.1f;
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        std::cout << "pess" + key << std::endl;
    if (action == GLFW_PRESS)
        switch (key) {
        case GLFW_KEY_W:
            //scale up
            //scale += 0.1f;
            //camPos += camSpeed * camDirection;
            camPos = glm::vec3(camPos.x, camPos.y + camSpeed, camPos.z);
            break;
        case GLFW_KEY_S:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x, camPos.y - camSpeed, camPos.z);
            break;
        case GLFW_KEY_D:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x + camSpeed, camPos.y, camPos.z);
            break;
        case GLFW_KEY_A:
            //scale down 
             //scale -= 0.1f;
             //camPos -= camSpeed * camDirection;
            camPos = glm::vec3(camPos.x - camSpeed, camPos.y, camPos.z);
            break;
        case GLFW_KEY_Y:
            /* rotate around Z */ 
            rotate_x += 1.0f;
            std::cout << "rotation X " << rotate_x << std::endl;
            break;
        case GLFW_KEY_U:
            /* rotate around Z */
            rotate_y += 1.0f;
            std::cout << "rotation Y " << rotate_y << std::endl;
            break;
        case GLFW_KEY_I:
            /* rotate around Z */
            rotate_z += 1.0f;
            std::cout << "rotation Z " << rotate_z << std::endl;
            break;
        case GLFW_KEY_Q: /* Escape */ exit(0);
            break;

        }
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << xpos << " : " << ypos << " ;::::" << std::endl;
    float camSpeed = 0.1f;
    int stateL = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (stateL == GLFW_PRESS)
    {
        //std::cout << " CLICKED " << std::endl;
        rotate_y += 1.0f;
        std::cout << "rotation Y " << rotate_y << std::endl;

        /*   camera_posx += 1.0f;
           angle += 0.1f;
           camPos += camSpeed * camDirection;*/

    }


}