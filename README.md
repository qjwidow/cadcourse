# CADCourse

本项目基于半边数据结构，实现五个基本欧拉操作（`mvfs`，`mev`，`mef`，`kemr`，`kfmrh`）和扫成操作（`sweep`），并构造出三个例子（`cube`，`sixteen_hole`，`two_rods`）

### 项目结构

```C++
.
├── Draw  // 使用OpenGL绘制实体模型
    ├── Draw.hpp
    ├── Draw.cpp
├── EulerOp  // 实现五个基本欧拉操作
    ├── EulerOp.hpp
    ├── EulerOp.cpp
├── HalfEdgeDS  // 半边数据结构
    ├── HalfEdgeDS.hpp
    ├── HalfEdgeDS.cpp
├── Sweep // 扫成操作
    ├── Sweep.hpp
    ├── Sweep.cpp
├── main.cpp  // 构造三个例子
```

### 编译方法

#### 依赖

项目使用`OpenGL`绘制实体模型，具体依赖于如下库：

* `glfw3`
* `OpenGL`
* `GLEW`
* `GLUT`
* `glm`

#### 编译

项目环境如下：

* `Ubuntu 20.04`
* `g++ 7.5.0`

编译方法：

```shell
mkdir build
cd build
cmake ..
make
```

编译成功后，会在build目录下生成`CADCourseCode_cpp`可执行文件。

#### 运行效果

![one_hole](C:\Users\浩\Desktop\CAD作业\CADCourseCode_cpp\docs\one_hole.png)
