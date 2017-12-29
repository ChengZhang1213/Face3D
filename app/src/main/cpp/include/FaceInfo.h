//
// Created by zhangcheng on 2017/12/14.
//

#ifndef FACE3D_FACEINFO_H
#define FACE3D_FACEINFO_H

#include "dlib/geometry/rectangle.h"
#include "dlib/image_processing/full_object_detection.h"

struct FaceInfo {
    dlib::rectangle face_rectangle;
    dlib::full_object_detection shape;
};

#endif //FACE3D_FACEINFO_H
