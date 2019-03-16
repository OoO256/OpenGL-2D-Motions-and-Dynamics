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
결정할게 많다. 
```


> Written with [StackEdit](https://stackedit.io/).
<!--stackedit_data:
eyJoaXN0b3J5IjpbNTE4OTYyMDg3LC0yMDU3NTMwODcxLC02Nz
U2OTkxLDI2NzgyODY3XX0=
-->