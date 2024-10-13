//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//#include <cstdlib> // for rand()
//
//// Window dimensions
//const unsigned int WIDTH = 800, HEIGHT = 600;
//
//// Variables to track mouse positions and drawing state
//double startX, startY, endX, endY;
//bool isDrawing = false;
//
//// Store rectangles and their colors
//struct Rectangle {
//    std::vector<float> vertices;
//    float color[3]; // RGB color
//    bool filled; // Whether the rectangle is filled
//};
//
//std::vector<Rectangle> rectangles;
//
//// Shader sources
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec2 aPos;
//
//    void main() {
//        gl_Position = vec4(aPos, 0.0, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform vec3 color;
//
//    void main() {
//        FragColor = vec4(color, 1.0); // Use the passed color
//    }
//)";
//
//// Function to adjust viewport on window resize
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//// Function to generate a random color
//void randomColor(float color[3]) {
//    color[0] = static_cast<float>(rand()) / RAND_MAX; // R
//    color[1] = static_cast<float>(rand()) / RAND_MAX; // G
//    color[2] = static_cast<float>(rand()) / RAND_MAX; // B
//}
//
//// Mouse button callback to handle mouse press and release
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        if (action == GLFW_PRESS) {
//            // On mouse press, get the starting position
//            glfwGetCursorPos(window, &startX, &startY);
//            startY = HEIGHT - startY; // Invert Y axis
//            isDrawing = true; // Set drawing mode to true
//        }
//        else if (action == GLFW_RELEASE) {
//            // On mouse release, get the ending position
//            glfwGetCursorPos(window, &endX, &endY);
//            endY = HEIGHT - endY; // Invert Y axis
//            isDrawing = false; // Stop drawing mode
//
//            // Store the rectangle vertices and assign a color
//            Rectangle rect;
//            std::vector<float> rectangleVertices = {
//                (float)startX / WIDTH * 2 - 1, (float)startY / HEIGHT * 2 - 1,   // Bottom-left
//                (float)endX / WIDTH * 2 - 1, (float)startY / HEIGHT * 2 - 1,     // Bottom-right
//                (float)endX / WIDTH * 2 - 1, (float)endY / HEIGHT * 2 - 1,       // Top-right
//                (float)startX / WIDTH * 2 - 1, (float)endY / HEIGHT * 2 - 1      // Top-left
//            };
//            rect.vertices = rectangleVertices;
//            rect.filled = false; // Initially, the rectangle is not filled
//            randomColor(rect.color); // Assign a random color for filling later
//            rectangles.push_back(rect);
//        }
//    }
//    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
//        // Right click to fill a rectangle
//        glfwGetCursorPos(window, &startX, &startY);
//        startY = HEIGHT - startY; // Invert Y axis
//
//        // Check if the click is within any rectangle
//        for (auto& rect : rectangles) {
//            float xMin = (rect.vertices[0] + 1) * WIDTH / 2;
//            float xMax = (rect.vertices[2] + 1) * WIDTH / 2;
//            float yMin = (rect.vertices[1] + 1) * HEIGHT / 2;
//            float yMax = (rect.vertices[5] + 1) * HEIGHT / 2;
//
//            if (startX >= xMin && startX <= xMax && startY >= yMin && startY <= yMax) {
//                rect.filled = true; // Mark the rectangle as filled
//                randomColor(rect.color); // Change the color of the rectangle
//                break; // Fill only the first rectangle found
//            }
//        }
//    }
//}
//
//// Cursor position callback to update the end position while dragging
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (isDrawing) {
//        endX = xpos;
//        endY = HEIGHT - ypos; // Invert Y axis
//    }
//}
//
//// Create and compile shaders
//unsigned int createShaderProgram() {
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    int success;
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//// Function to draw rectangles using modern OpenGL
//void drawRectangle(const Rectangle& rect, unsigned int VAO, unsigned int VBO, unsigned int shaderProgram) {
//    // Bind the VAO and VBO
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    // Update vertex buffer data with rectangle coordinates
//    glBufferData(GL_ARRAY_BUFFER, rect.vertices.size() * sizeof(float), rect.vertices.data(), GL_DYNAMIC_DRAW);
//
//    // Use the shader program
//    glUseProgram(shaderProgram);
//
//    // Set the rectangle color
//    glUniform3f(glGetUniformLocation(shaderProgram, "color"), rect.color[0], rect.color[1], rect.color[2]);
//
//    // Draw as filled or outline
//    if (rect.filled) {
//        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Fill the rectangle
//    }
//    else {
//        glDrawArrays(GL_LINE_LOOP, 0, 4); // Draw outline
//    }
//
//    // Unbind the VAO
//    glBindVertexArray(0);
//}
//
//int main() {
//    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
//
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cout << "Failed to initialize GLFW!" << std::endl;
//        return -1;
//    }
//
//    // Set GLFW version to 3.3 and use core profile
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a window
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Click & Drag Rectangle", NULL, NULL);
//    if (!window) {
//        std::cout << "Failed to create GLFW window!" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // Load GLAD to manage OpenGL functions
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD!" << std::endl;
//        return -1;
//    }
//
//    // Set the viewport and callback for resizing window
//    glViewport(0, 0, WIDTH, HEIGHT);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // Set the mouse button and cursor callbacks
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//    glfwSetCursorPosCallback(window, cursor_position_callback);
//
//    // Create and compile shaders
//    unsigned int shaderProgram = createShaderProgram();
//
//    // Set up Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
//    unsigned int VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    // Bind VAO and VBO
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    // Define the vertex attributes
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Unbind VAO
//    glBindVertexArray(0);
//
//    // Main render loop
//    while (!glfwWindowShouldClose(window)) {
//        // Clear the screen
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Draw all saved rectangles
//        for (const auto& rect : rectangles) {
//            drawRectangle(rect, VAO, VBO, shaderProgram);
//        }
//
//        // If drawing mode is active, draw the rectangle dynamically
//        if (isDrawing) {
//            std::vector<float> dynamicRectangle = {
//                (float)startX / WIDTH * 2 - 1, (float)startY / HEIGHT * 2 - 1,   // Bottom-left
//                (float)endX / WIDTH * 2 - 1, (float)startY / HEIGHT * 2 - 1,     // Bottom-right
//                (float)endX / WIDTH * 2 - 1, (float)endY / HEIGHT * 2 - 1,       // Top-right
//                (float)startX / WIDTH * 2 - 1, (float)endY / HEIGHT * 2 - 1      // Top-left
//            };
//            Rectangle dynamicRect;
//            dynamicRect.vertices = dynamicRectangle;
//            dynamicRect.filled = false; // Initially, the rectangle is not filled
//            dynamicRect.color[0] = 1.0f; // Default color (white)
//            dynamicRect.color[1] = 1.0f;
//            dynamicRect.color[2] = 1.0f;
//            drawRectangle(dynamicRect, VAO, VBO, shaderProgram);
//        }
//
//        // Swap buffers and poll events
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Clean up and terminate GLFW
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glfwTerminate();
//    return 0;
//}
