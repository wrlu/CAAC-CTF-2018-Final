# 民航CTF决赛Reverse部分WP

## 1. FlagCounter（200pt）
- 主要考点：JAR逆向
- 工具：Java Decompiler / Luyten

- 题目给出JAR文件，直接使用JD或Luyten工具进行反编译
- 找到`cn.edu.cauc.isecca.fc.FlagCounter`类，定位到按钮点击事件方法：

```java
if (e.getSource() == this.btnSubmit) {
            final FlagChecker fc = new FlagChecker();
            if (fc.check(this.textFieldFlag.getText())) {
                this.textFieldFlag.setBorder(new LineBorder(Color.GREEN));
            }
            else {
                this.textFieldFlag.setBorder(new LineBorder(Color.RED));
            }
        }
```

- 其中答案正确与否是通过修改文本框边框颜色实现，不难看出只要方法`fc.check()`返回true即可通过，随后定位到该函数：

```java
public boolean check(final String flag) {
            if (flag.length() != 21) {
                return false;
            }
            final String encodeRow = "MCwyLDAsNiw4LDExLDExLDgsNyw4LDEzLDQsNiwxMSw4LDEyLDIsNiwwLDExLDE1";
            final String encodeColumn = "MTAsMTQsMiw4LDQsMywxMiw0LDIsNCwzLDgsOCwxMiw0LDEwLDE0LDQsNSwxMiwxMw==";
            final String indexRowString = new String(Base64.getDecoder().decode(encodeRow));
            final String indexColumnString = new String(Base64.getDecoder().decode(encodeColumn));
            final String[] indexRowArray = indexRowString.split(String.valueOf((char)this.s[4][2]));
            final String[] indexColumnArray = indexColumnString.split(String.valueOf((char)this.s[4][2]));
            if (indexRowArray.length != indexColumnArray.length || indexRowArray.length == 0) {
                return false;
            }
            for (int i = 0; i < indexRowArray.length; ++i) {
                if (flag.charAt(i) != this.s[Integer.valueOf(indexRowArray[i])][Integer.valueOf(indexColumnArray[i])]) {
                    return false;
                }
            }
            return true;
        }
```

- 该类中包含一个二维数组，通过代码推测，是从base64解码出一些二维数组坐标，再取出对应的值拼接成字符串即为答案，编写逆算法：

```java
int[][] s =  new int[][] { { 99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118 }, 
{ 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192 }, 
{ 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21 }, 
{ 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117 }, 
{ 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132 }, 
{ 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207 }, 
{ 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168 }, 
{ 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210 }, 
{ 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115 }, 
{ 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219 }, 
{ 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121 }, 
{ 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8 }, 
{ 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138 }, 
{ 112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158 }, 
{ 225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223 }, 
{ 140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22 } };
						
		final String encodeRow = "MCwyLDAsNiw4LDExLDExLDgsNyw4LDEzLDQsNiwxMSw4LDEyLDIsNiwwLDExLDE1";
		final String encodeColumn = "MTAsMTQsMiw4LDQsMywxMiw0LDIsNCwzLDgsOCwxMiw0LDEwLDE0LDQsNSwxMiwxMw==";
		final String indexRowString = new String(Base64.getDecoder().decode(encodeRow));
		final String indexColumnString = new String(Base64.getDecoder().decode(encodeColumn));
		final String[] indexRowArray = indexRowString.split(String.valueOf((char)s[4][2]));
		final String[] indexColumnArray = indexColumnString.split(String.valueOf((char)s[4][2]));
		StringBuilder flag = new StringBuilder();
		for (int i = 0; i < indexRowArray.length; ++i) {
			flag.append((char)s[Integer.valueOf(indexRowArray[i])][Integer.valueOf(indexColumnArray[i])]);
		}
		System.out.println(flag.toString());
```

- 程序输出：

```
g1wE_me_@_fREe_t1CkeT
```

- 答案：

