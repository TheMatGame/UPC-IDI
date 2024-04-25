#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);
uniform vec3 posFoc;
uniform vec3 colFoc;
in vec3 RposFoc;
uniform vec3 RcolFoc;
in vec3 GposFoc;
uniform vec3 GcolFoc;
in vec3 BposFoc;
uniform vec3 BcolFoc;
in vec3 YposFoc;
uniform vec3 YcolFoc;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  vec3 NormSCO = normalize(fnormal);
  vec3 L = normalize(posFoc - fvertex);
  vec3 RL = normalize(RposFoc - fvertex);
  vec3 GL = normalize(GposFoc - fvertex);
  vec3 BL = normalize(BposFoc - fvertex);
  vec3 YL = normalize(YposFoc - fvertex);

  FragColor = vec4(Ambient() + Difus(NormSCO, L, colFoc) + Especular(NormSCO, L, fvertex, colFoc)
                             + Difus(NormSCO, RL, RcolFoc) + Especular(NormSCO, RL, fvertex, RcolFoc)
                             + Difus(NormSCO, GL, GcolFoc) + Especular(NormSCO, GL, fvertex, GcolFoc)
                             + Difus(NormSCO, BL, BcolFoc) + Especular(NormSCO, BL, fvertex, BcolFoc)
                             + Difus(NormSCO, YL, YcolFoc) + Especular(NormSCO, YL, fvertex, YcolFoc), 1);
}
