//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <cstdlib>
//
//// Window dimensions
//const unsigned int WIDTH = 800, HEIGHT = 600;
//
//// Variables to track mouse positions and drawing state
//double startX, startY, endX, endY;
//bool isDrawing = false;
//bool isDrawingLine = true; // Track if we're drawing a line or rectangle
//int clickCount = 0; // Count clicks for line drawing
//
//// Store the clipping rectangle
//struct Rectangle {
//    float left, right, bottom, top;
//};
//
//Rectangle clipRect = { 0.0f, 0.0f, 0.0f, 0.0f };
//
//// Store the line to be clipped
//struct Line {
//    float x1, y1, x2, y2;
//};
//
//Line userLine = { 0.0f, 0.0f, 0.0f, 0.0f }; // Initialize user-drawn line
//
//// Shader sources
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec2 aPos;
//    void main() {
//        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform vec3 color;
//    void main() {
//        FragColor = vec4(color, 1.0);
//    }
//)";
//
//// Function prototypes
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//unsigned int createShaderProgram();
//void drawLine(const Line& line, const glm::vec3& color, unsigned int VAO, unsigned int VBO, unsigned int shaderProgram);
//void clipLineToRectangle(const Line& line, const Rectangle& rect, std::vector<Line>& insideSegments, std::vector<Line>& outsideSegments);
//
//// Cohen-Sutherland algorithm for line clipping
//const int INSIDE = 0; // 0000
//const int LEFT = 1;   // 0001
//const int RIGHT = 2;  // 0010
//const int BOTTOM = 4; // 0100
//const int TOP = 8;    // 1000
//
//int computeOutCode(float x, float y, const Rectangle& rect) {
//    int code = INSIDE;
//    if (x < rect.left)
//        code |= LEFT;
//    else if (x > rect.right)
//        code |= RIGHT;
//    if (y < rect.bottom)
//        code |= BOTTOM;
//    else if (y > rect.top)
//        code |= TOP;
//    return code;
//}
//
//void clipLineToRectangle(const Line& line, const Rectangle& rect, std::vector<Line>& insideSegments, std::vector<Line>& outsideSegments) {
//    float x1 = line.x1, y1 = line.y1, x2 = line.x2, y2 = line.y2;
//    int outcode1 = computeOutCode(x1, y1, rect);
//    int outcode2 = computeOutCode(x2, y2, rect);
//    bool accept = false;
//
//    while (true) {
//        if (!(outcode1 | outcode2)) {
//            accept = true;
//            break;
//        }
//        else if (outcode1 & outcode2) {
//            break;
//        }
//        else {
//            int outcodeOut = outcode1 ? outcode1 : outcode2;
//            float x, y;
//
//            if (outcodeOut & TOP) {
//                x = x1 + (x2 - x1) * (rect.top - y1) / (y2 - y1);
//                y = rect.top;
//            }
//            else if (outcodeOut & BOTTOM) {
//                x = x1 + (x2 - x1) * (rect.bottom - y1) / (y2 - y1);
//                y = rect.bottom;
//            }
//            else if (outcodeOut & RIGHT) {
//                y = y1 + (y2 - y1) * (rect.right - x1) / (x2 - x1);
//                x = rect.right;
//            }
//            else if (outcodeOut & LEFT) {
//                y = y1 + (y2 - y1) * (rect.left - x1) / (x2 - x1);
//                x = rect.left;
//            }
//
//            if (outcodeOut == outcode1) {
//                x1 = x;
//                y1 = y;
//                outcode1 = computeOutCode(x1, y1, rect);
//            }
//            else {
//                x2 = x;
//                y2 = y;
//                outcode2 = computeOutCode(x2, y2, rect);
//            }
//        }
//    }
//
//    if (accept) {
//        insideSegments.push_back({ x1, y1, x2, y2 });
//        if (x1 != line.x1 || y1 != line.y1) {
//            outsideSegments.push_back({ line.x1, line.y1, x1, y1 });
//        }
//        if (x2 != line.x2 || y2 != line.y2) {
//            outsideSegments.push_back({ x2, y2, line.x2, line.y2 });
//        }
//    }
//    else {
//        outsideSegments.push_back(line);
//    }
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        if (action == GLFW_PRESS) {
//            double xpos, ypos;
//            glfwGetCursorPos(window, &xpos, &ypos);
//            ypos = HEIGHT - ypos; // Invert Y axis
//
//            if (isDrawingLine) {
//                if (clickCount == 0) {
//                    userLine.x1 = xpos / WIDTH * 2 - 1;
//                    userLine.y1 = ypos / HEIGHT * 2 - 1;
//                    clickCount++;
//                }
//                else {
//                    userLine.x2 = xpos / WIDTH * 2 - 1;
//                    userLine.y2 = ypos / HEIGHT * 2 - 1;
//                    clickCount = 0;
//                    isDrawingLine = false; // Switch to rectangle drawing mode
//                }
//            }
//            else {
//                startX = xpos;
//                startY = ypos;
//                isDrawing = true;
//            }
//        }
//        else if (action == GLFW_RELEASE && !isDrawingLine) {
//            glfwGetCursorPos(window, &endX, &endY);
//            endY = HEIGHT - endY; // Invert Y axis
//            isDrawing = false;
//
//            // Update the clipping rectangle
//            clipRect.left = std::min((float)startX, (float)endX) / WIDTH * 2 - 1;
//            clipRect.right = std::max((float)startX, (float)endX) / WIDTH * 2 - 1;
//            clipRect.bottom = std::min((float)startY, (float)endY) / HEIGHT * 2 - 1;
//            clipRect.top = std::max((float)startY, (float)endY) / HEIGHT * 2 - 1;
//        }
//    }
//}
//
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (isDrawing && !isDrawingLine) {
//        endX = xpos;
//        endY = HEIGHT - ypos; // Invert Y axis
//    }
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//unsigned int createShaderProgram() {
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//void drawLine(const Line& line, const glm::vec3& color, unsigned int VAO, unsigned int VBO, unsigned int shaderProgram) {
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    float vertices[] = {
//        line.x1, line.y1,
//        line.x2, line.y2
//    };
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
//
//    glUseProgram(shaderProgram);
//    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));
//
//    glDrawArrays(GL_LINES, 0, 2);
//    glBindVertexArray(0);
//}
//
//int main() {
//    // Initialize GLFW
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a GLFW window
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Line Clipping", NULL, NULL);
//    if (window == NULL) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLAD
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Set callbacks
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//    glfwSetCursorPosCallback(window, cursor_position_callback);
//
//    // Create and compile shaders
//    unsigned int shaderProgram = createShaderProgram();
//
//    // Set up vertex data and buffers
//    unsigned int VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, NULL, GL_DYNAMIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Main render loop
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Draw the user-drawn line
//        if (!isDrawingLine) {
//            drawLine(userLine, glm::vec3(0.0f, 0.0f, 1.0f), VAO, VBO, shaderProgram); // Blue color for user line
//        }
//
//        // Draw the clipping rectangle outline
//        if (isDrawing || (clipRect.left != clipRect.right && clipRect.bottom != clipRect.top)) {
//            float left = isDrawing ? std::min((float)startX, (float)endX) / WIDTH * 2 - 1 : clipRect.left;
//            float right = isDrawing ? std::max((float)startX, (float)endX) / WIDTH * 2 - 1 : clipRect.right;
//            float bottom = isDrawing ? std::min((float)startY, (float)endY) / HEIGHT * 2 - 1 : clipRect.bottom;
//            float top = isDrawing ? std::max((float)startY, (float)endY) / HEIGHT * 2 - 1 : clipRect.top;
//
//            Line rectLines[] = {
//                {left, bottom, right, bottom},
//                {right, bottom, right, top},
//                {right, top, left, top},
//                {left, top, left, bottom}
//            };
//
//            for (const auto& rectLine : rectLines) {
//                drawLine(rectLine, glm::vec3(1.0f, 1.0f, 1.0f), VAO, VBO, shaderProgram); // White color for rectangle
//            }
//        }
//
//        // Clip the line and draw segments
//        if (!isDrawingLine && clipRect.left != clipRect.right && clipRect.bottom != clipRect.top) {
//            std::vector<Line> insideSegments, outsideSegments;
//            clipLineToRectangle(userLine, clipRect, insideSegments, outsideSegments);
//
//            for (const auto& segment : insideSegments) {
//                drawLine(segment, glm::vec3(0.0f, 1.0f, 0.0f), VAO, VBO, shaderProgram); // Green for inside
//            }
//            for (const auto& segment : outsideSegments) {
//                drawLine(segment, glm::vec3(1.0f, 0.0f, 0.0f), VAO, VBO, shaderProgram); // Red for outside
//            }
//        }
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}