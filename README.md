# Computer Graphics
Notes, assignments and projects of the Computer Graphics learning unit

Teacher: M. en C. Rafael Norman Saucedo Delgado

Student: Bravo López Luis Ángel

## Assignments
1. [Implementation of the line drawing algorithms such as Naive, DDA and Bresenham. Make Bresenham algorithm version using threads.](https://github.com/pekochu/computer-graphics-isc/tree/main/practices/Drawing%20Line)
2. [Read an .obj file from Blender and drawing the model with the Line Drawing Algorithm.](https://github.com/pekochu/computer-graphics-isc/tree/main/practices/Reading%20Blender%20Objects)
3. [Animate the Blender model with rotation and make a video with it.](https://github.com/pekochu/computer-graphics-isc/tree/main/practices/Orthogonal%20Projection)
4. [Read an .obj file from Blender and parse it to a .vlf file for further assignments.](https://github.com/pekochu/computer-graphics-isc/tree/main/practices/VLF%20Parser)
5. [Z-Buffer and Scanline](https://github.com/pekochu/computer-graphics-isc/tree/main/practices/ZBuffer)

# Summary
If you dont want to visit each practice, here are a TL;DR.
## Drawing Line
This is a image rendered by the program generating 500 random lines with a random color, drawing it with Naive Algorithm.

![Sample image](./practices/Drawing%20Line/sample.jpg)

## Read from Blender
Image rendered with the Bresenham algorithm. I've exported an .obj file from Blender and read it with this program.

![Suzanne Model](./practices/Reading%20Blender%20Objects/Monkey.png)

## Perspective projection
Image rendered with the Bresenham algorithm. I've take advantage of my CPU 3 cores to render all the XY, XZ, YZ views.

### XY View
![XY View](./practices/Orthogonal%20Projection/XYView.ppm.png)

### XZ View
![XY View](./practices/Orthogonal%20Projection/XZView.ppm.png)

### ZY View
![XY View](./practices/Orthogonal%20Projection/ZYView.ppm.png)

## Animation!
One of the more frustating practices I ever had, just because I was strugling with centering the model, but, after MONTHS I finally figured out. Just check it out.

### Video from YouTube
[![YouTube](https://img.youtube.com/vi/c3h0p4jMf9U/0.jpg)](https://www.youtube.com/watch?v=c3h0p4jMf9U)

## Triangulation and VLF exporter

I needed to implement an algorithm to convert polygons with n sides into triangles. This was easy with squares, but with n-sided polygons it was a bit difficult. In the end, I was able to do it.

### Square without triangles
![Square without triangles](./practices/VLF%20Parser/CubeWithoutTriangles.png)

### Square with triangles
![Square with triangles](./practices/VLF%20Parser/CubeWithTriangles.png)

Also, I created new structures called Face and Edge, to better manage the VLF. I also added new attributes, called index, to the structures, to be able to write the VLF file fulfilling the file requirements.

### Preview: header of VLF

```
00000000  80 b5 00 00 80 b5 00 00  80 3c 00 00 bd fc d6 3e  |.........<.....>|
```
The first 8 bytes are to represent the size of the model vertices. `` 80 b5 00 00 `` = 46464 vertices. 

The next 8 bytes are to represent the size of the model edges. `` 80 b5 00 00 `` = 46464 edges. 

The next 8 bytes are to represent the size of the model faces. `` 80 3c 00 00 `` = 15488 faces. 

**These values are from the Suzanne model. For more information, please check the folder practice.**

## Z-Buffer and Scanline

``In progress...``