```
flag{g1wE_me_@_fREe_t1CkeT}
```

## 2. RELF（200pt）
- 主要考点：Linux程序逆向
- 工具：IDA Pro

- 题目给出一ELF文件，64位，使用64位IDA Pro打开，定位到主函数：

```c
int __cdecl main(int argc, const char **argv, const char **envp){  unsigned int v3; // eax  char s; // [rsp+10h] [rbp-100h]  puts("Please input your flag:");  __isoc99_scanf("%s", &s);  v3 = strlen(&s);  f(&s, v3);  return 0;}
```

- 容易得知函数`f`是关键函数，定位到该函数：

```c
int __fastcall f(__int64 a1, signed int a2){  int result; // eax  if ( a2 > 0 && a2 <= 29 )    result = c(a1, (unsigned int)a2);  else    result = puts("Wrong");  return result;}
```

- 分析可知，该函数限制了输入长度不得超过29，同时定位到函数`c`：

```c
int __fastcall c(__int64 a1, int a2){  int v3; // [rsp+10h] [rbp-190h]  int v4; // [rsp+14h] [rbp-18Ch]  int v5; // [rsp+18h] [rbp-188h]  int v6; // [rsp+1Ch] [rbp-184h]  int v7; // [rsp+20h] [rbp-180h]  int v8; // [rsp+24h] [rbp-17Ch]  int v9; // [rsp+28h] [rbp-178h]  int v10; // [rsp+2Ch] [rbp-174h]  int v11; // [rsp+30h] [rbp-170h]  int v12; // [rsp+34h] [rbp-16Ch]  int v13; // [rsp+38h] [rbp-168h]  int v14; // [rsp+3Ch] [rbp-164h]  int v15; // [rsp+40h] [rbp-160h]  int v16; // [rsp+44h] [rbp-15Ch]  int v17; // [rsp+48h] [rbp-158h]  int v18; // [rsp+4Ch] [rbp-154h]  int v19; // [rsp+50h] [rbp-150h]  int v20; // [rsp+54h] [rbp-14Ch]  int v21; // [rsp+58h] [rbp-148h]  int v22; // [rsp+5Ch] [rbp-144h]  int v23; // [rsp+60h] [rbp-140h]  int v24; // [rsp+64h] [rbp-13Ch]  int v25; // [rsp+68h] [rbp-138h]  int v26; // [rsp+6Ch] [rbp-134h]  int v27; // [rsp+70h] [rbp-130h]  int v28; // [rsp+74h] [rbp-12Ch]  int v29; // [rsp+78h] [rbp-128h]  int v30; // [rsp+7Ch] [rbp-124h]  int v31; // [rsp+80h] [rbp-120h]  int v32; // [rsp+84h] [rbp-11Ch]  int v33; // [rsp+90h] [rbp-110h]  int v34; // [rsp+94h] [rbp-10Ch]  int v35; // [rsp+98h] [rbp-108h]  int v36; // [rsp+9Ch] [rbp-104h]  int v37; // [rsp+A0h] [rbp-100h]  int v38; // [rsp+A4h] [rbp-FCh]  int v39; // [rsp+A8h] [rbp-F8h]  int v40; // [rsp+ACh] [rbp-F4h]  int v41; // [rsp+B0h] [rbp-F0h]  int v42; // [rsp+B4h] [rbp-ECh]  int v43; // [rsp+B8h] [rbp-E8h]  int v44; // [rsp+BCh] [rbp-E4h]  int v45; // [rsp+C0h] [rbp-E0h]  int v46; // [rsp+C4h] [rbp-DCh]  int v47; // [rsp+C8h] [rbp-D8h]  int v48; // [rsp+CCh] [rbp-D4h]  int v49; // [rsp+D0h] [rbp-D0h]  int v50; // [rsp+D4h] [rbp-CCh]  int v51; // [rsp+D8h] [rbp-C8h]  int v52; // [rsp+DCh] [rbp-C4h]  int v53; // [rsp+E0h] [rbp-C0h]  int v54; // [rsp+E4h] [rbp-BCh]  int v55; // [rsp+E8h] [rbp-B8h]  int v56; // [rsp+ECh] [rbp-B4h]  int v57; // [rsp+F0h] [rbp-B0h]  int v58; // [rsp+F4h] [rbp-ACh]  int v59; // [rsp+F8h] [rbp-A8h]  int v60; // [rsp+FCh] [rbp-A4h]  int v61; // [rsp+100h] [rbp-A0h]  int v62; // [rsp+104h] [rbp-9Ch]  int v63; // [rsp+110h] [rbp-90h]  int v64; // [rsp+114h] [rbp-8Ch]  int v65; // [rsp+118h] [rbp-88h]  int v66; // [rsp+11Ch] [rbp-84h]  int v67; // [rsp+120h] [rbp-80h]  int v68; // [rsp+124h] [rbp-7Ch]  int v69; // [rsp+128h] [rbp-78h]  int v70; // [rsp+12Ch] [rbp-74h]  int v71; // [rsp+130h] [rbp-70h]  int v72; // [rsp+134h] [rbp-6Ch]  int v73; // [rsp+138h] [rbp-68h]  int v74; // [rsp+13Ch] [rbp-64h]  int v75; // [rsp+140h] [rbp-60h]  int v76; // [rsp+144h] [rbp-5Ch]  int v77; // [rsp+148h] [rbp-58h]  int v78; // [rsp+14Ch] [rbp-54h]  int v79; // [rsp+150h] [rbp-50h]  int v80; // [rsp+154h] [rbp-4Ch]  int v81; // [rsp+158h] [rbp-48h]  int v82; // [rsp+15Ch] [rbp-44h]  int v83; // [rsp+160h] [rbp-40h]  int v84; // [rsp+164h] [rbp-3Ch]  int v85; // [rsp+168h] [rbp-38h]  int v86; // [rsp+16Ch] [rbp-34h]  int v87; // [rsp+170h] [rbp-30h]  int v88; // [rsp+174h] [rbp-2Ch]  int v89; // [rsp+178h] [rbp-28h]  int v90; // [rsp+17Ch] [rbp-24h]  int v91; // [rsp+180h] [rbp-20h]  int v92; // [rsp+184h] [rbp-1Ch]  int v93; // [rsp+194h] [rbp-Ch]  int v94; // [rsp+198h] [rbp-8h]  int i; // [rsp+19Ch] [rbp-4h]  v63 = 34;  v64 = 29;  v65 = 76;  v66 = 98;  v67 = 43;  v68 = 52;  v69 = 97;  v70 = 24;  v71 = 40;  v72 = 23;  v73 = 4;  v74 = 69;  v75 = 44;  v76 = 52;  v77 = 43;  v78 = 80;  v79 = 1;  v80 = 63;  v81 = 79;  v82 = 48;  v83 = 55;  v84 = 8;  v85 = 26;  v86 = 95;  v87 = 4;  v88 = 34;  v89 = 82;  v90 = 11;  v91 = 51;  v92 = 81;  v33 = 297;  v34 = 169;  v35 = 431;  v36 = 229;  v37 = 423;  v38 = 257;  v39 = 465;  v40 = 288;  v41 = 437;  v42 = 86;  v43 = 364;  v44 = 123;  v45 = 390;  v46 = 485;  v47 = 232;  v48 = 352;  v49 = 244;  v50 = 469;  v51 = 54;  v52 = 359;  v53 = 16;  v54 = 110;  v55 = 355;  v56 = 244;  v57 = 256;  v58 = 94;  v59 = 187;  v60 = 19;  v61 = 242;  v62 = 432;  v3 = 3765;  v4 = 3301;  v5 = 7803;  v6 = 10323;  v7 = 5712;  v8 = 3845;  v9 = 5218;  v10 = 1968;  v11 = 4237;  v12 = 1696;  v13 = 704;  v14 = 7713;  v15 = 4746;  v16 = 6517;  v17 = 2339;  v18 = 4192;  v19 = 354;  v20 = 6454;  v21 = 6137;  v22 = 5015;  v23 = 6396;  v24 = 942;  v25 = 2825;  v26 = 8129;  v27 = 448;  v28 = 1760;  v29 = 9863;  v30 = 778;  v31 = 6617;  v32 = 432;  for ( i = 0; i < a2; ++i )  {    v94 = *(&v63 + i) * *(char *)(i + a1);    v93 = *(&v33 + i) + v94;    if ( v93 != *(&v3 + i) )      return puts("Wrong");  }  return printf("Your flag is: %s\n", a1);}
```

