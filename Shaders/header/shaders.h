#pragma once
#include <string>

const std::string bloom_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;vec3 samplef(vec2 tc);vec3 blur(vec2 tc, float offs);vec3 highlights(vec3 pixel, float thres);void main(){vec2 tc=gl_FragCoord.xy/u_resolution.xy;vec3 color=blur(tc, 2.0);color += blur(tc, 3.0);color += blur(tc, 5.0);color += blur(tc, 7.0);color /= 4.0;color += samplef(tc);gl_FragColor=vec4(mix(samplef(tc), color, 1), 1.0);}vec3 samplef(vec2 tc){return pow(texture2D(texture, tc).xyz, vec3(2.2, 2.2, 2.2));}vec3 hsample(vec2 tc){return highlights(samplef(tc), 0.6);}vec3 blur(vec2 tc, float offs){vec4 xoffs=offs*vec4(-2.0, -1.0, 1.0, 2.0)/u_resolution.x;vec4 yoffs=offs*vec4(-2.0, -1.0, 1.0, 2.0)/u_resolution.y;vec3 color=vec3(0.0, 0.0, 0.0);color += hsample(tc+vec2(xoffs.x, yoffs.x))*0.00366;color += hsample(tc+vec2(xoffs.y, yoffs.x))*0.01465;color += hsample(tc+vec2(0.0, yoffs.x))*0.02564;color += hsample(tc+vec2(xoffs.z, yoffs.x))*0.01465;color += hsample(tc+vec2(xoffs.w, yoffs.x))*0.00366;color += hsample(tc+vec2(xoffs.x, yoffs.y))*0.01465;color += hsample(tc+vec2(xoffs.y, yoffs.y))*0.05861;color += hsample(tc+vec2(0.0, yoffs.y))*0.09524;color += hsample(tc+vec2(xoffs.z, yoffs.y))*0.05861;color += hsample(tc+vec2(xoffs.w, yoffs.y))*0.01465;color += hsample(tc+vec2(xoffs.x, 0.0))*0.02564;color += hsample(tc+vec2(xoffs.y, 0.0))*0.09524;color += hsample(tc+vec2(0.0, 0.0))*0.15018;color += hsample(tc+vec2(xoffs.z, 0.0))*0.09524;color += hsample(tc+vec2(xoffs.w, 0.0))*0.02564;color += hsample(tc+vec2(xoffs.x, yoffs.z))*0.01465;color += hsample(tc+vec2(xoffs.y, yoffs.z))*0.05861;color += hsample(tc+vec2(0.0, yoffs.z))*0.09524;color += hsample(tc+vec2(xoffs.z, yoffs.z))*0.05861;color += hsample(tc+vec2(xoffs.w, yoffs.z))*0.01465;color += hsample(tc+vec2(xoffs.x, yoffs.w))*0.00366;color += hsample(tc+vec2(xoffs.y, yoffs.w))*0.01465;color += hsample(tc+vec2(0.0, yoffs.w))*0.02564;color += hsample(tc+vec2(xoffs.z, yoffs.w))*0.01465;color += hsample(tc+vec2(xoffs.w, yoffs.w))*0.00366;return color;}vec3 highlights(vec3 pixel, float thres){float val=(pixel.x+pixel.y+pixel.z)/3.0;return pixel*smoothstep(thres-0.1, thres+0.1, val);}";
const std::string chromatic_abberation_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;void main(){float aberrationAmount=0.05;vec2 uv=gl_FragCoord.xy/u_resolution.xy;vec2 distFromCenter=uv-0.5;vec2 aberrated=aberrationAmount*pow(distFromCenter, vec2(3.0, 3.0));gl_FragColor=vec4(texture2D(texture, uv-aberrated).r,texture2D(texture, uv).g,texture2D(texture, uv+aberrated).b,1.0);};";
const std::string crt_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;vec2 curve(vec2 uv){uv=(uv-0.5)*2.0;uv*=1.1;uv.x*=1.0+pow((abs(uv.y)/5.0), 2.0);uv.y*=1.0+pow((abs(uv.x)/4.0), 2.0);uv =(uv/2.0)+0.5;uv= uv *0.92+0.04;return uv;}void main(){vec2 q=gl_FragCoord.xy/u_resolution.xy;vec2 uv=q;uv=curve(uv);vec3 col;float x=sin(0.3*u_time+uv.y*21.0)*sin(0.7*u_time+uv.y*29.0)*sin(0.3+0.33*u_time+uv.y*31.0)*0.0017;col.r=texture2D(texture, vec2(x+uv.x+0.001, uv.y+0.001)).x+0.05;col.g=texture2D(texture, vec2(x+uv.x+0.000, uv.y-0.002)).y+0.05;col.b=texture2D(texture, vec2(x+uv.x-0.002, uv.y+0.000)).z+0.05;col.r += 0.08*texture2D(texture, 0.75*vec2(x+0.025, -0.027)+vec2(uv.x+0.001, uv.y+0.001)).x;col.g += 0.05*texture2D(texture, 0.75*vec2(x+-0.022, -0.02)+vec2(uv.x+0.000, uv.y-0.002)).y;col.b += 0.08*texture2D(texture, 0.75*vec2(x+-0.02, -0.018)+vec2(uv.x-0.002, uv.y+0.000)).z;col=clamp(col*0.6+0.4*col*col*1.0, 0.0, 1.0);float vig=(0.0+1.0*8.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y));col*=vec3(pow(vig, 0.3));col*=vec3(0.95, 1.05, 0.95);col*=2.8;float scans=clamp(0.35+0.35*sin(3.5*u_time+uv.y*u_resolution.y*1.5), 0.0, 1.0);float s=pow(scans, 1.7);col=col*vec3(0.4+0.7*s);col*=1.0+0.01*sin(110.0*u_time);if (uv.x < 0.0 || uv.x > 1.0)col*=0.0;if (uv.y < 0.0 || uv.y > 1.0)col*=0.0;col*=1.0-0.65*vec3(clamp((mod(gl_FragCoord.x, 2.0)-1.0)*2.0, 0.0, 1.0));float comp=smoothstep(0.1, 0.9, sin(u_time));gl_FragColor=vec4(col, 1.0);}";
const std::string empty_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;void main(){gl_FragColor=texture2D(texture,gl_TexCoord[0].xy);};";
const std::string film_grain_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;void main(){vec2 uv=gl_FragCoord.xy/u_resolution.xy;vec4 color=texture2D(texture, uv);float strength=8.0;float x=(uv.x+4.0 )*(uv.y+4.0 )*(u_time*10.0);vec4 grain=vec4(mod((mod(x, 13.0)+1.0)*(mod(x, 123.0)+1.0), 0.01)-0.005)*strength;if(uv.x > 0.5){grain=1.0-grain;gl_FragColor=color*grain;}else{gl_FragColor=color+grain;}};";
const std::string vignetting_shader_frag_string="uniform sampler2D texture;uniform vec2 u_resolution;uniform float u_time;void main(){vec2 uv=gl_FragCoord.xy/u_resolution.xy;vec2 center=uv*2.0-1.0;center=center-vec2(0.05, -0.1);vec2 dist=abs(center-(center/vec2(2.0, 2.0)));vec4 vignetting=vec4(dot(dist, dist)*0.5);gl_FragColor=texture2D(texture, uv)-vignetting;};";