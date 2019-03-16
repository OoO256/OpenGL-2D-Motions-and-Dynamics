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
back buffer에 화면을 그리고 그림이 우front buffer에서 화면으로 추
그러려면 screen buffer외에 buffer를 하나 더 필요함

프레임 전환 시, 
```


> Written with [StackEdit](https://stackedit.io/).
<!--stackedit_data:
eyJoaXN0b3J5IjpbOTU3MzQyNjk4LDI2NzgyODY3XX0=
-->