- 先不看数据区，观察for循环可以得知，变量a1为输入的flag，而变量v3、v33、v63都是数组。代码可以转换为以下形式：

```c
for ( i = 0; i < a2; ++i )  {    v94 = v63[i] * a1[i];    v93 = v33[i] + v94;    if ( v93 != v3[i] )      return puts("Wrong");  }  return printf("Your flag is: %s\n", a1);
```

- 可以看出，这是一个一元一次方程的形式，`v63[i] * a1[i] + v33[i] = v3[i]`，所以解此方程可得flag：

```
flag{E1F_FUnct10n_Math_S01vE}
```


## 3. SoPerfectApp（400pt）
- 主要考点：Android Java和So逆向、Xposed
- 工具：apktool、dex2jar、Luyten、IDA Pro

- 题目给出一apk文件，使用dex2jar工具将其转换为jar文件，工具报出错误：

```sh
$ d2j-dex2jar.sh SoPerfectApp.apk 
dex2jar SoPerfectApp.apk -> ./SoPerfectApp-dex2jar.jar
Detail Error Information in File ./SoPerfectApp-error.zip
Please report this file to http://code.google.com/p/dex2jar/issues/entry if possible.
```

- 但是实际上已经得到了正确的jar文件，可以继续查看，打开jar文件，定位到按钮点击事件方法：

