/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

package com.example.face3d.gl;

import android.opengl.GLSurfaceView;
import android.util.Log;

import com.example.face3d.Face3D;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MyGLRenderer implements GLSurfaceView.Renderer {




    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        // called when the surface is created or recreated
        // create (or recreate) native objects that are required for rendering
        Log.d("MyGLRenderer", "onSurfaceCreated");
        Face3D.SurfaceCreatedNative();

    }

    public void onDrawFrame(GL10 unused) {

        // called to draw the current frame
        // call the rendering functions in native
        Face3D.DrawFrameNative();

    }

    public void onSurfaceChanged(GL10 unused, int width, int height) {

        // called after surface is created and GLES surface changes size
        // this can typically happen if device orientation changes
        // we need to set GLES viewport to handle the change in size
        Log.d("MyGLRenderer", "onSurfaceChanged");
        Face3D.SurfaceChangedNative(width, height);

    }

}