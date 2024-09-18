//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//#include <cmath>
//
//// Screen size
//const int SCREEN_WIDTH = 800;
//const int SCREEN_HEIGHT = 600;
//
//// Circle parameters
//float centerX = SCREEN_WIDTH / 2.0f;
//float centerY = SCREEN_HEIGHT / 2.0f;
//float radius = 200.0f;
//
//// Vertex Shader source
//const char* vertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec2 aPos;
//void main() {
//    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
//}
//)";
//
//// Fragment Shader source
//const char* fragmentShaderSource = R"(
//#version 330 core
//out vec4 FragColor;
//void main() {
//    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Orange color
//}
//)";
//
//// Function to compile shader and check for errors
//GLuint compileShader(GLenum type, const char* source) {
//    GLuint shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//
//    // Check for shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    return shader;
//}
//
//// Function to plot the circle points using Midpoint Circle Algorithm
//void plotCirclePoints(float xc, float yc, float x, float y, std::vector<float>& vertices) {
//    // Normalize coordinates to the range [-1, 1]
//    vertices.push_back((xc + x) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc + y) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc - x) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc + y) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc + x) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc - y) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc - x) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc - y) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc + y) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc + x) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc - y) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc + x) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc + y) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc - x) / SCREEN_HEIGHT * 2 - 1);
//
//    vertices.push_back((xc - y) / SCREEN_WIDTH * 2 - 1);
//    vertices.push_back((yc - x) / SCREEN_HEIGHT * 2 - 1);
//}
//
//// Function to generate circle points using Midpoint Circle Algorithm
//void generateCircle(float xc, float yc, float radius, std::vector<float>& vertices) {
//    int x = 0;
//    int y = radius;
//    int p = 1 - radius;
//
//    // Plot initial points
//    plotCirclePoints(xc, yc, x, y, vertices);
//
//    while (x < y) {
//        x++;
//        if (p < 0) {
//            p += 2 * x + 1;
//        }
//        else {
//            y--;
//            p += 2 * (x - y) + 1;
//        }
//        plotCirclePoints(xc, yc, x, y, vertices);
//    }
//}
//
//// Main function
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create window
//    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Circle Drawing with Midpoint Algorithm", nullptr, nullptr);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//
//    // Compile shaders
//    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
//    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Generate circle vertices
//    std::vector<float> circleVertices;
//    generateCircle(centerX, centerY, radius, circleVertices);
//
//    // Set up OpenGL buffers
//    GLuint VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Render loop
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_POINTS, 0, circleVertices.size() / 2); // Use GL_POINTS for rendering the circle
//        glBindVertexArray(0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
