#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
uniform mat4 transform;
void main() {
    gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 color;
void main() {
    FragColor = color;
}
)";

struct BezierCurve {
    std::vector<glm::vec2> controlPoints;
    std::vector<glm::vec2> originalPoints;  // Store original positions
    glm::vec4 color;
    std::vector<float> vertices;
    bool selected;

    BezierCurve() : color(1.0f, 1.0f, 1.0f, 1.0f), selected(false) {
        controlPoints = {
            glm::vec2(-0.5f, 0.0f),
            glm::vec2(-0.25f, 0.5f),
            glm::vec2(0.25f, 0.5f),
            glm::vec2(0.5f, 0.0f)
        };
        originalPoints = controlPoints;  // Save original positions
    }
};

class BezierEditor {
private:
    std::vector<BezierCurve> curves;
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    const int numPoints = 60;
    int windowWidth = 1200;  // Default window width
    int windowHeight = 800;  // Default window height

    // Animation properties
    bool isAnimating = false;
    float animationTime = 0.0f;
    const float animationDuration = 2.0f;  // Animation duration in seconds
    float lastFrameTime = 0.0f;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        auto* editor = static_cast<BezierEditor*>(glfwGetWindowUserPointer(window));
        editor->windowWidth = width;
        editor->windowHeight = height;
    }

    glm::mat4 calculateTransformMatrix() {
        float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        glm::mat4 transform = glm::mat4(1.0f);

        // If window is wider than it is tall
        if (aspectRatio > 1.0f) {
            transform = glm::scale(transform, glm::vec3(1.0f / aspectRatio, 1.0f, 1.0f));
        }
        // If window is taller than it is wide
        else {
            transform = glm::scale(transform, glm::vec3(1.0f, aspectRatio, 1.0f));
        }

        // Apply a consistent scale factor
        transform = glm::scale(transform, glm::vec3(0.8f));

        return transform;
    }

    glm::vec2 calculateBezierPoint(float t, const std::vector<glm::vec2>& controlPoints) {
        if (controlPoints.size() != 4) return glm::vec2(0.0f);

        float u = 1.0f - t;
        return u * u * u * controlPoints[0] +
            3.0f * u * u * t * controlPoints[1] +
            3.0f * u * t * t * controlPoints[2] +
            t * t * t * controlPoints[3];
    }

    void updateCurveVertices(BezierCurve& curve) {
        curve.vertices.clear();

        // Generate bezier curve points
        for (int i = 0; i <= numPoints; i++) {
            float t = static_cast<float>(i) / numPoints;
            glm::vec2 point = calculateBezierPoint(t, curve.controlPoints);
            curve.vertices.push_back(point.x);
            curve.vertices.push_back(point.y);
        }
    }

    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Initialize with empty buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1024, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void updateBuffers() {
        std::vector<float> allVertices;
        for (const auto& curve : curves) {
            allVertices.insert(allVertices.end(), curve.vertices.begin(), curve.vertices.end());
        }

        if (!allVertices.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), allVertices.data(), GL_DYNAMIC_DRAW);
        }
    }

    void updateAnimation() {
        if (!isAnimating) return;

        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        animationTime += deltaTime;
        float t = animationTime / animationDuration;

        if (t >= 1.0f) {
            // Reset animation
            isAnimating = false;
            animationTime = 0.0f;
            // Reset all curves to their original positions
            for (auto& curve : curves) {
                curve.controlPoints = curve.originalPoints;
                updateCurveVertices(curve);
            }
            updateBuffers();
            return;
        }

        // Animate all curves
        for (auto& curve : curves) {
            // Calculate current position for each control point
            for (size_t i = 1; i < 3; i++) {  // Only animate middle control points
                glm::vec2 startPos = curve.originalPoints[i];
                glm::vec2 endPos = curve.originalPoints[3];  // Move towards P3

                // Use smooth step for more pleasing animation
                float smoothT = t * t * (3.0f - 2.0f * t);
                curve.controlPoints[i] = glm::mix(startPos, endPos, smoothT);
            }
            updateCurveVertices(curve);
        }
        updateBuffers();
    }

public:
    BezierEditor(GLFWwindow* window) {
        // Set the user pointer to this instance
        glfwSetWindowUserPointer(window, this);

        // Set the framebuffer size callback
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        // Create and compile shaders
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        initializeBuffers();
        lastFrameTime = static_cast<float>(glfwGetTime());
    }

    void renderUI() {
        ImGui::Begin("Bezier Curve Editor");

        if (ImGui::Button("Add New Curve")) {
            curves.push_back(BezierCurve());
            updateCurveVertices(curves.back());
            updateBuffers();
        }

        ImGui::SameLine();

        if (ImGui::Button("Animate Points")) {
            if (!isAnimating) {
                isAnimating = true;
                animationTime = 0.0f;
                // Store current positions as original positions
                for (auto& curve : curves) {
                    curve.originalPoints = curve.controlPoints;
                }
            }
        }

        for (int i = 0; i < curves.size(); i++) {
            ImGui::PushID(i);
            if (ImGui::TreeNode(("Curve " + std::to_string(i)).c_str())) {
                auto& curve = curves[i];
                bool updated = false;

                for (int j = 0; j < 4; j++) {
                    ImGui::PushID(j);
                    ImGui::Text("Control Point %d", j);
                    updated |= ImGui::DragFloat("X", &curve.controlPoints[j].x, 0.01f, -1.0f, 1.0f);
                    updated |= ImGui::DragFloat("Y", &curve.controlPoints[j].y, 0.01f, -1.0f, 1.0f);
                    ImGui::PopID();
                }

                updated |= ImGui::ColorEdit4("Curve Color", &curve.color[0]);

                if (updated) {
                    updateCurveVertices(curve);
                    updateBuffers();
                }

                if (ImGui::Button("Delete Curve")) {
                    curves.erase(curves.begin() + i);
                    updateBuffers();
                    ImGui::TreePop();
                    ImGui::PopID();
                    break;
                }

                ImGui::TreePop();
            }
            ImGui::PopID();
        }

        ImGui::End();
    }

    void render() {
        updateAnimation();  // Update animation state

        if (curves.empty()) return;

        glUseProgram(shaderProgram);

        // Use the aspect-ratio-aware transform
        glm::mat4 transform = calculateTransformMatrix();
        int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);

        for (const auto& curve : curves) {
            int colorLoc = glGetUniformLocation(shaderProgram, "color");

            // Update buffer with curve points
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, curve.vertices.size() * sizeof(float), curve.vertices.data(), GL_DYNAMIC_DRAW);

            // Draw the bezier curve
            glUniform4fv(colorLoc, 1, glm::value_ptr(curve.color));
            glLineWidth(3.0f);
            glDrawArrays(GL_LINE_STRIP, 0, curve.vertices.size() / 2);

            // Draw control points
            glPointSize(10.0f);
            glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // White color for control points

            // Draw each control point
            for (const auto& point : curve.controlPoints) {
                float pointVertex[] = { point.x, point.y };
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertex), pointVertex, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }
    }

    ~BezierEditor() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Bezier Curve Editor", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    // Enable anti-aliasing and blending
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BezierEditor editor(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render UI and curves
        editor.renderUI();
        editor.render();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}