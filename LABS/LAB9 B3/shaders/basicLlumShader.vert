#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform mat3 NormMat;



//out vec3 fcolor;
out vec3 Normal;
out vec3 vertexF;

out vec3 matambF;
out vec3 matdiffF;
out vec3 matspecF;
out float matshinF;


void main()
{	  
  vertexF = (view * TG * vec4 (vertex, 1.0)).xyz;
  
  Normal = NormMat * normal;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);

    matambF = matamb;
    matdiffF = matdiff;
    matspecF = matspec;
    matshinF = matshin;

}
