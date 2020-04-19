#version 450 core

/*-----  ATTRIBUTES  -----*/
layout (location = 0) in vec3 aPos;         
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexCoords;   


/*------  UNIFORMS  ------*/
//Geometric
uniform mat4 m_Model;
uniform mat4 m_MVP;
uniform mat4 m_Normal;

/*-------  OUTPUTS  ------*/
out vec2 TexCoords;
out vec3 view_Pos;
out vec3 view_Normal;

void main() {
    view_Pos    = vec3( m_Model * vec4(aPos,1.0) );
    view_Normal = normalize( vec3( m_Normal * vec4(aNormal,1.0) ) );

    TexCoords = aTexCoords;
    gl_Position = m_MVP * vec4(aPos, 1.0);
}
