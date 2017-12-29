package com.example.face3d;

import android.content.res.AssetManager;
import android.graphics.Bitmap;

/**
 * Created by zhangcheng on 2017/12/14.
 */

public class Face3D {
    static {
        System.loadLibrary("face3d");
    }


    public static native void initConifg(String haarcascadeFilePath, String shapePredictModel);

    public static native void getFaceRect(Bitmap faceBitmap, Bitmap outBitmap);

    public static native void attachFaceLandmark(Bitmap bitmap, Bitmap faceDetectBitmap);

    public static native void  init3dModelConfig(String modelfile, String mappingsfile) ;

    public static native void generate3dFaceModel(Bitmap bitmap, String outPutFile, Bitmap face3dImage) ;

    public static native void CreateObjectNative(AssetManager assetManager,  String pathToInternalDir);
    public static native void DeleteObjectNative();

    public static native void DrawFrameNative();
    public static native void SurfaceCreatedNative();
    public static native void SurfaceChangedNative(int width, int height);



    public static native void DoubleTapNative();
    public static native void ScrollNative(float distanceX, float distanceY, float positionX, float positionY);
    public static native void ScaleNative(float scaleFactor);
    public static native void MoveNative(float distanceX, float distanceY);


}