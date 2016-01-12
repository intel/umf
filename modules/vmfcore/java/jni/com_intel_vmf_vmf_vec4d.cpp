#include<string>
#include<vector>
#include "vmf/types.hpp"
#include "../com_intel_vmf_vmf_vec2d.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
 * Class:     com_intel_vmf_vmf_vec4d
 * Method:    n_vmf_vec4d
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1vmf_1vec4d__ (JNIEnv *, jclass)
{
    std::shared_ptr<vmf_vec4d>* p = new std::shared_ptr<vmf_vec4d>(new vmf_vec4d());
    return (jlong)p;
}

/*
 * Class:     com_intel_vmf_vmf_vec4d
 * Method:    n_vmf_vec4d
 * Signature: (DDDD)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1vmf_1vec4d__DDDD (JNIEnv *, jclass, jdouble x, jdouble y, jdouble z, jdouble w)
{
    std::shared_ptr<vmf_vec4d>* p = new std::shared_ptr<vmf_vec4d>(new vmf_vec4d((double)x, (double)y, (double)z, (double)w));
    return (jlong)p;
}

/*
 * Class:     com_intel_vmf_vmf_vec4d
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1setTo (JNIEnv *, jclass, jlong selfAddr, jlong otherAddr)
{
    std::shared_ptr<vmf_vec4d>* self = (std::shared_ptr<vmf_vec4d>*) selfAddr;
    std::shared_ptr<vmf_vec4d>* other = (std::shared_ptr<vmf_vec4d>*) otherAddr;

    (**self) = (**other);
}

/*
* Class:     com_intel_vmf_vmf_vec4d
* Method:    n_get
* Signature: (J)[D
*/
JNIEXPORT jdoubleArray JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1get (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec4d>* obj = (std::shared_ptr<vmf_vec4d>*) self;

    jdoubleArray values = env->NewDoubleArray(4);
    jdouble* cArray = env->GetDoubleArrayElements(values, 0);

    cArray[0] = (jdouble)((**obj).x);
    cArray[1] = (jdouble)((**obj).y);
    cArray[2] = (jdouble)((**obj).z);
    cArray[3] = (jdouble)((**obj).w);

    env->ReleaseDoubleArrayElements(values, cArray, 0);
    return values;
}

/*
 * Class:     com_intel_vmf_vmf_vec4d
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec4d>* p = (std::shared_ptr<vmf_vec4d>*) self;
    delete p;
}