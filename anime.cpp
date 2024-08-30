//#include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <iostream>
// #include <cmath> // For mathematical functions
// #include <chrono> // For timing control
// #include <random> // For random color generation
// #include <array> // For std::array

// // Vertex Shader Source Code
// const char* vertexShaderSource = R"glsl(
// #version 330 core
// layout (location = 0) in vec3 aPos;

// uniform mat4 transform;

// void main()
// {
//   gl_Position = transform * vec4(aPos, 1.0);
// }
// )glsl";

// // Fragment Shader Source Code
// const char* fragmentShaderSource = R"glsl(
// #version 330 core
// out vec4 FragColor;

// uniform vec4 color;

// void main()
// {
//   FragColor = color; // Use the color uniform
// }
// )glsl";

// // Function to compile shader and check for errors
// unsigned int compileShader(unsigned int type, const char* source) {
//     unsigned int id = glCreateShader(type);
//     glShaderSource(id, 1, &source, nullptr);
//     glCompileShader(id);

//     int success;
//     glGetShaderiv(id, GL_COMPILE_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetShaderInfoLog(id, 512, nullptr, infoLog);
//         std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
//     }
//     return id;
// }

// // Function to create shader program
// unsigned int createShaderProgram() {
//     unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
//     unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

//     unsigned int shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     int success;
//     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//     if (!success) {
//         char infoLog[512];
//         glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//         std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//     }

//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }

// // Function to generate a random color
// std::array<float, 4> generateRandomColor() {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<> dis(0.0, 1.0);

//     return { static_cast<float>(dis(gen)), static_cast<float>(dis(gen)), static_cast<float>(dis(gen)), 1.0f };
// }

// int main() {
//     // Initialize GLFW
//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW" << std::endl;
//         return -1;
//     }

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     // Create a window
//     GLFWwindow* window = glfwCreateWindow(800, 600, "Walking Man Animation", nullptr, nullptr);
//     if (window == nullptr) {
//         std::cerr << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     // Initialize GLAD
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         std::cerr << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     // Viewport setup
//     glViewport(0, 0, 800, 600);

//     // Compile and link shaders
//     unsigned int shaderProgram = createShaderProgram();

//     // Define the vertices and indices for the rectangle
//     float vertices[] = {
//         // positions        
//          0.1f,  0.3f, 0.0f,  // top right
//          0.1f, -0.3f, 0.0f,  // bottom right
//         -0.1f, -0.3f, 0.0f,  // bottom left
//         -0.1f,  0.3f, 0.0f   // top left 
//     };

//     unsigned int indices[] = {
//         0, 1, 3,  // first triangle
//         1, 2, 3   // second triangle
//     };

//     // Setup OpenGL buffers and vertex array object
//     unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);

//     glBindVertexArray(0);

//     // Animation variables
//     float offset = 0.0f;
//     bool direction = true;
//     float speed = 0.3f; // Adjust this value to change the speed

//     // Timer setup
//     auto lastTime = std::chrono::high_resolution_clock::now();

//     // Generate initial color
//     auto color = generateRandomColor();

//     // Render loop
//     while (!glfwWindowShouldClose(window)) {
//         // Calculate delta time
//         auto currentTime = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<float> deltaTime = currentTime - lastTime;
//         lastTime = currentTime;

//         // Input processing
//         if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//             glfwSetWindowShouldClose(window, true);

//         // Clear the screen
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         // Use shader program
//         glUseProgram(shaderProgram);

//         // transformation matrix
//         float transform[16] = {
//             1.0f, 0.0f, 0.0f, 0.0f,
//             0.0f, 1.0f, 0.0f, 0.0f,
//             0.0f, 0.0f, 1.0f, 0.0f,
//             offset, 0.0f, 0.0f, 1.0f
//         };

//         unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
//         glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

//         // Update color for shader
//         unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
//         glUniform4f(colorLoc, color[0], color[1], color[2], color[3]);

//         // Render the man
//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//         // Update offset for animation based on delta time
//         if (direction) {
//             offset += speed * deltaTime.count();
//         }
//         else {
//             offset -= speed * deltaTime.count();
//         }

//         if (offset > 0.5f || offset < -0.5f) {
//             direction = !direction;
//             color = generateRandomColor(); // Change color when direction changes
//         }

//         // Swap buffers and poll events
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     // Cleanup
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//     glDeleteProgram(shaderProgram);

//     glfwDestroyWindow(window);
//     glfwTerminate();

//     return 0;
// }
