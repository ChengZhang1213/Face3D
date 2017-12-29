# Face3D
此例子是基于opencv+[dlib](https://github.com/davisking/dlib)+[eos](https://github.com/patrikhuber/eos)+[assimp](https://github.com/assimp/assimp)在移动端实现3D Morphable Face Model fitting.
其中dlib用来做FaceDetect & FaceLandmark  attach,eos用来做face reconstruct,assimp 用来做展示生成的obj模型
# NOTICE
使用时需要修改 CMakeLists.txt中OpenCv的路径
效果图
![device-2017-12-29-163133](/sample/device-2017-12-29-163133.png)
# Question
项目存在一个问题，使用dlib做faceDetect非常慢，考虑使用opencv的FaceDetect，但是存在编译问题，opencv使用的是gnustl，但是dlib需要的是c++static
