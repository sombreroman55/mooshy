/* shader.hh
 *
 * Shader class
 *
 */

#ifndef SHADER_HH
#define SHADER_HH

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"

class Shader
{
public:
  unsigned int id;

  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  // Activate the shader
  void use(void);

  // Uniform utitlity functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

private:
  void checkCompileStatus(unsigned int shader, std::string type);
};

#endif /* SHADER_HH */
