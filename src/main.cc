#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.hh"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  /*
  static bool wireframe = false;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    wireframe = !wireframe;
    glPolygonMode(GL_FRONT_AND_BACK, (wireframe ? GL_LINE : GL_FILL));
  }
  */
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

float vertices[] =
{
   0.5f, -0.5f, 0.0f, /* 1.0, 0.0, 0.0, */
  -0.5f, -0.5f, 0.0f, /* 0.0, 1.0, 0.0, */
   0.0f,  0.5f, 0.0f  /* 0.0, 0.0, 1.0 */
};

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window;
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mooshy", NULL, NULL);
  if (!window)
  {
    printf("Failed to create OpenGL window!\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD!\n");
    glfwTerminate();
    return -1;
  }

  Shader testShader = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /*
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  */

  // glBindVertexArray(0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(VAO);

  int colors[3] = {0x7F, 0, 0}; // r, g, b
  int section = 0;
  int colorMask = 0x7F;
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    testShader.use();
    int rainbowColorLocation = glGetUniformLocation(testShader.id, "rainbowColor");
    float r = (float)colors[2] / (float)colorMask;
    float g = (float)colors[1] / (float)colorMask;
    float b = (float)colors[0] / (float)colorMask;

    printf("%f %f %f\n", r, g, b);
    glUniform4f(rainbowColorLocation, r, g, b, 1.0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
    int nextSection = (section + 1) % 3;
    colors[section]--; colors[nextSection]++;
    if (colors[section] == 0) { section++; section %= 3; }
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
