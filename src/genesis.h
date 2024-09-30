#ifndef GENESIS_H
# define GENESIS_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
# define WINDOW_TITLE "Genesis"

typedef struct	s_dot
{
 float	x;
 float	y;
 float	z;
}				t_dot;

typedef struct	s_camera
{
 vec3	pos;
 vec3	front;
 vec3	up;
}				t_camera;

void	framebuffer_size_callback(GLFWwindow *window, int width, int height);
void	process_input(GLFWwindow *window);
void	generate_grid(float size, int divisions, GLfloat **vertices, int *vertex_count);

#endif
