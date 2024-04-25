#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 TGBola;
uniform mat4 Proj;
uniform mat4 View;
uniform mat3 NormMat;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;

out vec3 RposFoc;
out vec3 GposFoc;
out vec3 BposFoc;
out vec3 YposFoc;


void main()
{	
    RposFoc = (View * TGBola * vec4 (1.2, 0, 0, 1.0)).xyz;
    GposFoc = (View * TGBola * vec4 (-1.2,0,0, 1.0)).xyz;
    BposFoc = (View * TGBola * vec4 (0,0,1.2, 1.0)).xyz;
    YposFoc = (View * TGBola * vec4 (0,0,-1.2, 1.0)).xyz;

    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
    fvertex = (View * TG * vec4 (vertex, 1.0)).xyz;
    fnormal = NormMat * normal;

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
