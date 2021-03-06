#version 330 core

/*-----  ATTRIBUTES  -----*/
layout (location = 0) in vec3 aPos;         

/*------  UNIFORMS  ------*/
uniform mat4 m_Model;
uniform mat4 m_VP;

void main() 
{
    gl_Position = m_VP * m_Model * vec4(aPos, 1.0);
}
