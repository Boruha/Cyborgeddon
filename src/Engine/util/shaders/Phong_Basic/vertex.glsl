#version 330 core

/*-----  ATTRIBUTES  -----*/
layout (location = 0) in vec3 aPos;         
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexCoords;   


/*------  UNIFORMS  ------*/
uniform mat4 m_Model;
uniform mat4 m_MVP;
uniform mat4 m_Normal;
uniform mat4 m_LightVP;

/*-------  OUTPUTS  ------*/
out vec2 TexCoords;
out vec3 view_pos;
out vec3 view_normal;
out vec4 light_pos;

void main() {
    view_pos    = vec3(m_Model * vec4(aPos, 1.0));
    view_normal = normalize( vec3(m_Normal * vec4(aNormal, 1.0)) );
    light_pos   = m_LightVP * vec4(view_pos, 1.0);

    TexCoords = aTexCoords;
    gl_Position = m_MVP * vec4(aPos, 1.0);
}