```java
public void onClick(final View view) {
    MainActivity.this.check(MainActivity.this.flagText.getText().toString());
}
```

- 发现其调用了`check`方法，定位到该方法：

```java
public void check(String s) {
        if (this.realcheck(s)) {
            s = "RIGHT";
        }
        else {
            s = "WRONG";
        }
        Toast.makeText((Context)this, (CharSequence)s, 1).show();
}
```

- 调用了`realcheck`方法，定位到该方法：

```java
public boolean realcheck(final String s) {
        return false;
}
```

- 显然这个逻辑永远为假，我们必须找到真正的判断逻辑。经过观察，该apk中assets目录下包含`xposed_init`文件，说明这是一个Xposed模块。
- 根据其入口类，定位到`com.wrlus.caac.hook.HookEntry`类，发现该类实现了对`realcheck`方法的钩子：

```java
public void handleLoadPackage(final XC_LoadPackage$LoadPackageParam xc_LoadPackage$LoadPackageParam) throws Throwable {
        if (xc_LoadPackage$LoadPackageParam.packageName.equals("cn.edu.cauc.isecca.soperfectapp")) {
            XposedHelpers.findAndHookMethod("cn.edu.cauc.isecca.soperfectapp.MainActivity", xc_LoadPackage$LoadPackageParam.classLoader, "realcheck", new Object[] { String.class, new XC_MethodHook() {
                    protected void afterHookedMethod(final XC_MethodHook$MethodHookParam xc_MethodHook$MethodHookParam) throws Throwable {
                        super.afterHookedMethod(xc_MethodHook$MethodHookParam);
                        final Object[] args = xc_MethodHook$MethodHookParam.args;
                        final boolean b = false;
                        final String s = (String)args[0];
                        boolean b2 = b;
                        if (MainActivity.check1(s)) {
                            b2 = b;
                            if (MainActivity.check2(s)) {
                                b2 = true;
                            }
                        }
                        xc_MethodHook$MethodHookParam.setResult((Object)b2);
                    }
                    
                    protected void beforeHookedMethod(final XC_MethodHook$MethodHookParam xc_MethodHook$MethodHookParam) throws Throwable {
                        super.beforeHookedMethod(xc_MethodHook$MethodHookParam);
                        final Object[] args = xc_MethodHook$MethodHookParam.args;
                        final boolean b = false;
                        final String s = (String)args[0];
                        boolean b2 = b;
                        if (MainActivity.check1(s)) {
                            b2 = b;
                            if (MainActivity.check2(s)) {
                                b2 = true;
                            }
                        }
                        xc_MethodHook$MethodHookParam.setResult((Object)b2);
                    }
                } });
        }
    }
```

