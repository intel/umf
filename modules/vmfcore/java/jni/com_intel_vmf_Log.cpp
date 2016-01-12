#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/log.hpp"
#include "../com_intel_vmf_Log.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
* Class:     com_intel_vmf_Log
* Method:    n_logToFile
* Signature: (Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToFile (JNIEnv *env, jclass, jstring file)
{
    std::string sPath(env->GetStringUTFChars(file, NULL));
    Log::logToFile(sPath);
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_logToConsole
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1logToConsole (JNIEnv *env, jclass)
{
    Log::logToConsole();
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_setVerbosityLevel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Log_n_1setVerbosityLevel (JNIEnv *env, jclass, jint level)
{
    Log::setVerbosityLevel((LogLevel)level);
}

/*
 * Class:     com_intel_vmf_Log
 * Method:    n_getVerbosityLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Log_n_1getVerbosityLevel (JNIEnv *env, jclass)
{
    return Log::getVerbosityLevel();
}
