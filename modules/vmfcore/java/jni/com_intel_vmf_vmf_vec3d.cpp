#include<string>
#include<vector>
#include "vmf/types.hpp"
#include "../com_intel_vmf_vmf_vec3d.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_vmf_vec3d
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__(JNIEnv *, jclass)
{
    std::shared_ptr<vmf_vec3d>* p = new std::shared_ptr<vmf_vec3d>(new vmf_vec3d());
    return (jlong)p;
}

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_vmf_vec3d
 * Signature: (DDD)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__DDD (JNIEnv *, jclass, jdouble x, jdouble y, jdouble z)
{
    std::shared_ptr<vmf_vec3d>* p = new std::shared_ptr<vmf_vec3d>(new vmf_vec3d((double)x, (double)y, (double)z));
    return (jlong)p;
}

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1setTo (JNIEnv *, jclass, jlong selfAddr, jlong otherAddr)
{
    std::shared_ptr<vmf_vec3d>* self = (std::shared_ptr<vmf_vec3d>*) selfAddr;
    std::shared_ptr<vmf_vec3d>* other = (std::shared_ptr<vmf_vec3d>*) otherAddr;

    (**self) = (**other);
}

/*
* Class:     com_intel_vmf_vmf_vec3d
* Method:    n_get
* Signature: (J)[D
*/
JNIEXPORT jdoubleArray JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1get (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;

    jdoubleArray values = env->NewDoubleArray(3);
    jdouble* cArray = env->GetDoubleArrayElements(values, 0);

    cArray[0] = (jdouble)((**obj).x);
    cArray[1] = (jdouble)((**obj).y);
    cArray[2] = (jdouble)((**obj).z);

    env->ReleaseDoubleArrayElements(values, cArray, 0);
    return values;
}

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec3d>* p = (std::shared_ptr<vmf_vec3d>*) self;
    delete p;
}
  