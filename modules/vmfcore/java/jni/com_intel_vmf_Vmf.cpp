#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
* Class:     com_intel_vmf_Vmf
* Method:    n_initialize
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1initialize(JNIEnv *env, jclass);


JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1initialize (JNIEnv *env, jclass)
{
    static const char method_name[] = "Vmf::n_1initialize";

    try
    {
        //vmf::initialize();
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
* Class:     com_intel_vmf_Vmf
* Method:    n_terminate
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1terminate(JNIEnv *env, jclass);


JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1terminate (JNIEnv *env, jclass)
{
    static const char method_name[] = "Vmf::n_1terminate";

    try
    {
        //vmf::terminate();
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


}
