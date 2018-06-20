attribute vec3 vpos;


void main() {
	gl_Position = mvpMatrix * vec4(vpos, 1);
}