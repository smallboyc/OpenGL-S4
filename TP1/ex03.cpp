#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

// DISK

int window_width = 1000;
int window_height = 700;

struct Vertex2DColor {
  Vertex2DColor() {}
  Vertex2DColor(const glm::vec2 position, const glm::vec3 color)
      : m_position(position), m_color(color) {}
  glm::vec2 m_position;
  glm::vec3 m_color;
};

std::vector<Vertex2DColor> CircleBuffer;

void add_triangles(float radius, float teta, int number_of_triangles) {
  glm::vec3 center_color{0.0, 0.0, 1.0};
  glm::vec3 vertex_color{0.0, 1.0, 0.5};
  Vertex2DColor centered_vertex = {{0.0f, 0.0f}, center_color};

  glm::vec2 first_position{radius * glm::cos(0), radius * glm::sin(0)};
  Vertex2DColor first_vertex = {first_position, vertex_color};
  Vertex2DColor previous_vertex = first_vertex;

  for (int i = 1; i <= number_of_triangles; ++i) {
    float x = radius * glm::cos(i * teta);
    float y = radius * glm::sin(i * teta);
    glm::vec2 current_position{x, y};
    Vertex2DColor current_vertex = {current_position, vertex_color};

    CircleBuffer.push_back(centered_vertex);
    CircleBuffer.push_back(previous_vertex);
    CircleBuffer.push_back(current_vertex);

    previous_vertex = current_vertex;
  }
}

int main(int argc, char *argv[]) {
  /* Initialize the library */
  if (!glfwInit()) {
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
  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Intialize glad (loads the OpenGL functions) */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  glimac::FilePath applicationPath(argv[0]);
  glimac::Program program =
      loadProgram(applicationPath.dirPath() + "TP1/shaders/triangle.vs.glsl",
                  applicationPath.dirPath() + "TP1/shaders/triangle.fs.glsl");
  program.use();

  int number_of_triangles = 42;
  float teta = 2 * glm::pi<float>() / number_of_triangles;
  float radius = 0.5f;

  add_triangles(radius, teta, number_of_triangles);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, CircleBuffer.size() * sizeof(Vertex2DColor),
               CircleBuffer.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),
                        (const void *)(offsetof(Vertex2DColor, m_position)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),
                        (const void *)(offsetof(Vertex2DColor, m_color)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3 * number_of_triangles);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &vao);
  glfwTerminate();
  return 0;
}
