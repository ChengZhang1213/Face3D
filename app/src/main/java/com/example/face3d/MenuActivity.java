package com.example.face3d;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.example.face3d.camera.CameraActivity;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhangcheng on 2017/12/22.
 * <p>
 * list the function
 * 1) deal the picture which comes from assets
 * 2) deal the picture which comes from camera
 */

public class MenuActivity extends Activity {
    private static final int PICK_IMAGE = 0x0001;
    private List<String> functionList = new ArrayList<>();
    private ListView lv_function;
    private Context context;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.context = MenuActivity.this;
        initView();
        initFunctionList();

        copyDataFromAssetToSDCard();

    }

    private void copyDataFromAssetToSDCard() {
        File file = new File(Constants.GLOBAL_DATA_PATH + "model/shape_predictor_68_face_landmarks.dat");
        if(!file.exists())
        {
            FileUtils.copyFilesFassets(context,"model",Constants.GLOBAL_DATA_PATH+"model/");
            FileUtils.copyFilesFassets(context,"image",Constants.GLOBAL_DATA_PATH+"images/");
        }

    }

    private void initView() {
        setContentView(R.layout.activity_menu);
        lv_function = findViewById(R.id.lv_function);
    }

    private void initFunctionList() {
        functionList.add("Picture");
        functionList.add("Camera");

        FunctionAdapter functionAdapter = new FunctionAdapter(context, functionList);
        lv_function.setAdapter(functionAdapter);
    }

    public class FunctionAdapter extends BaseAdapter {

        private List<String> functionList;
        private Context context;

        public FunctionAdapter(Context context, List<String> functionList) {
            this.functionList = functionList;
            this.context = context;
        }

        @Override
        public int getCount() {
            return functionList.isEmpty() ? 0 : functionList.size();
        }

        @Override
        public Object getItem(int position) {
            return functionList.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(final int position, final View convertView, ViewGroup parent) {
            TextView textView = new TextView(context);
            textView.setPadding(10, 10, 10, 10);
            textView.setTextSize(45);
            textView.setText(functionList.get(position));

            textView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    switch (position) {
                        case 0:
                            //choose picture from gallery
                            choosePictureFromGallery();
                            break;
                        case 1:
                            startActivity(new Intent(context, CameraActivity.class));
                            break;
                        default:
                            break;
                    }
                }
            });

            return textView;
        }
    }

    private void choosePictureFromGallery() {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_IMAGE) {
            if(data!=null){
                Uri data1 = data.getData();
                if (data1 != null) {
                    Intent intent = new Intent(context, FaceDetectActivity.class);
                    intent.setData(data1);
                    startActivity(intent);
                }
            }

        }
    }
}
