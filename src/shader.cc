/* shader.cc
 *
 * Shader class
 *
 */

#include "../include/shader.hh"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  std::string vertexSource;
  std::string fragmentSource;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexSource   = vShaderStream.str();
    fragmentSource = fShaderStream.str();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  const char* vShaderSource = vertexSource.c_str();
  const char* fShaderSource = fragmentSource.c_str();

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderSource, NULL);
  glCompileShader(vertex);
  checkCompileStatus(vertex, "VERTEX");

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderSource, NULL);
  glCompileShader(fragment);
  checkCompileStatus(fragment, "FRAGMENT");

  id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  checkCompileStatus(id, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader()
{
  glDeleteProgram(id);
}

void Shader::use(void)
{
  glUseProgram(id);
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
}

void Shader::checkCompileStatus(unsigned int shader, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl;
      std::cout << infoLog << std::endl; 
      std::cout << "-- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl;
      std::cout << infoLog << std::endl; 
      std::cout << "-- --------------------------------------------------- -- " << std::endl;
    }
  }
}
