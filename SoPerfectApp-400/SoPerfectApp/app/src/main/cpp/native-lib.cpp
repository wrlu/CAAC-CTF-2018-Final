#include <jni.h>
#include <string>


extern "C"
JNIEXPORT jstring JNICALL
Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//flag{Xposed_can_Hook_any?????}
extern "C"
JNIEXPORT jboolean JNICALL
Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check1(JNIEnv *env, jclass type, jstring flag_) {
    const char *flag = env->GetStringUTFChars(flag_, 0);
    jboolean ret;
    for (int i = 0; i < 30; ++i) {
        if (flag[i] == '\0') {
            ret = false;
            env->ReleaseStringUTFChars(flag_, flag);
            return ret;
        }
    }
    if (flag[30] != '\0') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    if (flag[0] != 'f' || flag[1] != 'l' || flag[2] != 'a' || flag[3] != 'g' || flag[4] != '{' || flag[29] != '}') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    if (flag[21] != 'a' || flag[22] != 'n' || flag[23] != 'y') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    jclass clazz = env->FindClass("cn/edu/cauc/isecca/soperfectapp/Cpp2JavaBridge");
    jmethodID methodId = env->GetStaticMethodID(clazz, "chk", "(Ljava/lang/String;)Z");
    ret = env->CallStaticBooleanMethod(clazz, methodId, flag_);
    env->ReleaseStringUTFChars(flag_, flag);
    return ret;
}

//flag{Xposed_can_Hook_???thing}
extern "C"
JNIEXPORT jboolean JNICALL
Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check2(JNIEnv *env, jclass type, jstring flag_) {
    const char *flag = env->GetStringUTFChars(flag_, 0);
    jboolean ret;
    for (int i = 0; i < 30; ++i) {
        if (flag[i] == '\0') {
            ret = false;
            env->ReleaseStringUTFChars(flag_, flag);
            return ret;
        }
    }
    if (flag[30] != '\0') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    if (flag[0] != 'f' || flag[1] != 'l' || flag[2] != 'a' || flag[3] != 'g' || flag[4] != '{' || flag[29] != '}') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    if (flag[24] != 't' || flag[25] != 'h' || flag[26] != 'i' || flag[27] != 'n' || flag[28] != 'g') {
        ret = false;
        env->ReleaseStringUTFChars(flag_, flag);
        return ret;
    }
    jclass clazz = env->FindClass("cn/edu/cauc/isecca/soperfectapp/Cpp2JavaBridge");
    jmethodID methodId = env->GetStaticMethodID(clazz, "chk", "(Ljava/lang/String;)Z");
    ret = env->CallStaticBooleanMethod(clazz, methodId, flag_);
    env->ReleaseStringUTFChars(flag_, flag);
    return ret;
}