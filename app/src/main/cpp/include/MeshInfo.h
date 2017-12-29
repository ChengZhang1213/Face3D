//
// Created by zhangcheng on 2017/12/20.
//

#ifndef FACE3D_MESHINFO_H
#define FACE3D_MESHINFO_H
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
struct MeshInfo {
    GLuint  textureIndex;
    int     numberOfFaces;
    GLuint  faceBuffer;
    GLuint  vertexBuffer;
    GLuint  textureCoordBuffer;
};

#endif //FACE3D_MESHINFO_H
