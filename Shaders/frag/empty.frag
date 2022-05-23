uniform sampler2D texture;
uniform vec2 u_resolution;
uniform float u_time;

void main() {
	//vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	//vec2 coord = gl_FragCoord.xy / u_resolution;
	//vec3 color = pixel.rgb;
	//float size = 10.0f;
	//float alpha = sin(floor(coord.y * size) + u_time * 4);
	//gl_FragColor = vec4(color, alpha);

	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
};