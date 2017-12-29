#include <jni.h>
#include <string>
#include <modelAssimp.h>
#include <myJNIHelper.h>
#include "android_utils.h"
#include "Face.h"

Face face;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_getFaceRect(JNIEnv *env, jclass type, jobject faceBitmap,
                                           jobject outBitmap) {

    Mat srcMat;
    bitmap_to_mat(env, faceBitmap, srcMat);

    Mat faceMat;
    cvtColor(srcMat, faceMat, COLOR_RGBA2RGB);

//    Mat faceMat;
//    resize(srcConvert, faceMat, Size(srcConvert.size().width / 4, srcConvert.size().height / 4));

    face.faceDetect(faceMat);

    mat_to_bitmap(env, faceMat, outBitmap);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_initConifg(JNIEnv *env, jclass type, jstring haarcascadeFilePath_,
                                          jstring shapePredictModel_) {
    const char *haarcascadeFilePath = env->GetStringUTFChars(haarcascadeFilePath_, 0);
    const char *shapePredictModel = env->GetStringUTFChars(shapePredictModel_, 0);

    dlib::shape_predictor shape_predictor;
    dlib::deserialize(shapePredictModel) >> shape_predictor;

    CascadeClassifier face_cascade;
    face_cascade.load(haarcascadeFilePath);


    face.initShapePredictor(shape_predictor, face_cascade);


    env->ReleaseStringUTFChars(haarcascadeFilePath_, haarcascadeFilePath);
    env->ReleaseStringUTFChars(shapePredictModel_, shapePredictModel);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_attachFaceLandmark(JNIEnv *env, jclass type, jobject faceBitmap,
                                                  jobject faceDetectBitmap) {

    Mat srcMat;
    bitmap_to_mat(env, faceBitmap, srcMat);

    Mat faceMat;
    cvtColor(srcMat, faceMat, COLOR_RGBA2RGB);

    face.faceDetect(faceMat);

    face.landmarkDetect(faceMat);

    mat_to_bitmap(env, faceMat, faceDetectBitmap);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_init3dModelConfig(JNIEnv *env, jclass type, jstring modelfile_,
                                                 jstring mappingsfile_) {
    const char *modelfile = env->GetStringUTFChars(modelfile_, 0);
    const char *mappingsfile = env->GetStringUTFChars(mappingsfile_, 0);


    face.initFace3dConfig(modelfile, mappingsfile);


    env->ReleaseStringUTFChars(modelfile_, modelfile);
    env->ReleaseStringUTFChars(mappingsfile_, mappingsfile);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_generate3dFaceModel(JNIEnv *env, jclass type, jobject faceBitmap,
                                                   jstring outPutFile_, jobject face3dBitmap) {
    const char *outPutFile = env->GetStringUTFChars(outPutFile_, 0);

    Mat srcMat;
    bitmap_to_mat(env, faceBitmap, srcMat);


    Mat faceMat;
    cvtColor(srcMat, faceMat, COLOR_RGBA2RGB);

    Mat mat = face.generateFace3dModel(faceMat, outPutFile);

    mat_to_bitmap(env, mat, face3dBitmap);

    env->ReleaseStringUTFChars(outPutFile_, outPutFile);
}


// global pointer is used in JNI calls to reference to same object of type Cube
ModelAssimp *gAssimpObject = NULL;

// global pointer to instance of MyJNIHelper that is used to read from assets
MyJNIHelper *gHelperObject = NULL;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_CreateObjectNative(JNIEnv *env, jclass type, jobject assetManager,
                                                  jstring pathToInternalDir_) {
//    const char *pathToInternalDir = env->GetStringUTFChars(pathToInternalDir_, 0);

    gHelperObject = new MyJNIHelper(env, assetManager, pathToInternalDir_);
    gAssimpObject = new ModelAssimp();

//    env->ReleaseStringUTFChars(pathToInternalDir_, pathToInternalDir);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_DeleteObjectNative(JNIEnv *env, jclass type) {

    if (gAssimpObject != NULL) {
        delete gAssimpObject;
    }
    gAssimpObject = NULL;

    if (gHelperObject != NULL) {
        delete gHelperObject;
    }
    gHelperObject = NULL;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_DrawFrameNative(JNIEnv *env, jclass type) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->Render();

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_SurfaceCreatedNative(JNIEnv *env, jclass type) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->PerformGLInits();

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_SurfaceChangedNative(JNIEnv *env, jclass type, jint width,
                                                    jint height) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->SetViewport(width, height);

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_DoubleTapNative(JNIEnv *env, jclass type) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->DoubleTapAction();

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_ScrollNative(JNIEnv *env, jclass type, jfloat distanceX,
                                            jfloat distanceY, jfloat positionX, jfloat positionY) {


    if (gAssimpObject == NULL) {
        return;
    }
    // normalize movements on the screen wrt GL surface dimensions
    // invert dY to be consistent with GLES conventions
    float dX = (float) distanceX / gAssimpObject->GetScreenWidth();
    float dY = -(float) distanceY / gAssimpObject->GetScreenHeight();
    float posX = 2 * positionX / gAssimpObject->GetScreenWidth() - 1.;
    float posY = -2 * positionY / gAssimpObject->GetScreenHeight() + 1.;
    posX = fmax(-1., fmin(1., posX));
    posY = fmax(-1., fmin(1., posY));
    gAssimpObject->ScrollAction(dX, dY, posX, posY);

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_ScaleNative(JNIEnv *env, jclass type, jfloat scaleFactor) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->ScaleAction((float) scaleFactor);

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_face3d_Face3D_MoveNative(JNIEnv *env, jclass type, jfloat distanceX,
                                          jfloat distanceY) {

    if (gAssimpObject == NULL) {
        return;
    }

    // normalize movements on the screen wrt GL surface dimensions
    // invert dY to be consistent with GLES conventions
    float dX = distanceX / gAssimpObject->GetScreenWidth();
    float dY = -distanceY / gAssimpObject->GetScreenHeight();
    gAssimpObject->MoveAction(dX, dY);

}