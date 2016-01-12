#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_Vmf.h"

using namespace vmf;

/*
* Class:     com_intel_vmf_Vmf
* Method:    n_initialize
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1initialize (JNIEnv *env, jclass)
{
    vmf::initialize ();
}

/*
* Class:     com_intel_vmf_Vmf
* Method:    n_terminate
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Vmf_n_1terminate (JNIEnv *env, jclass)
{
    vmf::terminate ();
}