- 观察发现，其调用了`MainActivity`中的`check1`和`check2`方法，同时满足才可以通过检测，转到这两个方法发现其为native方法：

```java
public static native boolean check1(final String p0);
public static native boolean check2(final String p0);
```

- 解压该apk，寻找到该apk的so文件，使用32位的IDA Pro打开其ARM版本的so文件，定位到这两个方法：

```cpp
int __fastcall Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check1(_JNIEnv *a1, int a2, int a3){  int v3; // ST18_4  int v4; // ST14_4  unsigned __int8 v5; // ST23_1  signed int i; // [sp+1Ch] [bp-24h]  _BYTE *v8; // [sp+24h] [bp-1Ch]  int v9; // [sp+28h] [bp-18h]  _JNIEnv *v10; // [sp+30h] [bp-10h]  unsigned __int8 v11; // [sp+37h] [bp-9h]  v10 = a1;  v9 = a3;  v8 = (_BYTE *)_JNIEnv::GetStringUTFChars(a1, a3);  for ( i = 0; i <= 29; ++i )  {    if ( !v8[i] )    {      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      return 0;    }  }  if ( v8[30] )  {    _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);    v11 = 0;  }  else if ( *v8 == 102 && v8[1] == 108 && v8[2] == 97 && v8[3] == 103 && v8[4] == 123 && v8[29] == 125 )  {    if ( v8[21] == 97 && v8[22] == 110 && v8[23] == 121 )    {      v3 = _JNIEnv::FindClass(v10, "cn/edu/cauc/isecca/soperfectapp/Cpp2JavaBridge");      v4 = _JNIEnv::GetStaticMethodID(v10, v3, "chk", "(Ljava/lang/String;)Z", "(Ljava/lang/String;)Z");      v5 = _JNIEnv::CallStaticBooleanMethod(v10, v3, v4, v9);      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      v11 = v5;    }    else    {      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      v11 = 0;    }  }  else  {    _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);    v11 = 0;  }  return v11;}
```

```cpp
int __fastcall Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check2(_JNIEnv *a1, int a2, int a3){  int v3; // ST18_4  int v4; // ST14_4  unsigned __int8 v5; // ST23_1  signed int i; // [sp+1Ch] [bp-24h]  _BYTE *v8; // [sp+24h] [bp-1Ch]  int v9; // [sp+28h] [bp-18h]  _JNIEnv *v10; // [sp+30h] [bp-10h]  unsigned __int8 v11; // [sp+37h] [bp-9h]  v10 = a1;  v9 = a3;  v8 = (_BYTE *)_JNIEnv::GetStringUTFChars(a1, a3);  for ( i = 0; i <= 29; ++i )  {    if ( !v8[i] )    {      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      return 0;    }  }  if ( v8[30] )  {    _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);    v11 = 0;  }  else if ( *v8 == 102 && v8[1] == 108 && v8[2] == 97 && v8[3] == 103 && v8[4] == 123 && v8[29] == 125 )  {    if ( v8[24] == 116 && v8[25] == 104 && v8[26] == 105 && v8[27] == 110 && v8[28] == 103 )    {      v3 = _JNIEnv::FindClass(v10, "cn/edu/cauc/isecca/soperfectapp/Cpp2JavaBridge");      v4 = _JNIEnv::GetStaticMethodID(v10, v3, "chk", "(Ljava/lang/String;)Z", "(Ljava/lang/String;)Z");      v5 = _JNIEnv::CallStaticBooleanMethod(v10, v3, v4, v9);      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      v11 = v5;    }    else    {      _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);      v11 = 0;    }  }  else  {    _JNIEnv::ReleaseStringUTFChars(v10, v9, v8);    v11 = 0;  }  return v11;}
```

