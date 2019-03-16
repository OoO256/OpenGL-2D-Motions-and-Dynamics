# OpenGL Tutorials

## from  [http://ogldev.atspace.co.uk](http://ogldev.atspace.co.uk/)

## Tutorial 01

### 윈도우 열기

#### 배경
```
Windowing systems 자체에서 OpenGL을 위한 바인딩을 제공한다. OpenGL에서 API를 정하는것이 아님.
유닉스(X window system)에서는 GLX, 윈도에서는 WGL, 멕에서는 CGL 이라고 한다.
그래서 GLUT : OpenGL utility library 가 high level library 
```

#### GLUT_DOUBLE : DOUBLE BUFFER

```
화면이 갑자기 변하는 것을 막기 위해
이전 버퍼를 미리 화면에 뿌려놓고 write함
back buffer에 화면을 그리고, front buffer에서 화면으로 출력한다.
back buffer에 화면이 다 그려지면, 두 버퍼를 swap한다.


trearing을 방지하기 위해 Page flipping을 사용하기도 한다.
https://blog.seulgi.kim/2014/05/flip-copy-double-buffering-2.html
```

#### glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

```
concept of state in OpenGL.
렌더링은 적은 파라미터를 가진 함수만으로 할 수 있는게 아니다.
세이더, 버퍼, 플레그 등 결정할게 많다. 
이런 결정을 매 함수 호출시마다 하지 않고 state로 지정하면 함수의 파라미터를 줄일 수 있다.
```
## Tutorial 02
### # Hello dot!

#### GLEW
```
extension management
```

#### GLuint VBO;
```
store the handle of the vertex buffer object

GLuint act like pointer

Store data in video memory and provide the shortest access time to the GPU rather than call every single vertex.
```
 #### mapping
 ```
converting in rasterizer
X, Y, Z [-1.0, 1.0] -> coordinates in screen

(0, 0, 0) means center of the screen
```

#### glGenBuffers(num_object, address)
```
generating objects of various types
```
#### binding
```
OpenGL bind the handle to a target name and then execute commands on that target.
```

#### handle & target
```
IDK actually
I assume that handle means GLuint address
and target means what that is
```

#### GL_ARRAY_BUFFER
```
the buffer will contain an array of vertices
```

#### GL_ELEMENT_ARRAY_BUFFER 
```
means that the buffer contains the indices of the vertices in another buffer.
```




> Written with [StackEdit](https://stackedit.io/).
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE0NTI5NDE0NjUsODIxMTkxNzU3LDYxND
IzODQyLDE1MTg0NTEzMDgsMTkxMDQyNjc1MiwtNzM2MDgzNDQ4
LC0xMjU5MTkyMjk0LDg1NDIwNjMxMiwxNTc5MzUwMTM4LDk3OD
g4MjY4OCwtMTczMDM3NDY2NCwtODU4OTE0NiwyMTE3OTgxMjUx
LC0yMDU3NTMwODcxLC02NzU2OTkxLDI2NzgyODY3XX0=
-->