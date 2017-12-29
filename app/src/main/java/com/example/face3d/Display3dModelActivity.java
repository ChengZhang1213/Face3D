package com.example.face3d;

import android.app.Activity;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import com.example.face3d.gl.GestureClass;
import com.example.face3d.gl.MyGLSurfaceView;

/**
 * Created by zhangcheng on 2017/12/20.
 */

public class Display3dModelActivity extends Activity{

    private GLSurfaceView mGLView = null;

    GestureClass mGestureObject;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        AssetManager assetManager = getAssets();
        String pathToInternalDir = getFilesDir().getAbsolutePath();
        String outPutFile = Constants.GLOBAL_DATA_PATH + "out";
        // call the native constructors to create an object
        Face3D.CreateObjectNative(assetManager, pathToInternalDir);

        // layout has only two components, a GLSurfaceView and a TextView
        setContentView(R.layout.assimp_layout);
        mGLView = (MyGLSurfaceView) findViewById (R.id.gl_surface_view);

        // mGestureObject will handle touch gestures on the screen
        mGestureObject = new GestureClass(this);
        mGLView.setOnTouchListener(mGestureObject.TwoFingerGestureListener);
    }

    @Override
    protected void onResume() {

        super.onResume();

        // Android suggests that we call onResume on GLSurfaceView
        if (mGLView != null) {
            mGLView.onResume();
        }

    }

    @Override
    protected void onPause() {

        super.onPause();

        // Android suggests that we call onPause on GLSurfaceView
        if(mGLView != null) {
            mGLView.onPause();
        }
    }

    @Override
    protected void onDestroy() {

        super.onDestroy();

        // We are exiting the activity, let's delete the native objects
        Face3D.DeleteObjectNative();

    }

}
