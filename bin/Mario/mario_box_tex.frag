uniform sampler2D textImage;
varying vec2 coords;

void main() { 
	gl_FragColor = texture2D(textImage, coords); 
}