-  发现其逻辑基本一致，规定其长度为29，并且检查了一些字符，同时因为存在`CallStaticBooleanMethod`，所以还调用了Java方法，定位到该方法：

```java
package cn.edu.cauc.isecca.soperfectapp;

import android.util.*;

public class Cpp2JavaBridge
{
    private static String[] keywords;
    
    static {
        Cpp2JavaBridge.keywords = new String[] { "ZmxhZ3tYcG9zZWQ=", "Y2Fu", "SG9vaw==" };
    }
    
    public static boolean chk(final String s) {
        try {
            final String[] split = s.split("_");
            for (int i = 0; i < 3; ++i) {
                if (!split[i].equals(new String(Base64.decode(Cpp2JavaBridge.keywords[i], 0)))) {
                    return false;
                }
            }
            return true;
        }
        catch (Exception ex) {
            ex.printStackTrace();
            return false;
        }
    }
}
```

- 结合以上代码，得知check1需要满足的条件是：

```
flag{Xposed_can_Hook_any?????}
```

- check2需要满足的条件是：

```
flag{Xposed_can_Hook_???thing}
```

- 综合考虑，flag为：

```
flag{Xposed_can_Hook_anything}
```

## 4. 打个气（400pt）
- 主要考点：Windows程序逆向、Windows程序动态调试
- 工具：IDA Pro/OllyDbg

- 题目给出一exe文件，运行之发现要求我们猜数字，如果猜错就退出，所以考虑使用静态分析结合动态调试进行绕过，使用32位的IDA Pro打开文件，定位到主函数：

```c
int __cdecl main(int argc, const char **argv, const char **envp){  sub_403620();  if ( sub_402B20() && sub_402B20() && sub_402B20() && sub_402B20() && sub_402B20() )    sub_402B20();  system("pause");  return 0;}
```

- 发现需要连续调用六次`sub_402B20()`成功之后才可能显示flag，定位到该函数：

```c
signed int sub_402B20(){  unsigned int v0; // eax  signed int result; // eax  int v2; // [esp+18h] [ebp-10h]  int v3; // [esp+1Ch] [ebp-Ch]  sub_402AA4();  printf("=====Challenge %d=====\n", dword_404104);  puts("Please guess a number (1-6):");  v0 = time(0);  srand(v0);  v3 = 0;  if ( dword_404104 == dword_404040 )    v3 = rand() % 6 + 7;  else    v3 = rand() % 6 + 1;  v2 = 0;  scanf("%d", &v2);  if ( v2 > 0 && v2 <= 6 )  {    if ( v2 == v3 )    {      if ( dword_404104 == dword_404040 )      {        puts("Here is your flag:");        sub_401500();      }      else      {        puts("OK: See you next time! ");        ++dword_404104;      }      result = 1;    }    else    {      dword_404104 = 1;      puts("ERROR: Guess wrong! ");      result = 0;    }  }  else  {    dword_404104 = 1;    puts("ERROR: Out of range! ");    result = 0;  }  return result;}
```

- 观察其逻辑发现，前五次猜测需要猜中1-6之间的一个随机数，而第六次猜测输入的数字是1-6，但是生成的数字却是7-12之间的，所以不可能猜对。先尝试直接逆向生成flag的算法，定位到`sub_401500()`：

