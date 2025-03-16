shader_type canvas_item;

uniform vec3 color: source_color;
uniform float lineWidth : hint_range(0.0, 100.0, 0.1);
uniform vec2 size;
uniform sampler2D noise: filter_nearest;
uniform sampler2D noise2: filter_nearest;
uniform float edge_fade : hint_range(0.0, 1.0, 0.1);
uniform float wave_speed: hint_range(0.0, 10.0, 0.1);

void fragment() {
	
	float n = texture(noise,mod(UV+-TIME*wave_speed/21.2,1.0)).r;
	float n2 = texture(noise2,mod(UV+14.7+TIME*wave_speed/40.3,1.0)).r;;
	float n3 = clamp(0.0,0.3+pow(n+(n2*0.4),3.0)*1.5,1.0);
	COLOR.rgb = color + vec3(n3);
	
	vec2 m = 1.0-(edge_fade*abs(UV-0.5)*2.0*size-size+1.0+lineWidth/50.0);
	float ma = min(m.x,m.y);
	
	float uvx = mod(UV.x-((n3-0.5)/100.0)-(mod(size.x,2)/2.0+0.5),1.0/size.x)*size.x;
	float uvy = mod(UV.y-((n3-0.5)/100.0)-(mod(size.y,2)/2.0+0.5),1.0/size.y)*size.y;
	vec2 uv = vec2(uvx,uvy);
	vec2 w = size*max(n3,0.5)*lineWidth/1000.0;
	if(((uv.x>=w.x)&&(uv.x<=1.0-w.x))&&((uv.y>=w.y)&&(uv.y<=1.0-w.y))){
		discard;
	}
	COLOR.a = ma*n3;
}