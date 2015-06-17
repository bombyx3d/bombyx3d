// Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).

%fragment

#ifdef GL_ES
precision mediump float;
#endif

%both

varying vec4 v_color;

%vertex

attribute vec2 a_position;
attribute vec4 a_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
    v_color = a_color;
    gl_Position = u_projection * u_modelview * vec4(a_position, 0.0, 1.0);
}

%fragment

uniform vec4 u_color;

void main()
{
    gl_FragColor = u_color * v_color;
}
