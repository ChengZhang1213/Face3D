//
// Created by zhangcheng on 2017/12/15.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "IncompatibleTypes"
#ifndef FACE3D_FACE_H
#define FACE3D_FACE_H

#include <FaceInfo.h>
#include "../dlib/image_processing/shape_predictor.h"
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../dlib/image_processing/frontal_face_detector.h"
#include "../dlib/image_processing.h"
#include "../dlib/opencv/cv_image.h"
#include "../dlib/image_io.h"
#include "ModelInfo.h"

//eos
//#include "eos/core/Landmark.hpp"
//#include "eos/core/LandmarkMapper.hpp"
//#include "eos/fitting/nonlinear_camera_estimation.hpp"
//#include "eos/fitting/linear_shape_fitting.hpp"
//#include "eos/render/utils.hpp"
//#include "eos/render/texture_extraction.hpp"
#include <../eos/include/eos/core/Landmark.hpp>
#include <../eos/include/eos/morphablemodel/MorphableModel.hpp>
#include <../eos/include/eos/core/Image_opencv_interop.hpp>
#include <eos/include/eos/core/LandmarkMapper.hpp>
#include <Eigen/src/Core/Matrix.h>
#include <eos/include/eos/fitting/orthographic_camera_estimation_linear.hpp>
#include <eos/include/eos/fitting/RenderingParameters.hpp>
#include <eos/include/eos/fitting/linear_shape_fitting.hpp>
#include <eos/include/eos/render/texture_extraction.hpp>


using namespace std;
using namespace cv;

using namespace eos;
//namespace po = boost::program_options;
using eos::core::Landmark;
using eos::core::LandmarkCollection;
using cv::Mat;
using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Vector4f;
using std::cout;
using std::endl;
using std::vector;
using std::string;


bool ATTACH_FACE_INFO = true;

ModelInfo *modelInfo = NULL;

class Face {
private:
    dlib::shape_predictor shape_predictor;
    CascadeClassifier face_cascade;

    morphablemodel::MorphableModel morphable_model;
    core::LandmarkMapper landmark_mapper;
    vector<FaceInfo> faceInfos;


public:
    void initShapePredictor(dlib::shape_predictor shape_predictor, CascadeClassifier face_cascade) {
        this->shape_predictor = std::move(shape_predictor);
        this->face_cascade = std::move(face_cascade);
    }


    void faceDetect(Mat &faceImage) {


//        //detect by dlib
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();


        dlib::cv_image<dlib::rgb_pixel> dlib_image(faceImage);
        std::vector<dlib::rectangle> dets = detector(dlib_image, 0);
        cout << "Number of faces detected: " << dets.size() << endl;
        // just some valid rectangle arguments
//        vector<FaceInfo> faceInfos;
        if (!dets.empty()) {
            FaceInfo faceInfo;

            dlib::rectangle &reference = dets[0];
            faceInfo.face_rectangle = dets[0];

            if (ATTACH_FACE_INFO) {
                int x = (int &&) (dets[0].left());
                int y = (int &&) (dets[0].top());
                int width = (int &&) (dets[0].width());
                int height = (int &&) (dets[0].height());
                cv::Rect rect(x, y, width, height);
                cv::rectangle(faceImage, rect, cv::Scalar(0, 255, 0));
            }
            faceInfos.push_back(faceInfo);
        }


        //detect by opencv
//        CascadeClassifier face_cascade;
//        face_cascade.load(resource_path + "haarcascade/haarcascade_frontalface_alt.xml");

//        std::vector<Rect> dets2;
//        face_cascade.detectMultiScale(faceImage, dets2, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
//        vector<FaceInfo> faceInfos;
//        if (!dets.empty()) {
//            for (int i = 0; i < dets.size(); ++i) {
//                Rect_<int> &reference = dets[i];
//                FaceInfo faceInfo;
//                faceInfo.face_rectangle = dlib::rectangle((long) reference.tl().x, (long) reference.tl().y,
//                                                          (long) reference.br().x - 1, (long) reference.br().y - 1);
//
//                faceInfos.push_back(faceInfo);
//                if (ATTACH_FACE_INFO) {
//                    int x = (int &&) (faceInfo.face_rectangle.left());
//                    int y = (int &&) (faceInfo.face_rectangle.top());
//                    int width = static_cast<int>(faceInfo.face_rectangle.width());
//                    int height = static_cast<int>(faceInfo.face_rectangle.height());
//                    cv::Rect rect(x, y, width, height);
//                    cv::rectangle(faceImage, rect, cv::Scalar(0, 255, 0));
//                }
//
//            }
//
//        }
//        return faceInfos;
    }

