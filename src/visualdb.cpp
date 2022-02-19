/* welcome to my special hell */
#include "visualdb.h"
#include "render.h"

std::map<char, Character> Characters;
unsigned int VAO, VBO;
int fbw, fbh;

/* OpenGL debugging! */
void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length,
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    /* Keep window size at something kosher */
    if (width <= 1280) {
        glfwSetWindowSize(window, 1280, 720);
    }
    else if (width <= 1920)  {
        glfwSetWindowAspectRatio(window, 1280, 720);
    }
    else if (width <= 2560) {
        glfwSetWindowAspectRatio(window, 2560, 1440);
    }

    /* Reset glViewport with new values */
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
}

int main() {
	std::cout << "Hello VDB." << std::endl;
	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "VisualDB", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwGetFramebufferSize(window, &fbw, &fbh);

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
    } 

    /* Sets an rgba color to glClear */
	glClearColor(1.0f,0.65f,0.15f,0.9f);
    
    /* Font loading! */
    //std::map<char, Character> arial_Characters = fontLoad("fonts/arial.ttf");
    std::map<char, Character> MKDS_Characters = fontLoad("fonts/MKDS.ttf");
    std::map<char, Character> halflife_Characters = fontLoad("fonts/halflife.ttf");
    std::map<char, Character> UbuntuB_Characters = fontLoad("fonts/Ubuntu-Bold.ttf");
    std::map<char, Character> UbuntuM_Characters = fontLoad("fonts/Ubuntu-Medium.ttf");
    std::map<char, Character> UbuntuR_Characters = fontLoad("fonts/Ubuntu-Regular.ttf");

    /* Enable blending*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Projection matrix, Orthographic */ 
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f); // for 1280x720 screen coordinates

    /* Creating a VBO and VAO for rendering the quads. */
    glGenVertexArrays(1, &VAO); // generate vertex array object and put the handle into the VAO variable
    glGenBuffers(1, &VBO); // generate buffer object and put the handle into the VBO variable
    glBindVertexArray(VAO); // set the currently bound vertex array to the vertex array object with the handle stored in VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // set the buffer currently bound to the ARRAY_BUFFER target to the buffer object with the handle stored in VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    /* Load glyph shader */
    Shader glyphShader(glyph_vertexShaderPath, glyph_fragmentShaderPath);
    glProgramUniformMatrix4fv(glyphShader.ID, glGetUniformLocation(glyphShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSetWindowSizeCallback(window, window_size_callback); //Check for window resize

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        RenderText(UbuntuB_Characters, glyphShader, "framebuffer size: " + std::to_string(fbw) + "px, " + std::to_string(fbh) + "px", 20.0f, 20.0f, 0.8f, glm::vec3(0.5f, 0.8f, 0.5f));
        RenderText(MKDS_Characters, glyphShader, "HAYDON BRAIN GO RBBRRBRB CRK CRASH BEEP", 100.0f, 620.0f, 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));
        //RenderText(UbuntuB_Characters, glyphShader, "HaYDoN BRain Go RBBRRBRB CRK CrASh bEEP", 250.0f, 550.0f, 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));
        //RenderText(UbuntuM_Characters, glyphShader, "HaYDoN BRain Go RBBRRBRB CRK CrASh bEEP", 250.0f, 400.0f, 1.0f, glm::vec3(0.3f, 0.7f, 0.9f));

        glfwSwapBuffers(window); //Swap front and back buffers
        glfwPollEvents(); //Poll for and process events
    }

    glfwTerminate();
    return 0;
}