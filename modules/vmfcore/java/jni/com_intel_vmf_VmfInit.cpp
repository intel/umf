#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_VmfInit.h"

using namespace vmf;

/*
 * Class:     com_intel_vmf_VmfInit
 * Method:    n_vmfInitialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_VmfInit_n_1vmfInitialize (JNIEnv *env, jclass)
{
    vmf::initialize ();
}

/*
 * Class:     com_intel_vmf_VmfInit
 * Method:    n_vmfTerminate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_VmfInit_n_1vmfTerminate (JNIEnv *env, jclass)
{
    vmf::terminate ();
}
