#version 450 core

/*-----  ATTRIBUTES  -----*/
layout (location = 0) in vec3 aPos;         

/*------  UNIFORMS  ------*/
uniform mat4 m_Model;
uniform mat4 m_MVP;

/*-------  OUTPUTS  ------*/
out vec3 view_Pos;

void main() {
    view_Pos    = vec3( m_Model * vec4(aPos,1.0) );
    gl_Position = m_MVP * vec4(aPos, 1.0);
}
