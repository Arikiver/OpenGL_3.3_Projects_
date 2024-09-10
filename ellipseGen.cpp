#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

float a = 0.5f; // Semi-major axis
float b = 0.25f; // Semi-minor axis
bool isDragging = false; // Track if the mouse is being dragged
double prevMouseX = 0.0, prevMouseY = 0.0;

// Vertex Shader source
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

// Fragment Shader source
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);  // White color for the ellipse
    }
)";

// Function to compile shaders
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

// Function to create shader program
unsigned int createShaderProgram() {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Function to generate ellipse vertices
void generateEllipse(float* vertices, float x_center, float y_center, float a, float b, int num_segments) {
    for (int i = 0; i < num_segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);  // Current angle
        float x = a * cosf(theta);  // X coordinate
        float y = b * sinf(theta);  // Y coordinate
        vertices[2 * i] = x + x_center;
        vertices[2 * i + 1] = y + y_center;
    }
}

// Mouse callback for dragging
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        isDragging = true;
        glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isDragging = false;
    }
}

// Mouse movement callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        // Calculate the change in mouse position
        double dx = xpos - prevMouseX;
        double dy = ypos - prevMouseY;

        // Adjust a (semi-major axis) with horizontal movement and b (semi-minor axis) with vertical movement
        a += dx / WIDTH;
        b -= dy / HEIGHT;

        // Clamp the values to avoid negative or zero radii
        if (a < 0.05f) a = 0.05f;
        if (b < 0.05f) b = 0.05f;

        // Update previous mouse position
        prevMouseX = xpos;
        prevMouseY = ypos;
    }
}

// Callback to resize the OpenGL viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW: OpenGL version 3.3, Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ellipse Drawing - OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the OpenGL viewport and register the resize callback
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set up mouse input callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Compile and link shaders
    unsigned int shaderProgram = createShaderProgram();

    // Generate the ellipse vertices
    const int num_segments = 100;
    float vertices[2 * num_segments];  // 2D vertices: x and y for each segment

    // Create and bind VAO and VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Update ellipse vertices dynamically
        generateEllipse(vertices, 0.0f, 0.0f, a, b, num_segments);

        // Bind and buffer vertex data
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Vertex attribute setup
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program and draw the ellipse
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, num_segments);
        glBindVertexArray(0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
