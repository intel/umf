#include<string>
#include<vector>
#include "vmf/types.hpp"
#include "../com_intel_vmf_vmf_vec2d.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
 * Class:     com_intel_vmf_vmf_vec2d
 * Method:    n_vmf_vec2d
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec2d_n_1vmf_1vec2d__ (JNIEnv *env, jclass)
{
    std::shared_ptr<vmf_vec2d>* p = new std::shared_ptr<vmf_vec2d>(new vmf_vec2d());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_vmf_vec2d
 * Method:    n_vmf_vec2d
 * Signature: (DD)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec2d_n_1vmf_1vec2d__DD (JNIEnv *env, jclass, jdouble x, jdouble y)
{
    std::shared_ptr<vmf_vec2d>* p = new std::shared_ptr<vmf_vec2d>(new vmf_vec2d((double)x, (double)y));
    return (jlong)p;
}

/*
 * Class:     com_intel_vmf_vmf_vec2d
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec2d_n_1setTo (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    std::shared_ptr<vmf_vec2d>* self = (std::shared_ptr<vmf_vec2d>*) selfAddr;
    std::shared_ptr<vmf_vec2d>* other = (std::shared_ptr<vmf_vec2d>*) otherAddr;

    (**self) = (**other);
}

/*
* Class:     com_intel_vmf_vmf_vec2d
* Method:    n_get
* Signature: (J)[D
*/
JNIEXPORT jdoubleArray JNICALL Java_com_intel_vmf_vmf_1vec2d_n_1get (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec2d>* obj = (std::shared_ptr<vmf_vec2d>*) self;

    jdoubleArray values = env->NewDoubleArray(2);
    jdouble* cArray = env->GetDoubleArrayElements(values, 0);

    cArray[0] = (jdouble)((**obj).x);
    cArray[1] = (jdouble)((**obj).y);

    env->ReleaseDoubleArrayElements(values, cArray, 0);
    return values;
}

/*
 * Class:     com_intel_vmf_vmf_vec2d
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec2d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec2d>* p = (std::shared_ptr<vmf_vec2d>*) self;
    delete p;
}