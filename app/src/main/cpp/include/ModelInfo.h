//
// Created by zhangcheng on 2017/12/21.
//

#ifndef FACE3D_MODELINFO_H
#define FACE3D_MODELINFO_H

#include <opencv2/core/cvstd.hpp>

struct ModelInfo{
    cv::String objFilePath;
    cv::String mtlFilePath;
    cv::String imageFilePath;
};
extern ModelInfo *modelInfo;
#endif //FACE3D_MODELINFO_H
