uniform sampler2D mapImage, textImage;
uniform float timer;
varying vec2 coords;

void main() {

 gl_TexCoord[0]=gl_MultiTexCoord0;
 
 vec4 offset = vec4 (0,0,0,0);
 
 coords= gl_TexCoord[0].st+vec2(0,timer);
 
 vec4 hColor= texture2D (mapImage,coords);
 offset.y = hColor.b;
 
 vec4 point = gl_Vertex;
 point.x = point.x*7;
 point.z = point.z*7;
   
 gl_Position=gl_ModelViewProjectionMatrix*(point+offset);
}