```c
int sub_401500(){  char v1[33]; // [esp+1Bh] [ebp-EDh]  int v2[33]; // [esp+3Ch] [ebp-CCh]  int v3; // [esp+C3h] [ebp-45h]  int v4; // [esp+C7h] [ebp-41h]  int v5; // [esp+CBh] [ebp-3Dh]  int v6; // [esp+CFh] [ebp-39h]  int v7; // [esp+D3h] [ebp-35h]  int v8; // [esp+D7h] [ebp-31h]  int v9; // [esp+DBh] [ebp-2Dh]  int v10; // [esp+DFh] [ebp-29h]  char v11; // [esp+E3h] [ebp-25h]  int v12; // [esp+E4h] [ebp-24h]  int v13; // [esp+E8h] [ebp-20h]  int i; // [esp+ECh] [ebp-1Ch]  v3 = 858927408;  v4 = 926299444;  v5 = 1650538808;  v6 = 1717920867;  v7 = 1667523942;  v8 = 943284578;  v9 = 875902519;  v10 = 808530483;  v11 = 0;  qmemcpy(v2, &unk_404080, sizeof(v2));  v13 = 32;  for ( i = 0; 100 * v13 > i; ++i )  {    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;    v12 = *((char *)&v3 + (i ^ 3) % v13);    *((_BYTE *)&v3 + (i ^ 3) % v13) = *((_BYTE *)&v3 + i * i % v13);    *((_BYTE *)&v3 + i * i % v13) = v12;    v12 = *((char *)&v3 + 7 * i % v13);    *((_BYTE *)&v3 + 7 * i % v13) = *((_BYTE *)&v3 + 13 * i % v13);    *((_BYTE *)&v3 + 13 * i % v13) = v12;    v12 = *((char *)&v3 + (i | 0xC) % v13);    *((_BYTE *)&v3 + (i | 0xC) % v13) = *((_BYTE *)&v3 + 8 * i % v13);    *((_BYTE *)&v3 + 8 * i % v13) = v12;    v12 = *((char *)&v3 + 3 * i % v13);    *((_BYTE *)&v3 + 3 * i % v13) = *((_BYTE *)&v3 + 17 * i % v13);    *((_BYTE *)&v3 + 17 * i % v13) = v12;  }  for ( i = 0; (unsigned int)i <= 0x83; ++i )    v1[i] = *((_BYTE *)&v3 + i) ^ LOBYTE(v2[i]);  return printf("flag{%s}\n", v1);}
```

- 发现逻辑过于复杂，不便于分析，本题最佳解法不是分析这个函数，而是进行动态调试，观察`sub_402B20()`的汇编代码：

```asm
.text:00402C10 loc_402C10:                             ; CODE XREF: sub_402B20+D1↑j.text:00402C10                 mov     eax, [ebp+var_10].text:00402C13                 cmp     eax, [ebp+var_C].text:00402C16                 jnz     short loc_402C5F.text:00402C18                 mov     edx, dword_404104.text:00402C1E                 mov     eax, dword_404040.text:00402C23                 cmp     edx, eax.text:00402C25                 jnz     short loc_402C3F.text:00402C27                 mov     dword ptr [esp], offset aHereIsYourFlag ; "Here is your flag:".text:00402C2E                 call    puts.text:00402C33                 call    sub_401500.text:00402C38                 mov     eax, 1.text:00402C3D                 jmp     short locret_402C7A
```

- 发现程序通过cmp比较之后进行jnz跳转，所以我们只需要在jnz处下断点，并且修改标志寄存器中ZF标志位的值，即可实现每次都判断为成功的逻辑。

```asm
cmp     eax, [ebp+var_C]jnz     short loc_402C5F
```

- 经过动态调试绕过六次猜数字游戏后，得到输出的flag为：

```
flag{0llyDbg0rIDAPr0_To_DebUg_A5_weLL}
```