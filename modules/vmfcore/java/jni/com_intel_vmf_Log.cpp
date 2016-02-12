#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/log.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
* Class:     com_intel_vmf_Log
* Method:    n_log
* Signature: (ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1log(JNIEnv *env, jclass, jint level, jstring msg, jstring func, jstring file, jint line);


JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1log(JNIEnv *env, jclass, jint level, jstring msg, jstring func, jstring file, jint line)
{
    static const char method_name[] = "Log::n_1log";

    try
    {
        const char* message = env->GetStringUTFChars(msg, NULL);
        const char* function = env->GetStringUTFChars(func, NULL);
        const char* path = env->GetStringUTFChars(file, NULL);

        std::string sMsg(message);
        env->ReleaseStringUTFChars(msg, message);
        std::string sFunc(function);
        env->ReleaseStringUTFChars(func, function);
        std::string sPath(path);
        env->ReleaseStringUTFChars(file, path);

        Log::log((LogLevel)level, sMsg, sFunc, sPath, (int)line);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
* Class:     com_intel_vmf_Log
* Method:    n_logToFile
* Signature: (Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToFile(JNIEnv *env, jclass, jstring file);


JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToFile (JNIEnv *env, jclass, jstring file)
{
    static const char method_name[] = "Log::n_1logToFile";

    try
    {
        const char* path = env->GetStringUTFChars(file, NULL);
        std::string sPath(path);
        Log::logToFile(sPath);
        env->ReleaseStringUTFChars(file, path);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_logToConsole
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToConsole(JNIEnv *env, jclass);


JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToConsole (JNIEnv *env, jclass)
{
    static const char method_name[] = "Log::n_1logToConsole";

    try
    {
        Log::logToConsole();
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_setVerbosityLevel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1setVerbosityLevel(JNIEnv *env, jclass, jint level);


JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1setVerbosityLevel (JNIEnv *env, jclass, jint level)
{
    static const char method_name[] = "Log::n_1setVerbosityLevel";

    try
    {
        Log::setVerbosityLevel((LogLevel)level);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_getVerbosityLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Log_n_1getVerbosityLevel(JNIEnv *env, jclass);


JNIEXPORT jint JNICALL Java_com_intel_vmf_Log_n_1getVerbosityLevel (JNIEnv *env, jclass)
{
    static const char method_name[] = "Log::n_1getVerbosityLevel";

    try
    {
        return Log::getVerbosityLevel();
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }

    return 0;
}


}
