#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TexZoomLevel;


uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TexZoomLevel;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TexZoomLevel = a_TexZoomLevel;

	gl_Position = u_ViewProjection * a_Position;
	//gl_Position =  a_Position;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform float u_TexZoomLevel;
uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TexZoomLevel;

void main()
{

	//color = v_Color;
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TexZoomLevel) * v_Color;
}