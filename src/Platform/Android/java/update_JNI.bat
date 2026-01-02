@echo off

SET java_path="C:\NVPACK\jdk1.7.0_71\bin"
SET android_path="C:\NVPACK\android-sdk-windows\platforms\android-19\android.jar"
SET "src_path=LynxEngine/Android/JNI.java"
SET "package=LynxEngine.Android.JNI"
SET "jni_header=LynxAndroidJNI.h"
cd src

%java_path%\javac -classpath %android_path%;. %src_path%
%java_path%\javah -jni -classpath %android_path%;. %package%
if exist %jni_header% del %jni_header% /F /Q
rename LynxEngine_Android_JNI.h  %jni_header%
XCOPY %jni_header% ..\..\..\..\inc\Platform\Android\%jni_header% /D /Y /I /R /E

echo done.

pause