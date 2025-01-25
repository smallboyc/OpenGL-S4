#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

// TRIANGLE

int window_width = 1000;
int window_height = 700;

static void key_callback(GLFWwindow * /*window*/, int /*key*/, int /*scancode*/,
                         int /*action*/, int /*mods*/) {}

static void mouse_button_callback(GLFWwindow * /*window*/, int /*button*/,
                                  int /*action*/, int /*mods*/) {}

static void scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/,
                            double /*yoffset*/) {}

static void cursor_position_callback(GLFWwindow * /*window*/, double /*xpos*/,
                                     double /*ypos*/) {}

static void size_callback(GLFWwindow * /*window*/, int width, int height) {
  window_width = width;
  window_height = height;
}

struct Vertex2DColor {
  Vertex2DColor();
  Vertex2DColor(const glm::vec2 position, const glm::vec3 color)
      : m_position(position), m_color(color) {}
  glm::vec2 m_position;
  glm::vec3 m_color;
};

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

  /* Hook input callbacks */
  glfwSetKeyCallback(window, &key_callback);
  glfwSetMouseButtonCallback(window, &mouse_button_callback);
  glfwSetScrollCallback(window, &scroll_callback);
  glfwSetCursorPosCallback(window, &cursor_position_callback);
  glfwSetWindowSizeCallback(window, &size_callback);

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  Vertex2DColor vertices[] = {
      Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
      Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
      Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0, 0, 1))};
  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DColor), vertices,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //
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

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &vao);
  glfwTerminate();
  return 0;
}