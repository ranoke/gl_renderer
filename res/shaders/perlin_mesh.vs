#version 130

in vec3 in_Position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec4 ex_Color;
out vec3 ex_Normal;
out vec3 ex_FragPos;

void main(void) {
	//Fragment Position in Model Space
	ex_FragPos = (u_model * vec4(in_Position, 1.0f)).xyz;
	ex_Normal = in_Position;	//Pass Normal

	//Fragment in Screen Space
	gl_Position = u_proj*u_view * vec4(ex_FragPos, 1.0f);

	//Color from Normal Vector
	ex_Color = vec4(normalize(in_Position), 1.0);
}