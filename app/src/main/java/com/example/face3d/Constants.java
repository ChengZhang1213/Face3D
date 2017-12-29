package com.example.face3d;

import android.os.Environment;

import java.io.File;

/**
 * Created by zhangcheng on 2017/12/18.
 */

public class Constants {

    public static String GLOBAL_DATA_PATH = Environment.getExternalStorageDirectory().getAbsolutePath()+ File.separator+"Face3d"+File.separator;
}
