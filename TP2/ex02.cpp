#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

// QUAD & transformations

int   window_width  = 1000;
int   window_height = 700;
float elapsedTime   = 0.0f;

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/) {}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/) {}

static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/) {}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

struct Vertex2DUV {
    Vertex2DUV();
    Vertex2DUV(const glm::vec2 position, const glm::vec2 texture)
        : m_position(position), m_texture(texture) {}
    glm::vec2 m_position;
    glm::vec2 m_texture;
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
}

glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}

glm::mat3 rotate(float angle_deg)
{
    float angle_rad = glm::radians(angle_deg);
    return glm::mat3(glm::vec3(cos(angle_rad), sin(angle_rad), 0), glm::vec3(-sin(angle_rad), cos(angle_rad), 0), glm::vec3(0, 0, 1));
}

//-----------------------------//

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
    //  easier to stay at root folder and run following commands
    //  - compile with make -C build
    //  - execute with bin/TP2_ex01 color2D.vs.glsl color2D.fs.glsl
    std::cout << "Size of arguments : " << argc << "\n";
    std::cout << "Exe : " << argv[0] << "\n";
    std::cout << "Vertex shader : " << argv[1] << "\n";
    std::cout << "Fragment shader : " << argv[2] << "\n";
    glimac::Program program =
        loadProgram(applicationPath.dirPath() + "TP2/shaders/" + argv[1], applicationPath.dirPath() + "TP2/shaders/" + argv[2]);
    program.use();

    // uniforms
    GLint  location_uTime        = glGetUniformLocation(program.getGLId(), "uTime");
    GLuint location_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");

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
    Vertex2DUV vertices[] = {
        Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0, 0)),
        Vertex2DUV(glm::vec2(1, -1), glm::vec2(0, 0)),
        Vertex2DUV(glm::vec2(0, 1), glm::vec2(0, 0)),
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glm::uint32_t index[] = {
        0,
        1,
        2,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const void*)(offsetof(Vertex2DUV, m_position)));
    const GLuint VERTEX_ATTR_TEXTURE = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const void*)(offsetof(Vertex2DUV, m_texture)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        elapsedTime += 0.1f;
        glUniform1f(location_uTime, elapsedTime);
        glUniformMatrix3fv(location_uModelMatrix, 1, GL_FALSE, glm::value_ptr(rotate(elapsedTime)));
        //
        glClear(GL_COLOR_BUFFER_BIT);
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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