# 3d-obj-loader
A simple 3d file loader implemented in c with its on triangle and line restarizers.


There are two main.h files here . One has all the functions we wrote in the course one one has none. This means one is for your reference and other is for you to 
write from scratch. The file includes the properly scaled deer.obj and cube.obj without normals. If you want the normals you have to import them in blender and export with normals checked. 

the early phase uses simple cube coordinates to print in screen here is its structure. 


draw_triangle(68.642784f, 180.526321f, 115.536751f, 111.114182f, 82.839745f, 40.027714f, color_red);
draw_triangle(115.536751f, 111.114182f, 68.642784f, 180.526321f, 150.690704f, 191.869644f, color_green);
draw_triangle(115.536751f, 111.114182f, 150.690704f, 191.869644f, 197.584671f, 122.457504f, color_red);
draw_triangle(115.536751f, 111.114182f, 197.584671f, 122.457504f, 164.887665f, 51.371037f, color_black);
draw_triangle(115.536751f, 111.114182f, 164.887665f, 51.371037f, 82.839745f, 40.027714f, color_blue);
draw_triangle(68.642784f, 180.526321f, 82.839745f, 40.027714f, 35.945774f, 109.439850f, color_green);
draw_triangle(150.690704f, 191.869644f, 68.642784f, 180.526321f, 35.945774f, 109.439850f, color_black);
draw_triangle(164.887665f, 51.371037f, 35.945774f, 109.439850f, 82.839745f, 40.027714f, color_black);
draw_triangle(197.584671f, 122.457504f, 150.690704f, 191.869644f, 117.993698f, 120.783173f, color_blue);
draw_triangle(197.584671f, 122.457504f, 117.993698f, 120.783173f, 164.887665f, 51.371037f, color_red);
draw_triangle(150.690704f, 191.869644f, 35.945774f, 109.439850f, 117.993698f, 120.783173f, color_blue);
draw_triangle(164.887665f, 51.371037f, 117.993698f, 120.783173f, 35.945774f, 109.439850f, color_green);

