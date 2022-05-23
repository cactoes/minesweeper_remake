uniform sampler2D texture;
uniform vec2 u_resolution;
uniform float u_time;

void main() {
    float aberrationAmount = 0.05;

    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    vec2 distFromCenter = uv - 0.5;

    // stronger aberration near the edges by raising to power 3
    vec2 aberrated = aberrationAmount * pow(distFromCenter, vec2(3.0, 3.0));

    gl_FragColor = vec4
    (
        texture2D(texture, uv - aberrated).r,
        texture2D(texture, uv).g,
        texture2D(texture, uv + aberrated).b,
        1.0
    );
};