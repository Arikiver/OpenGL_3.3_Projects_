#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// Vertex Shader Source Code
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout(location = 0) in vec2 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

// Fragment Shader Source Code
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)glsl";

std::vector<float> points; // Store line points

// Convert screen coordinates to NDC
float screenToNDCX(float x, int width) {
    return (2.0f * x) / width - 1.0f;
}

float screenToNDCY(float y, int height) {
    return 1.0f - (2.0f * y) / height;
}

// Generalized Bresenham's Line Algorithm to calculate points between two given points
void bresenhamLine(float x1, float y1, float x2, float y2, int width, int height) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int p = dx - dy;

    while (true) {
        // Store the current point after converting to NDC
        points.push_back(screenToNDCX(x1, width));
        points.push_back(screenToNDCY(y1, height));

        // If the start point reaches the end point, break the loop
        if (x1 == x2 && y1 == y2) {
            break;
        }

        // Calculate the por term and adjust points accordingly
        int e2 = 2 * p;
        if (e2 > -dy) {
            p -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            p += dx;
            y1 += sy;
        }
    }
}

// Function to compile and link shaders
unsigned int compileShaders() {
    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders to a program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::vector<std::pair<float, float>> clickedPoints; // Store clicked points

// Mouse button callback to handle clicks
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Store the point
        clickedPoints.emplace_back(xpos, ypos);

        // Window dimensions
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // If there are two points, draw a line
        if (clickedPoints.size() == 2) {
            // Convert to Cartesian coordinates
            float ndcX1 = screenToNDCX(static_cast<float>(clickedPoints[0].first), width);
            float ndcY1 = screenToNDCX(static_cast<float>(height - clickedPoints[0].second), height);
            float ndcX2 = screenToNDCX(static_cast<float>(clickedPoints[1].first), width);
            float ndcY2 = screenToNDCX(static_cast<float>(height - clickedPoints[1].second), height);
            // Print the coordinates of the line drawn
            std::cout << "line drawn from: (" << ndcX1 << ", " << ndcY1 << ") to (" << ndcX2 << ", " << ndcY2 << ") where the equation of the line drawn is: \n";
            std::cout << "y = " << (ndcY2 - ndcY1) / (ndcX2 - ndcX1) << "x + " << ndcY1 - (ndcY2 - ndcY1) / (ndcX2 - ndcX1) * ndcX1 << "\n";

            // Draw the line using Bresenham's algorithm
            bresenhamLine(clickedPoints[0].first, clickedPoints[0].second, clickedPoints[1].first, clickedPoints[1].second, width, height);

            clickedPoints.clear(); // Clear points for the next line
        }
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "Bresenham Line Drawing", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set viewport
    glViewport(0, 0, 800, 800);

    // Compile and use shaders
    unsigned int shaderProgram = compileShaders();
    glUseProgram(shaderProgram);

    // Set up vertex data and buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), points.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Register mouse button callback
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update VBO with new points data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), points.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Draw the lines
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, points.size() / 2);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
