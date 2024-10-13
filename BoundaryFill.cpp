//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//#include <stack>
//#include <thread>
//
//// Window dimensions
//const GLint WIDTH = 800, HEIGHT = 600;
//
//// Fill and boundary color
//float boundaryColor[] = { 0.0f, 0.0f, 0.0f };  // Black boundary
//float fillColor[] = { 1.0f, 0.0f, 0.0f };      // Red fill color
//
//// Shader source code (Vertex and Fragment Shaders)
//const char* vertexShaderSource = R"(
//#version 330 core
//layout(location = 0) in vec2 position;
//out vec2 TexCoords;
//void main() {
//    TexCoords = (position + 1.0) / 2.0;  // Map from [-1,1] to [0,1] for texture coordinates
//    gl_Position = vec4(position, 0.0, 1.0);
//}
//)";
//
//const char* fragmentShaderSource = R"(
//#version 330 core
//in vec2 TexCoords;
//out vec4 color;
//uniform sampler2D screenTexture;
//
//void main() {
//    color = texture(screenTexture, TexCoords);
//}
//)";
//
//// Function to compile and link shaders
//GLuint CompileShaders() {
//    GLint success;
//    GLchar infoLog[512];
//
//    // Vertex Shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glCompileShader(vertexShader);
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Fragment Shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    glCompileShader(fragmentShader);
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Shader Program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//// Create VAO and VBO for a full-screen rectangle
//GLuint createRectangle() {
//    GLfloat vertices[] = {
//        // Positions
//        -0.5f, -0.5f,
//         0.5f, -0.5f,
//         0.5f,  0.5f,
//        -0.5f,  0.5f
//    };
//
//    GLuint indices[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    GLuint VAO, VBO, EBO;
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    // Vertex Buffer
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Element Buffer
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Vertex Attributes
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    return VAO;
//}
//
//// Get the pixel color from the texture
//void getPixelColor(int x, int y, std::vector<unsigned char>& pixels, int width, int height, unsigned char color[3]) {
//    int index = (y * width + x) * 3;
//    color[0] = pixels[index];     // Red
//    color[1] = pixels[index + 1]; // Green
//    color[2] = pixels[index + 2]; // Blue
//}
//
//// Set the pixel color in the texture
//void setPixelColor(int x, int y, std::vector<unsigned char>& pixels, int width, int height, unsigned char color[3]) {
//    int index = (y * width + x) * 3;
//    pixels[index] = color[0];     // Red
//    pixels[index + 1] = color[1]; // Green
//    pixels[index + 2] = color[2]; // Blue
//}
//
//void boundaryFill(int x, int y, std::vector<unsigned char>& pixels, int width, int height, unsigned char fillColor[3], unsigned char boundaryColor[3], GLuint screenTexture) {
//    std::stack<std::pair<int, int>> pixelStack;
//    pixelStack.push(std::make_pair(x, y));
//
//    while (!pixelStack.empty()) {
//        std::pair<int, int> pixel = pixelStack.top();
//        pixelStack.pop();
//
//        int px = pixel.first;
//        int py = pixel.second;
//
//        unsigned char currentColor[3];
//        getPixelColor(px, py, pixels, width, height, currentColor);
//
//        // If the current pixel is not the boundary and not already filled
//        if ((currentColor[0] != boundaryColor[0] || currentColor[1] != boundaryColor[1] || currentColor[2] != boundaryColor[2]) &&
//            (currentColor[0] != fillColor[0] || currentColor[1] != fillColor[1] || currentColor[2] != fillColor[2])) {
//
//            // Fill the current pixel
//            setPixelColor(px, py, pixels, width, height, fillColor);
//
//            // Update the texture with the new pixel data
//            glBindTexture(GL_TEXTURE_2D, screenTexture);
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
//
//            // Add a delay to slow down the filling process
//            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Adjust the duration as needed
//
//            // Push the neighboring pixels onto the stack
//            if (px + 1 < width) pixelStack.push(std::make_pair(px + 1, py));   // Right
//            if (px - 1 >= 0)  pixelStack.push(std::make_pair(px - 1, py));     // Left
//            if (py + 1 < height) pixelStack.push(std::make_pair(px, py + 1));  // Top
//            if (py - 1 >= 0)  pixelStack.push(std::make_pair(px, py - 1));     // Bottom
//        }
//    }
//}
//
//
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // Set GLFW options for OpenGL 3.3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a GLFW window
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modern OpenGL Boundary Fill", nullptr, nullptr);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    // Initialize GLAD
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Set the viewport
//    glViewport(0, 0, WIDTH, HEIGHT);
//
//    // Compile shaders
//    GLuint shaderProgram = CompileShaders();
//    glUseProgram(shaderProgram);
//
//    // Create a rectangle boundary
//    GLuint rectangleVAO = createRectangle();
//
//    // Create a texture for the screen
//    std::vector<unsigned char> pixels(WIDTH * HEIGHT * 3, 255);  // White background
//    GLuint screenTexture;
//    glGenTextures(1, &screenTexture);
//    glBindTexture(GL_TEXTURE_2D, screenTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//    bool isFilling = false;  // Flag to indicate filling state
//
//    while (!glfwWindowShouldClose(window)) {
//        // Render the rectangle with the texture
//        glClear(GL_COLOR_BUFFER_BIT);
//        glBindTexture(GL_TEXTURE_2D, screenTexture);
//        glBindVertexArray(rectangleVAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        // Handle mouse input for filling
//        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !isFilling) {
//            double xpos, ypos;
//            glfwGetCursorPos(window, &xpos, &ypos);
//            int ix = static_cast<int>(xpos);
//            int iy = HEIGHT - static_cast<int>(ypos);  // Invert Y coordinate
//
//            unsigned char fill[3] = { 255, 0, 0 };     // Red fill color
//            unsigned char boundary[3] = { 0, 0, 0 };  // Black boundary
//
//            isFilling = true;  // Set the filling flag
//            boundaryFill(ix, iy, pixels, WIDTH, HEIGHT, fill, boundary, screenTexture); // Updated call
//            isFilling = false; // Reset the filling flag
//        }
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//
//        // Poll for and process events
//        glfwPollEvents();
//    }
//
//    // Clean up
//    glDeleteTextures(1, &screenTexture);
//    glDeleteVertexArrays(1, &rectangleVAO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
