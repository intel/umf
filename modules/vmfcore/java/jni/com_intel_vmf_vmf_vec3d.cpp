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
* Method:    n_getX
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getX(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getX";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;
        return (jdouble)((**obj).x);
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

/*
* Class:     com_intel_vmf_vmf_vec3d
* Method:    n_getY
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getY(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getY";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;
        return (jdouble)((**obj).y);
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

/*
* Class:     com_intel_vmf_vmf_vec3d
* Method:    n_getZ
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getZ (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getZ";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;
        return (jdouble)((**obj).z);
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

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1delete";

    try
    {
        std::shared_ptr<vmf_vec3d>* p = (std::shared_ptr<vmf_vec3d>*) self;
        delete p;
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
  