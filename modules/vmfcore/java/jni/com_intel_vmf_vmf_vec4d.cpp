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
* Method:    n_getX
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1getX (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec4d::n_1getX";

    try
    {
        std::shared_ptr<vmf_vec4d>* obj = (std::shared_ptr<vmf_vec4d>*) self;
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
* Class:     com_intel_vmf_vmf_vec4d
* Method:    n_getY
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1getY (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec4d::n_1getY";

    try
    {
        std::shared_ptr<vmf_vec4d>* obj = (std::shared_ptr<vmf_vec4d>*) self;
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
* Class:     com_intel_vmf_vmf_vec4d
* Method:    n_getZ
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1getZ (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec4d::n_1getZ";

    try
    {
        std::shared_ptr<vmf_vec4d>* obj = (std::shared_ptr<vmf_vec4d>*) self;
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
* Class:     com_intel_vmf_vmf_vec4d
* Method:    n_getW
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1getW (JNIEnv *env, jclass, jlong self)
{

    static const char method_name[] = "vmf_vec4d::n_1getW";

    try
    {
        std::shared_ptr<vmf_vec4d>* obj = (std::shared_ptr<vmf_vec4d>*) self;
        return (jdouble)((**obj).w);
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
 * Class:     com_intel_vmf_vmf_vec4d
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec4d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<vmf_vec4d>* p = (std::shared_ptr<vmf_vec4d>*) self;
    delete p;
}