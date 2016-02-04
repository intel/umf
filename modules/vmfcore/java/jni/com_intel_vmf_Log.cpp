#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/log.hpp"
//#include "../com_intel_vmf_Log.h"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

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