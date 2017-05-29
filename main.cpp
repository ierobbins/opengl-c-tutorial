#define GLEW_STATIC
#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void processInput(GLFWwindow *window);

int main(){
    GLenum err;
    glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(err = glGetError() != GL_NO_ERROR){
        std::cout << "Error beneath make context current: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieve function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if(glewInit() != GLEW_OK){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    if(err = glGetError() != GL_NO_ERROR){
        std::cerr << "Error beneath glewInit(): " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if(err = glGetError() != GL_NO_ERROR){
        std::cerr << "Error beneath glfwGetFramebufferSize: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);

    // Begin vertex and fragment shaders
    GLfloat vertices[] ={
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // VBO INIT
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLchar *vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "void main(){"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";

    const GLchar *fragment_shader =
    "#version 330 core\n"
    "out vec4 FragColor;"
    "void main(){"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}";

    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    // Checking for vertex shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // Checking for fragment shader compilation errors
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create Shader program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Game loop
    while(!glfwWindowShouldClose(window)){
        // Check if any events have been activated (key press, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        processInput(window);

        // Render, clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    // When a user presses the escape key, we set the WindowShouldClose propety to true, closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// int main(){
//     //start openGL in C
//     if(!glfwInit()){
//         fprintf(stderr, "ERROR: could not start GLFW\n");
//         return 1;
//     }
//
//     GLenum err;
//     GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
//     if(!window){
//         fprintf(stderr, "ERROR: could not open window with GLFW3\n");
//         glfwTerminate();
//         return 1;
//     }
//
//     glfwMakeContextCurrent(window);
//
//     // start GLEW extension handler
//     glewExperimental = GL_TRUE;
//     glewInit();
//
//     const GLubyte* renderer = glGetString(GL_RENDERER);
//     const GLubyte* version = glGetString(GL_VERSION);
//     printf("Renderer: %s\n", renderer);
//     printf("OpenGL version supported %s\n", version);
//
//     // tell GL to only draw onto a pixel if the shape is closer to the viewer
//     glEnable(GL_DEBUG_OUTPUT); // enable depth-testing
//     glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
//
//     /* OTHER STUFF GOES HERE NEXT */
//     float points[] = {
//         0.0f ,  0.5f, 0.0f,
//         0.5f , -0.5f, 0.0f,
//         -0.5f, -0.5f, 0.0f
//     };
//
//     // VERTEX BUFFER OBJECT (VBO)
//     GLuint vbo = 0;
//     glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
//
//     // VERTEX ATTRIBUTE OBJECT (VAO)
//     GLuint vao = 0;
//     glGenVertexArrays(1, &vao);
//     glBindVertexArray(vao);
//     glEnableVertexAttribArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//
//     // vertex shader
//     const char* vertex_shader =
//     "#version 410\n"
//     "in vec3 vp;"
//     "void main() {"
//     "  gl_Position = vec4(vp, 1.0);"
//     "}";
//
//     // fragment shader
//     const char* fragment_shader =
//     "#version 410\n"
//     "out vec4 frag_colour;"
//     "void main() {"
//     "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
//     "}";
//
//     // Creating and compiling the vertex and fragment shaders from the code above
//     GLuint vs = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vs, 1, &vertex_shader, NULL);
//     glCompileShader(vs);
//     GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fs, 1, &fragment_shader, NULL);
//     glCompileShader(fs);
//
//     // Now, these compiled shaders must be combined into a single executable GPU shader program
//     GLuint shader_program = glCreateProgram();
//     glAttachShader(shader_program, fs);
//     glAttachShader(shader_program, vs);
//     glLinkProgram(shader_program);
//
//     // Window drawing
//     while(!glfwWindowShouldClose(window)){
//         // update other events like input handling
//         glfwPollEvents();
//
//         // wipe the drawing surface clear
//         glClearColor(0.0, 1.0, 0.0, 1.0);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glBindVertexArray(vao);
//         glUseProgram(shader_program);
//         if((err = glGetError()) != GL_NO_ERROR){
//             printf("glGetError(use program): %x\n", err);
//             return 1;
//         }
//         // draw points 0-3 from the currently bound VAO with current in-use shader
//         glDrawArrays(GL_TRIANGLES, 0, 3);
//         glBindVertexArray(0); // unbind vao
//         // put the stuff we've been drawing onto the display
//         glfwSwapBuffers(window);
//         printf("ERROR: %d, %d", glGetError(), GL_NO_ERROR);
//     }
//
//     // close GL context and any other GLFW resources
//     glfwTerminate();
//
//     return 0;
// }
