#version 330 core

/*-----  ATTRIBUTES  -----*/
layout (location = 0) in vec3 aPos;         
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexCoords;   


/*------  UNIFORMS  ------*/
uniform mat4 m_Model;
uniform mat4 m_MVP;
uniform mat4 m_Normal;

/*-------  OUTPUTS  ------*/
out vec2 TexCoords;
out vec3 cameraSpace_pos;
out vec3 cameraSpace_normal;


void main() {
    cameraSpace_pos    = vec3(m_Model * vec4(aPos, 1.0));
    cameraSpace_normal = normalize( vec3(m_Normal * vec4(aNormal, 1.0)) );

    TexCoords   = aTexCoords;
    gl_Position = m_MVP * vec4(aPos, 1.0);
}
