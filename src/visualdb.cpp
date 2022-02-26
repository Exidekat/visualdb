/* welcome to my special hell */
#include "visualdb.h"
#include "render.h"
#include <thread>

std::map<char, Character> Characters;
unsigned int VAO, VBO;
int fbw, fbh, NRSCNTST;

class Timer
{
public:
    
    double lastTime = glfwGetTime(), timer = lastTime, startTime = lastTime;
    double deltaTime = 0, nowTime = 0;
    int frames = 0, updates = 0;

    //setTime(double t) { time.thsithis.isthis }
};

/* OpenGL debugging! */
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

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
    else if (width <= 1920) {
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

	std::cout << "Hello VDB!" << std::endl;
	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_MAXIMIZED, true);
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
    if (NRSCNTST) {
        glClearColor(1.0f, 0.65f, 0.15f, 0.9f);
    }
    else {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }

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

    std::vector<float> data = {
        100.0, 100.0,
        400.0, 100.0,
        400.0, 400.0,
        100.0, 400.0
    };

    VertexBuffer* cvbo = new VertexBuffer(data);    //
    cvbo->bind();                                   //  todo: ask andy how work
                                                    //
    VertexArray* cvao = new VertexArray();          //
    cvao->vbo(cvbo, {2});                           //

    /* Load glyph shader */
    Shader glyphShader(glyph_vertexShaderPath, glyph_fragmentShaderPath);
    glProgramUniformMatrix4fv(glyphShader.ID, glGetUniformLocation(glyphShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader circleShader("shaders/coloredshape.vs.glsl","shaders/coloredcircle.fs.glsl");
    glProgramUniformMatrix4fv(circleShader.ID, glGetUniformLocation(circleShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    Shader shapeShader("shaders/coloredshape.vs.glsl", "shaders/coloredshape.fs.glsl");
    glProgramUniformMatrix4fv(shapeShader.ID, glGetUniformLocation(shapeShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

    glm::vec4 top_rgb       = rgba8_to_float(75, 220, 205, 255);       //teal
    glm::vec4 middle_rgb    = rgba8_to_float(220, 75, 50, 255);        //velvet
    glm::vec4 bottom_rgb    = rgba8_to_float(162, 220, 75, 255);       //limegreen

    std::string query;  // Active VDB query. should be defined by lua speech 'set query' //cheatsheet notes: 1280x720 min res
                                                                                         //
    static double limitFPS = 1.0 / 30.0;                                                 //
                                                                                         //
    double lastTime = glfwGetTime(), timer = lastTime;                                   //
    double deltaTime = 0, nowTime = 0;                                                   //
    int frames = 0, updates = 0;                                                         //MKDS max 40 characters at 1.0f scale
    /* Loop until the user closes the window */                                                 
    while (!glfwWindowShouldClose(window))                                                      
    {                                                                                           
        glfwSetWindowSizeCallback(window, window_size_callback); //Check for window resize      
        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;

        // Render at 30fps
        while (deltaTime >= 1.0) {
            update();   // - Update function
            updates++;
            deltaTime--;
        }

        /* Render here */                                                                       
        glClear(GL_COLOR_BUFFER_BIT);                                                           

        //top 'welcome' bar
        RenderShape(Shape::Rectangle, shapeShader, cvao, cvbo, { Align::Center, Align::Top }, 640.0f, 720.0f, 880.f, 120.f, { 0.0f, 0.0f, 1.0f, 1.0f });
        RenderText(MKDS_Characters, glyphShader, "WELCOME TO VDB", { Align::Center, Align::Top }, 640.0f, 715.0f, 1.0f, top_rgb);

        //da main thing
        RenderShape(Shape::Rectangle, shapeShader, cvao, cvbo, { Align::Center, Align::Center }, 640.0f, 360.0f, 880.f, 320.f, { 0.6f, 0.6f, 1.0f, 1.0f });
        RenderShape(Shape::Rectangle, shapeShader, cvao, cvbo, { Align::Center, Align::Center }, 640.0f, 360.0f, 870.f, 310.f, { (glm::sin(glfwGetTime()) + 1.0f) / 2.0f, (glm::sin(glfwGetTime() + 104) + 1.0f) / 2.0f, (glm::sin(glfwGetTime() + 20) + 1.0f) / 2.0f, 1.0f });
        RenderText(UbuntuB_Characters, glyphShader, "The main text goin on here.", { Align::Center, Align::Center }, 640.0f, 360.0f, 1.0f, middle_rgb);
        
        RenderText(UbuntuM_Characters, glyphShader, "current query(lua stuffs goin on here): " + query, { Align::Right, Align::Bottom }, 1275.0f, 5.0f, 0.5f, bottom_rgb);
        RenderText(UbuntuM_Characters, glyphShader, "framebuffer size: " + std::to_string(fbw) + "px, " + std::to_string(fbh) + "px", { Align::Left, Align::Bottom }, 5.0f, 5.0f, 0.5f, bottom_rgb);

        RenderShape(Shape::Rectangle, shapeShader, cvao, cvbo, { Align::Left, Align::Bottom }, 5.0f, 5.0f, 200.f, 200.f, { 1.0f, 0.2f, 0.2f, 1.0f });

        if (glfwGetTime() < 5) {
            RenderShape(Shape::Rectangle, shapeShader, cvao, cvbo, { Align::Left, Align::Bottom }, 0.0f, 0.0f, 1280.f, 720.f, { (glm::sin(glfwGetTime()) + 69.0f) / 2.0f, (glm::sin(glfwGetTime() + 104) + 1.0f) / 2.0f, (glm::sin(glfwGetTime() + 420.0f) + 1.0f) / 2.0f, 1.0f - (glfwGetTime()/6)});
            RenderText(MKDS_Characters, glyphShader, "WELCOME TO", { Align::Center, Align::Bottom }, 640.0f, 310.0f, 3.0f, rgba8_to_float(75, 220, 205, 255 - (255 * glfwGetTime()/6)));
            RenderText(MKDS_Characters, glyphShader, "VEE DEE BEE", { Align::Center, Align::Top }, 640.0f, 290.0f, 3.0f, rgba8_to_float(75, 220, 205, 255 - (255 * glfwGetTime()/6)));
        }

        /*shapeShader.use();
        glUniform2f(glGetUniformLocation(shapeShader.ID, "uCenter"), 200, 200);
        glUniform4f(glGetUniformLocation(shapeShader.ID, "uColor"), 0.0f, 0.0f, 1.0f, 0.4f);
        glUniform1f(glGetUniformLocation(shapeShader.ID, "uRadius2"), 100 * 100);
        glBindVertexArray(cvao->getHandle());
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);*/


        //glUniform2f(glGetUniformLocation(shapeShader.ID, "uCenter"), 200 + (25 * glm::cos(glfwGetTime())), + 200 + (25 * glm::sin(glfwGetTime())));
        //glUniform4f(glGetUniformLocation(shapeShader.ID, "uColor"), 0.0f, 0.0f, 1.0f, 0.4f);
        //glUniform1f(glGetUniformLocation(shapeShader.ID, "uRadius2"), 50 * 50);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //glUniform2f(glGetUniformLocation(shapeShader.ID, "uCenter"), 200 + (25 * glm::cos(glfwGetTime())) + (12.5 * glm::cos(2 * glfwGetTime() + 1)), + 200 + (25 * glm::sin(glfwGetTime()))  + (12.5 * glm::sin(2 * glfwGetTime() + 1)));
        //glUniform4f(glGetUniformLocation(shapeShader.ID, "uColor"), 0.0f, 0.0f, 1.0f, 0.4f);
        //glUniform1f(glGetUniformLocation(shapeShader.ID, "uRadius2"), 25 * 25);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glfwSwapBuffers(window); //Swap front and back buffers
        glfwPollEvents(); //Poll for and process events
        frames++; //fully rendered frame
        if (glfwGetTime() - timer > 1.0) {
            timer++;
            std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
            updates = 0, frames = 0;
        }
    }

    glfwTerminate();

    return 0;
}