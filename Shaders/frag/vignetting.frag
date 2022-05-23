uniform sampler2D texture;
uniform vec2 u_resolution;
uniform float u_time;

#define INTENSITY 0.5

void main() {
	// Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    
    // Vignetting algorithm
    vec2 center = uv * 2.0 - 1.0;
    center = center - vec2(0.05, -0.1);
    
    vec2 dist = abs(center - (center/vec2(2.0, 2.0)));
    vec4 vignetting = vec4(dot(dist, dist)*INTENSITY);
    
    // Output to screen
	gl_FragColor = texture2D(texture, uv) - vignetting;

};