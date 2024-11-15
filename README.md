# StretchTool
![image](https://github.com/user-attachments/assets/0eb0bdc1-1738-4374-a9b6-1816f96380ac)

I did this project for the lesson of computer graphics taught by Konstantinos Moustakas.

It had mainly to do with implementing a tool that stretches a model according to some weights given by the user. It was implemented using C++ and OpenGL.
In the beginning I implemented ray cast picking.
After that I used ray cast picking to create a brush drawing tool. The user can set the brushes colour (rgb),radius,hardness and permeability (draw both sides of the model). By clicking and draging the cursor above the models surface the user assigns colours to the models vertices.

![image](https://github.com/user-attachments/assets/ab751352-63e3-4851-9f9b-631fd1258e64)


This brush was used afterwards so that the user can draw some weights ,with values from 0 to 1, on the models surface. The weights were visualized in red,meaning that the closer the color to black the closer to 0 the weight of the vertex is and similarly the more red the closer to 1 the weight is. At the areas where color is abscent the weights are equal to 0.

![image](https://github.com/user-attachments/assets/0c6f64f4-fd13-4b9c-84f2-f11ded032668)

Subsequently the user gets to set a point on the model. This point is rendered as a blue polyhedron. 


![image](https://github.com/user-attachments/assets/7d3ea3d8-b2c0-4958-a7b0-a8a476ae4fd3)


![image](https://github.com/user-attachments/assets/dd2b3b00-80b8-4675-b44c-779e05410834)

![image](https://github.com/user-attachments/assets/b949f5f7-72b4-4044-bc45-77ed21be3aa4)

Finally the by use of the cursor the model is being stretched according to the weights. So the user moves the blue polyhedron and the model stretches accordingly. Note that the blue point is moved on the plane which normal points to the camera.

![image](https://github.com/user-attachments/assets/6edb52c1-0973-4422-b156-7f99073c832e)

![image](https://github.com/user-attachments/assets/655680f0-9d19-4898-94a3-1842686f2a08)

![image](https://github.com/user-attachments/assets/9528929f-b179-4dad-b451-3e422665b546)

The above case , where the user places the blue point inside a red area is the easiest to implement. Things mess up when the user places the point elsewhere, because of the distribution of the weights (above the heighest weight is in the center of the red area which means that if the blue point was placed outside the red area the center of the area would stretch the most). To deal with that problem I dicided to redistribute the weights. The closer a vertex to the blue point the higher its weight. I implemented a breadth first search algorithm to find the distance of the vertices relative to the blue point. Below I breadth first search from the red polyhedron to the green one. Firstly I computed the segment from the red to the green. Then I found the two planes which are perpendicular to this segment and in addition the first plane contains the red point and the second the green point.As one can see the closer a vertex to the green plane is the higher its weight and vise versa for the red one.

![image](https://github.com/user-attachments/assets/95f81bc4-b447-49dc-9a86-f3ffa63f27bf)

Below are some pairs of blue point placement and red areas where I used the algorithm described above to stretch the model

![image](https://github.com/user-attachments/assets/4961ad06-3cb5-4400-a218-469b0afa9f14)

![image](https://github.com/user-attachments/assets/567317e8-2c54-477e-b484-158273aedd7f)

![image](https://github.com/user-attachments/assets/832188ec-3692-43bc-965b-08537566bf3d)

![image](https://github.com/user-attachments/assets/a977c05b-84d8-4fdf-bad2-2f1aed30ec32)
