//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <queue>
//#include <cmath>
//
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// Shader sources
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec2 aPos;
//    void main()
//    {
//        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform vec3 color;
//    void main()
//    {
//        FragColor = vec4(color, 1.0);
//    }
//)";
//
//// Global variables
//GLuint shaderProgram;
//GLuint VAO, VBO;
//std::vector<std::vector<bool>> shape;
//std::vector<std::vector<bool>> filled;
//glm::mat4 projection;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//void drawShape() {
//    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 1.0f, 1.0f);
//    glPointSize(1.0f);
//    glBegin(GL_POINTS);
//    for (int y = 0; y < SCR_HEIGHT; ++y) {
//        for (int x = 0; x < SCR_WIDTH; ++x) {
//            if (shape[y][x]) {
//                float nx = (float)x / SCR_WIDTH * 2 - 1;
//                float ny = (float)y / SCR_HEIGHT * 2 - 1;
//                glVertex2f(nx, ny);
//            }
//        }
//    }
//    glEnd();
//}
//
//void drawFilled() {
//    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 1.0f, 0.0f);
//    glPointSize(1.0f);
//    glBegin(GL_POINTS);
//    for (int y = 0; y < SCR_HEIGHT; ++y) {
//        for (int x = 0; x < SCR_WIDTH; ++x) {
//            if (filled[y][x]) {
//                float nx = (float)x / SCR_WIDTH * 2 - 1;
//                float ny = (float)y / SCR_HEIGHT * 2 - 1;
//                glVertex2f(nx, ny);
//            }
//        }
//    }
//    glEnd();
//}
//
//void floodFill(int x, int y) {
//    if (x < 0 || x >= SCR_WIDTH || y < 0 || y >= SCR_HEIGHT || shape[y][x] || filled[y][x]) {
//        return;
//    }
//
//    std::queue<std::pair<int, int>> q;
//    q.push({ x, y });
//    filled[y][x] = true;
//
//    while (!q.empty()) {
//        int cx = q.front().first;
//        int cy = q.front().second;
//        q.pop();
//
//        for (int dx = -1; dx <= 1; dx++) {
//            for (int dy = -1; dy <= 1; dy++) {
//                int nx = cx + dx;
//                int ny = cy + dy;
//                if (nx >= 0 && nx < SCR_WIDTH && ny >= 0 && ny < SCR_HEIGHT && !shape[ny][nx] && !filled[ny][nx]) {
//                    filled[ny][nx] = true;
//                    q.push({ nx, ny });
//                }
//            }
//        }
//    }
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//        double xpos, ypos;
//        glfwGetCursorPos(window, &xpos, &ypos);
//
//        int x = static_cast<int>(xpos);
//        int y = SCR_HEIGHT - static_cast<int>(ypos) - 1;
//
//        if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
//            floodFill(x, y);
//        }
//    }
//}
//
//void createStarShape() {
//    int centerX = SCR_WIDTH / 2;
//    int centerY = SCR_HEIGHT / 2;
//    int outerRadius = std::min(SCR_WIDTH, SCR_HEIGHT) / 3;
//    int innerRadius = outerRadius / 2;
//
//    for (int i = 0; i < 10; i++) {
//        float angle = i * M_PI / 5;
//        int radius = (i % 2 == 0) ? outerRadius : innerRadius;
//        int x = centerX + static_cast<int>(radius * std::cos(angle));
//        int y = centerY + static_cast<int>(radius * std::sin(angle));
//
//        if (i == 0) {
//            for (int dy = 0; dy < SCR_HEIGHT; ++dy) {
//                for (int dx = 0; dx < SCR_WIDTH; ++dx) {
//                    int nx = centerX + (x - centerX) * dx / SCR_WIDTH;
//                    int ny = centerY + (y - centerY) * dy / SCR_HEIGHT;
//                    if (nx >= 0 && nx < SCR_WIDTH && ny >= 0 && ny < SCR_HEIGHT) {
//                        shape[ny][nx] = true;
//                    }
//                }
//            }
//        }
//        else {
//            int prevX = centerX + static_cast<int>(((i % 2 == 0) ? innerRadius : outerRadius) * std::cos((i - 1) * M_PI / 5));
//            int prevY = centerY + static_cast<int>(((i % 2 == 0) ? innerRadius : outerRadius) * std::sin((i - 1) * M_PI / 5));
//
//            for (int t = 0; t <= 100; ++t) {
//                int nx = prevX + (x - prevX) * t / 100;
//                int ny = prevY + (y - prevY) * t / 100;
//                if (nx >= 0 && nx < SCR_WIDTH && ny >= 0 && ny < SCR_HEIGHT) {
//                    shape[ny][nx] = true;
//                }
//            }
//        }
//    }
//}
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Shape and Flood Fill", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//
//    // Initialize GLEW
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "Failed to initialize GLEW" << std::endl;
//        return -1;
//    }
//
//    // Compile shaders
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Initialize shape and filled vectors
//    shape.resize(SCR_HEIGHT, std::vector<bool>(SCR_WIDTH, false));
//    filled.resize(SCR_HEIGHT, std::vector<bool>(SCR_WIDTH, false));
//
//    // Create star shape
//    createStarShape();
//
//    // Main loop
//    while (!glfwWindowShouldClose(window)) {
//        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//
//        drawShape();
//        drawFilled();
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}