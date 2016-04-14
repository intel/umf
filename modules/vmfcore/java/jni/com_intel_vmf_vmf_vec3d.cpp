#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_vmf_vec3d
 * Method:    n_vmf_vec3d
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__(JNIEnv *env, jclass)
{
    static const char method_name[] = "vmf_vec3d::n_1vec3d__";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = new std::shared_ptr<vmf_vec3d>(new vmf_vec3d());
        return (jlong)obj;
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
 * Method:    n_vmf_vec3d
 * Signature: (DDD)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__DDD(JNIEnv *env, jclass, jdouble x, jdouble y, jdouble z);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1vmf_1vec3d__DDD (JNIEnv *env, jclass, jdouble x, jdouble y, jdouble z)
{
    static const char method_name[] = "vmf_vec3d::n_1vec3d__DDD";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = new std::shared_ptr<vmf_vec3d>(new vmf_vec3d((double)x, (double)y, (double)z));
        return (jlong)obj;
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
 * Method:    n_setTo
 * Signature: (JJ)V
 */
/*JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1setTo(JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1setTo (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "vmf_vec3d::n_1setTo";

    try
    {
        std::shared_ptr<vmf_vec3d>* self = (std::shared_ptr<vmf_vec3d>*) selfAddr;
        std::shared_ptr<vmf_vec3d>* other = (std::shared_ptr<vmf_vec3d>*) otherAddr;

        if ((self == NULL) || (self->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Vec3d is null pointer.");

        if ((other == NULL) || (other->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Other vec3d is null pointer.");

        (**self) = (**other);
    }
    catch (const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}*/

/*
* Class:     com_intel_vmf_vmf_vec3d
* Method:    n_getX
* Signature: (J)D
*/
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getX(JNIEnv *env, jclass, jlong self);


JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getX(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getX";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getY(JNIEnv *env, jclass, jlong self);


JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getY(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getY";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getZ(JNIEnv *env, jclass, jlong self);


JNIEXPORT jdouble JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1getZ (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1getZ";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_vmf_1vec3d_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "vmf_vec3d::n_1delete";

    try
    {
        std::shared_ptr<vmf_vec3d>* obj = (std::shared_ptr<vmf_vec3d>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Vec3d is null pointer.");

        delete obj;
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
 