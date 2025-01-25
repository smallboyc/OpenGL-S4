#include <glm/glm.hpp>
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

// DISK with IBO

int window_width  = 1000;
int window_height = 700;

struct Vertex2DColor {
    Vertex2DColor() {}
    Vertex2DColor(const glm::vec2 position, const glm::vec3 color)
        : m_position(position), m_color(color) {}
    glm::vec2 m_position;
    glm::vec3 m_color;
};

void add_triangles(float radius, float teta, int number_of_triangles, std::vector<Vertex2DColor>& circle_vertex_buffer)
{
    glm::vec3     center_color{0.0, 0.0, 1.0};
    glm::vec3     vertex_color{0.0, 1.0, 0.5};
    Vertex2DColor centered_vertex = {{0.0f, 0.0f}, center_color};
    circle_vertex_buffer.push_back(centered_vertex);
    for (int i = 0; i < number_of_triangles; i++)
    {
        float         x = radius * glm::cos(i * teta);
        float         y = radius * glm::sin(i * teta);
        glm::vec2     current_position{x, y};
        Vertex2DColor current_vertex = {current_position, vertex_color};
        circle_vertex_buffer.push_back(current_vertex);
    }
}

void add_index(const int number_of_triangles, std::vector<glm::uint32_t>& circle_index_buffer)
{
    for (int i = 0; i < number_of_triangles; i++)
    {
        circle_index_buffer.push_back(0);
        circle_index_buffer.push_back(i + 1);
        circle_index_buffer.push_back((i + 2 > number_of_triangles) ? 1 : i + 2);
    }
}

int main(int argc, char* argv[])
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window =
        glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program =
        loadProgram(applicationPath.dirPath() + "TP1/shaders/triangle.vs.glsl", applicationPath.dirPath() + "TP1/shaders/triangle.fs.glsl");
    program.use();

    int   number_of_triangles = 41;
    float teta                = 2 * glm::pi<float>() / number_of_triangles;
    float radius              = 0.25f;

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<Vertex2DColor> circle_vertex_buffer;
    add_triangles(radius, teta, number_of_triangles, circle_vertex_buffer);
    std::cout << circle_vertex_buffer.size() << "\n";
    glBufferData(GL_ARRAY_BUFFER, circle_vertex_buffer.size() * sizeof(Vertex2DColor), circle_vertex_buffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    std::vector<glm::uint32_t> circle_index_buffer;
    add_index(number_of_triangles, circle_index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, circle_index_buffer.size() * sizeof(glm::uint32_t), circle_index_buffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const void*)(offsetof(Vertex2DColor, m_position)));
    const GLuint VERTEX_ATTR_COLOR = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const void*)(offsetof(Vertex2DColor, m_color)));
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3 * number_of_triangles, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vao);
    glfwTerminate();
    return 0;
}
