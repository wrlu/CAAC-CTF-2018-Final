package com.wrlus.caac.hook;

import cn.edu.cauc.isecca.soperfectapp.MainActivity;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class HookEntry implements IXposedHookLoadPackage {

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {
        if(lpparam.packageName.equals("cn.edu.cauc.isecca.soperfectapp")) {
            XposedHelpers.findAndHookMethod("cn.edu.cauc.isecca.soperfectapp.MainActivity",
                    lpparam.classLoader,
                    "realcheck", String.class, new XC_MethodHook() {
                        @Override
                        protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                            super.beforeHookedMethod(param);
                            String args = (String) param.args[0];
                            param.setResult(MainActivity.check1(args) && MainActivity.check2(args));
                        }

                        @Override
                        protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                            super.afterHookedMethod(param);
                            String args = (String) param.args[0];
                            param.setResult(MainActivity.check1(args) && MainActivity.check2(args));
                        }
                    });
        } else {
            return;
        }
    }
}