    void landmarkDetect(Mat faceImage) {
        dlib::cv_image<dlib::rgb_pixel> dlib_image(faceImage);
        for (int i = 0; i < faceInfos.size(); ++i) {
            FaceInfo &faceInfo = faceInfos[i];
            const dlib::full_object_detection shape = shape_predictor(dlib_image,
                                                                      faceInfo.face_rectangle);
            cout << "number of parts: " << shape.num_parts() << endl;
            faceInfo.shape = shape;
            if (ATTACH_FACE_INFO) {
                for (int i = 0; i < faceInfo.shape.num_parts(); ++i) {
                    dlib::point &vector = faceInfo.shape.part(i);
                    //cout << "landmark position " << vector.x() << "," << vector.y() << endl;
                    circle(faceImage,
                           Point(static_cast<int>(vector.x()), static_cast<int>(vector.y())), 1,
                           Scalar(0, 255, 0),
                           1, 8, 0);
                }
            }

        }


    }


    void writeFaceInfoToFile(FaceInfo face_info, const string &filePath) {
        ofstream out(filePath);
        if (out.is_open()) {
            int x = (int &&) (face_info.face_rectangle.left());
            int y = (int &&) (face_info.face_rectangle.top());
            int width = (int &&) (face_info.face_rectangle.width());
            int height = (int &&) (face_info.face_rectangle.height());
            out << "face Rectangle :" << x << " " << y << " " << width << " " << height << "\n";
            for (int i = 0; i < face_info.shape.num_parts(); ++i) {
                out << "feature  " << i << ": " << face_info.shape.part(i).x() << " "
                    << face_info.shape.part(i).y()
                    << endl;
            }
            out.close();
        }
    }


    void initFace3dConfig(const char *modelFile, const char *mappingsFile) {
        morphable_model = morphablemodel::load_model(modelFile);
        landmark_mapper = core::LandmarkMapper(mappingsFile);
    }


    Mat generateFace3dModel(Mat faceImage, String outPut3dModelPath) {

        if (faceInfos.empty()) {
            faceDetect(faceImage);
            landmarkDetect(faceImage);
        }


        LandmarkCollection<Vector2f> landmarks;
        landmarks.reserve(68);
        FaceInfo &faceInfo = faceInfos[0];
        for (int j = 0; j < faceInfo.shape.num_parts(); ++j) {
            Landmark<Vector2f> landmark;
            dlib::point &part = faceInfo.shape.part(j);
            landmark.name = std::to_string(j + 1);
            landmark.coordinates = Vector2f(part.x(), part.y());
            landmarks.emplace_back(landmark);
        }

        std::vector<Vector4f> model_points; // the points in the 3D shape model
        std::vector<int> vertex_indices; // their vertex indices
        std::vector<Vector2f> image_points; // the corresponding 2D landmark points
        // Sub-select all the landmarks which we have a mapping for (i.e. that are defined in the 3DMM):
        for (int i = 0; i < landmarks.size(); ++i) {
            const experimental::optional<string> &converted_name = landmark_mapper.convert(
                    landmarks[i].name);
            if (!converted_name) { // no mapping defined for the current landmark
                continue;
            }
            int vertex_idx = std::stoi(*converted_name);
            Vector3f vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
            model_points.emplace_back(Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.0f));
            vertex_indices.emplace_back(vertex_idx);
            image_points.emplace_back(landmarks[i].coordinates);
        }


        // Estimate the camera (pose) from the 2D - 3D point correspondences
        fitting::ScaledOrthoProjectionParameters pose =
                fitting::estimate_orthographic_projection_linear(image_points, model_points, true,
                                                                 faceImage.rows);
        fitting::RenderingParameters rendering_params(pose, faceImage.cols, faceImage.rows);

        // The 3D head pose can be recovered as follows:
        const float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
        // and similarly for pitch and roll.


        // Estimate the shape coefficients by fitting the shape to the landmarks:
        const Eigen::Matrix<float, 3, 4> &affine_from_ortho = fitting::get_3x4_affine_camera_matrix(
                rendering_params,
                faceImage.cols,
                faceImage.rows);
        const vector<float> fitted_coeffs = fitting::fit_shape_to_landmarks_linear(
                morphable_model.get_shape_model(), affine_from_ortho, image_points, vertex_indices);


        // Obtain the full mesh with the estimated coefficients:
        const core::Mesh mesh = morphable_model.draw_sample(fitted_coeffs, vector<float>());

        // Extract the texture from the image using given mesh and camera parameters:
        const core::Image4u isomap = render::extract_texture(mesh, affine_from_ortho,
                                                             core::from_mat(faceImage));

        // Save the mesh as textured obj:
        core::write_textured_obj(mesh, outPut3dModelPath + ".obj");

        // And save the isomap:

        const Mat &mat = core::to_mat(isomap);
//        cv::imwrite(outPut3dModelPath+".isomap.png", mat);
        modelInfo = new ModelInfo();
        modelInfo->objFilePath = outPut3dModelPath + ".obj";
        modelInfo->mtlFilePath = outPut3dModelPath + ".mtl";
        modelInfo->imageFilePath = outPut3dModelPath + ".isomap.png";
        cout << "Finished fitting and wrote result mesh and isomap to files with basename "
             << outPut3dModelPath << "." << endl;
        return mat;

    }
};

#endif //FACE3D_FACE_H

#pragma clang diagnostic pop