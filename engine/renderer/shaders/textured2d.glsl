// Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).

#ifdef GL_ES
precision mediump float;
#endif

%both

varying vec2 v_texcoord0;

%vertex

attribute vec2 a_position;
attribute vec2 a_texcoord0;

void main()
{
    v_texcoord0 = a_texcoord0;
    gl_Position = vec4(a_position, 0.0, 1.0);
}

%fragment

uniform sampler2D u_texture0;

void main()
{
    gl_FragColor = texture2D(u_texture0, v_texcoord0);
}
