//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//
//// Vertex Shader
//const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;
//out vec3 outColor;
//uniform mat4 transform;
//void main()
//{
//    gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
//    outColor = aColor;
//}
//)";
//
//// Fragment Shader
//const char* fragmentShaderSource = R"(
//#version 330 core
//in vec3 outColor;
//out vec4 FragColor;
//void main()
//{
//    FragColor = vec4(outColor, 1.0);
//}
//)";
//
//// Window dimensions
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// Clipping rectangle
//const float CLIP_MIN_X = -0.5f;
//const float CLIP_MAX_X = 0.5f;
//const float CLIP_MIN_Y = -0.5f;
//const float CLIP_MAX_Y = 0.5f;
//
//// Function prototypes
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//unsigned int compileShader(unsigned int type, const char* source);
//unsigned int createShaderProgram(const char* vertexShader, const char* fragmentShader);
//std::vector<float> clipLine(float x1, float y1, float x2, float y2);
//
//int main()
//{
//    // Initialize GLFW
//    if (!glfwInit())
//    {
//        std::cout << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // Set OpenGL version
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create window
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Line Clipping", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // Initialize GLEW
//    if (glewInit() != GLEW_OK)
//    {
//        std::cout << "Failed to initialize GLEW" << std::endl;
//        return -1;
//    }
//
//    // Create and compile shaders
//    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
//
//    // Define line vertices
//    float lineVertices[] = {
//        -0.8f, -0.8f,  // Start point
//         0.8f,  0.8f   // End point
//    };
//
//    // Clip the line
//    std::vector<float> clippedLine = clipLine(lineVertices[0], lineVertices[1], lineVertices[2], lineVertices[3]);
//
//    // Create vertex array object and vertex buffer object
//    unsigned int VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, clippedLine.size() * sizeof(float), clippedLine.data(), GL_STATIC_DRAW);
//
//    // Set vertex attribute pointers
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // Render loop
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//
//        // Create transformation matrix
//        glm::mat4 transform = glm::mat4(1.0f);
//        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
//        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
//
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_LINES, 0, clippedLine.size() / 5);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Clean up
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//unsigned int compileShader(unsigned int type, const char* source)
//{
//    unsigned int shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, NULL);
//    glCompileShader(shader);
//
//    int success;
//    char infoLog[512];
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success)
//    {
//        glGetShaderInfoLog(shader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
//        return 0;
//    }
//    return shader;
//}
//
//unsigned int createShaderProgram(const char* vertexShader, const char* fragmentShader)
//{
//    unsigned int program = glCreateProgram();
//    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
//    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//    glLinkProgram(program);
//
//    int success;
//    char infoLog[512];
//    glGetProgramiv(program, GL_LINK_STATUS, &success);
//    if (!success)
//    {
//        glGetProgramInfoLog(program, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//        return 0;
//    }
//
//    glDeleteShader(vs);
//    glDeleteShader(fs);
//
//    return program;
//}
//
//// Cohen-Sutherland algorithm for line clipping
//std::vector<float> clipLine(float x1, float y1, float x2, float y2)
//{
//    std::vector<float> result;
//
//    // Define region codes
//    const int INSIDE = 0; // 0000
//    const int LEFT = 1;   // 0001
//    const int RIGHT = 2;  // 0010
//    const int BOTTOM = 4; // 0100
//    const int TOP = 8;    // 1000
//
//    // Function to compute region code for a point (x, y)
//    auto computeCode = [](float x, float y) {
//        int code = INSIDE;
//        if (x < CLIP_MIN_X)
//            code |= LEFT;
//        else if (x > CLIP_MAX_X)
//            code |= RIGHT;
//        if (y < CLIP_MIN_Y)
//            code |= BOTTOM;
//        else if (y > CLIP_MAX_Y)
//            code |= TOP;
//        return code;
//        };
//
//    // Compute region codes for P1, P2
//    int code1 = computeCode(x1, y1);
//    int code2 = computeCode(x2, y2);
//    bool accept = false;
//
//    while (true)
//    {
//        // If both endpoints lie within rectangle
//        if ((code1 == 0) && (code2 == 0))
//        {
//            accept = true;
//            break;
//        }
//        // If both endpoints are outside rectangle
//        else if (code1 & code2)
//        {
//            break;
//        }
//        else
//        {
//            // Line needs clipping
//            // At least one endpoint is outside the rectangle, select it
//            int codeOut = code1 ? code1 : code2;
//
//            // Find intersection point
//            float x, y;
//            if (codeOut & TOP)
//            {
//                x = x1 + (x2 - x1) * (CLIP_MAX_Y - y1) / (y2 - y1);
//                y = CLIP_MAX_Y;
//            }
//            else if (codeOut & BOTTOM)
//            {
//                x = x1 + (x2 - x1) * (CLIP_MIN_Y - y1) / (y2 - y1);
//                y = CLIP_MIN_Y;
//            }
//            else if (codeOut & RIGHT)
//            {
//                y = y1 + (y2 - y1) * (CLIP_MAX_X - x1) / (x2 - x1);
//                x = CLIP_MAX_X;
//            }
//            else if (codeOut & LEFT)
//            {
//                y = y1 + (y2 - y1) * (CLIP_MIN_X - x1) / (x2 - x1);
//                x = CLIP_MIN_X;
//            }
//
//            // Replace point outside rectangle by intersection point
//            if (codeOut == code1)
//            {
//                x1 = x;
//                y1 = y;
//                code1 = computeCode(x1, y1);
//            }
//            else
//            {
//                x2 = x;
//                y2 = y;
//                code2 = computeCode(x2, y2);
//            }
//        }
//    }
//
//    if (accept)
//    {
//        // Line accepted
//        // Add the clipped line (green color)
//        result.push_back(x1);
//        result.push_back(y1);
//        result.push_back(0.0f);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//
//        result.push_back(x2);
//        result.push_back(y2);
//        result.push_back(0.0f);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//    }
//
//    // Add the original line segments outside the clipping area (red color)
//    if (x1 < CLIP_MIN_X || x1 > CLIP_MAX_X || y1 < CLIP_MIN_Y || y1 > CLIP_MAX_Y)
//    {
//        result.push_back(-0.8f);
//        result.push_back(-0.8f);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//        result.push_back(0.0f);
//
//        result.push_back(x1);
//        result.push_back(y1);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//        result.push_back(0.0f);
//    }
//
//    if (x2 < CLIP_MIN_X || x2 > CLIP_MAX_X || y2 < CLIP_MIN_Y || y2 > CLIP_MAX_Y)
//    {
//        result.push_back(x2);
//        result.push_back(y2);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//        result.push_back(0.0f);
//
//        result.push_back(0.8f);
//        result.push_back(0.8f);
//        result.push_back(1.0f);
//        result.push_back(0.0f);
//        result.push_back(0.0f);
//    }
//
//    return result;
//}