#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main(){
    //start openGL in C
    if(!glfwInit()){
        fprintf(stderr, "ERROR: could not start GLFW\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if(!window){
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onro a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    // OTHER STUFF GOES HERE NEXT

    // close GL context and any other GLFW resources
    glfwTerminate();

    return 0;
}
