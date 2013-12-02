uniform float timer;

uniform sampler2D heightmapImageLoc;

void main()
{
    
    gl_TexCoord[0] = gl_MultiTexCoord0;

    float x = (gl_TexCoord[0].t + timer)*22.5;
    float y = sin(x) * gl_TexCoord[0].t*22.5 / 100.0;

    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.x, gl_Vertex.y + y, gl_Vertex.z, gl_Vertex.w);
}
