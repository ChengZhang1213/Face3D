package com.example.face3d;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FaceDetectActivity extends AppCompatActivity {


    private Context context;
    private static final int DETECT_FINISH = 1;
    private static final int LOAD_IMAGE_SUCCESS = 2;
    private static final int GENERATE_FINISH = 3;
    private ImageView iv_faceImage;
    private ImageView iv_face_detectImage;

    @SuppressLint("HandlerLeak")
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case DETECT_FINISH:
                    Bitmap faceDetectBitmap = (Bitmap) msg.obj;
                    iv_face_detectImage.setImageBitmap(faceDetectBitmap);
                    break;
                case LOAD_IMAGE_SUCCESS:
                    Bitmap srcBitmap = (Bitmap) msg.obj;
                    iv_faceImage.setImageBitmap(srcBitmap);
                    break;
                case GENERATE_FINISH:
                    Toast.makeText(context, "generate success", Toast.LENGTH_SHORT).show();
                    Intent display3dModelIntent = new Intent(context, Display3dModelActivity.class);
                    startActivity(display3dModelIntent);
                    finish();
                    break;
                default:
                    break;
            }
        }
    };
    Bitmap faceDetectBitmap;
    Bitmap srcBitmap;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_face_detect);
        this.context = FaceDetectActivity.this;


        iv_faceImage = findViewById(R.id.iv_faceImage);
        iv_face_detectImage = findViewById(R.id.iv_face_detectImage);


        Uri data = getIntent().getData();


        File file = new File(
                Constants.GLOBAL_DATA_PATH + "images" + File.separator + "pic.jpg");

        try {
            InputStream fileInputStream;

            if(data!=null){
                fileInputStream = context.getContentResolver().openInputStream(data);
            }else{
                fileInputStream = new FileInputStream(file);

            }
            Bitmap bitmap = BitmapFactory.decodeStream(fileInputStream);

            int width = bitmap.getWidth();
            int height = bitmap.getHeight();
            srcBitmap = Bitmap.createScaledBitmap(bitmap, width / 4, height / 4, false);

//            Message loadImageMessage = Message.obtain();
//            loadImageMessage.what = LOAD_IMAGE_SUCCESS;
//            loadImageMessage.obj = bitmap;
//            handler.sendMessage(loadImageMessage);
            iv_faceImage.setImageBitmap(srcBitmap);
            faceDetectBitmap = Bitmap.createBitmap(srcBitmap.getWidth(), srcBitmap.getHeight(), Bitmap.Config.ARGB_8888);


            new Thread() {
                @Override
                public void run() {
                    super.run();
                    String haarcascadeConfigPath = Constants.GLOBAL_DATA_PATH + "haarcascade" + File.separator +
                            "haarcascade_frontalface_alt.xml";
                    String shapePredictorModelPath = Constants.GLOBAL_DATA_PATH + "model/shape_predictor_68_face_landmarks.dat";
                    Face3D.initConifg(haarcascadeConfigPath, shapePredictorModelPath);
                    Face3D.attachFaceLandmark(srcBitmap, faceDetectBitmap);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            iv_face_detectImage.setImageBitmap(faceDetectBitmap);
                        }
                    });

                    String modelfile = Constants.GLOBAL_DATA_PATH + "model/sfm_shape_3448.bin";
                    String mappingsfile = Constants.GLOBAL_DATA_PATH + "model/ibug_to_sfm.txt";
                    Face3D.init3dModelConfig(modelfile, mappingsfile);

                    String outPutFile = Constants.GLOBAL_DATA_PATH + "out";

                    Bitmap face3dImage = Bitmap.createBitmap(512, 512, Bitmap.Config.ARGB_8888);

                    Face3D.generate3dFaceModel(srcBitmap, outPutFile, face3dImage);

                    File face3dImageFile = new File(outPutFile + ".isomap.png");
                    FileOutputStream fOut = null;
                    try {
                        fOut = new FileOutputStream(face3dImageFile);
                        face3dImage.compress(Bitmap.CompressFormat.PNG, 100, fOut);
                        fOut.flush();
                        fOut.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }


                    Message generateFace3dMessage = Message.obtain();
                    generateFace3dMessage.what = GENERATE_FINISH;
                    handler.sendMessage(generateFace3dMessage);
                }
            }.start();

//
//            Message detectFaceMessage = Message.obtain();
//            detectFaceMessage.what = DETECT_FINISH;
//            detectFaceMessage.obj = faceDetectBitmap;
//            handler.sendMessage(detectFaceMessage);
//
//
//

        } catch (IOException e) {
            e.printStackTrace();
        }


    }


}
