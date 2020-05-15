package cn.edu.cauc.isecca.soperfectapp;

import android.util.Base64;
import android.util.Log;

public class Cpp2JavaBridge {

//  flag{Xposed, can, Hook
    private static String[] keywords = {"ZmxhZ3tYcG9zZWQ=", "Y2Fu", "SG9vaw=="};

    public static boolean chk(String flag) {
        try{
            String[] spstrs = flag.split("_");
            for (int i = 0; i < 3; ++i) {
                if (!spstrs[i].equals(new String(Base64.decode(keywords[i], Base64.DEFAULT)))) {
                    return false;
                }
            }
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

}
