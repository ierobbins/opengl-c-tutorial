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

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEBUG_OUTPUT); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */
    GLfloat points[] = {
        0.0f ,  0.5f, 0.0f,
        0.5f , -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    // VERTEX BUFFER OBJECT (VBO)
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // VERTEX ATTRIBUTE OBJECT (VAO)
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // vertex shader
    const char* vertex_shader =
    "#version 410\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);"
    "}";

    // fragment shader
    const char* fragment_shader =
    "#version 410\n"
    "out vec4 frag_color;"
    "void main() {"
    "  frag_color = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

    // Creating and compiling the vertex and fragment shaders from the code above
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // Now, these compiled shaders must be combined into a single executable GPU shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    printf("THIS IS AN ERROR MESSAGE");
    printf("ERROR: %d, %d", glGetError(), GL_NO_ERROR);

    // Window drawing
    while(!glfwWindowShouldClose(window)){
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);

        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // update other events like input handling
        glfwPollEvents();

        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();

    return 0;
}
