#include<string>
#include<vector>
#include <iostream>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
* Class:     com_intel_vmf_Variant
* Method:    n_Variant
* Signature: ()J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant (JNIEnv *env, jclass)
{
    static const char method_name[] = "Variant::n_1Variant";

    try
    {
        std::shared_ptr <Variant>* p = new std::shared_ptr <Variant>(new Variant());
        return (jlong)p;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_Variant
 * Signature: (J)J
 */
/*JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant__J(JNIEnv *env, jclass, jlong otherAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant__J (JNIEnv *env, jclass, jlong otherAddr)
{
    static const char method_name[] = "Variant::n_1Variant__J";

    try
    {
        std::shared_ptr <Variant>* other = (std::shared_ptr <Variant>*)otherAddr;
        std::shared_ptr <Variant>* obj = new std::shared_ptr <Variant>(new Variant(**other));
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
}*/

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JI(JNIEnv *env, jclass, jlong self, jint value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JI (JNIEnv *env, jclass, jlong self, jint value)
{
    static const char method_name[] = "Variant::n_1setTo__JI";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        (**obj) = (int) value;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JF)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JF(JNIEnv *env, jclass, jlong self, jfloat value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JF (JNIEnv *env, jclass, jlong self, jfloat value)
{
    static const char method_name[] = "Variant::n_1setTo__JF";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        (**obj) = (float) value;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (J[I)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3I(JNIEnv *env, jclass, jlong self, jintArray values);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3I (JNIEnv *env, jclass, jlong self, jintArray values)
{
    static const char method_name[] = "Variant::n_1setTo__J_3I";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector <int> vecValues;
        jint *cArray = env->GetIntArrayElements(values, 0);
        jsize len = env->GetArrayLength(values);

        for (int i = 0; i < len; i++)
        {
            vecValues.push_back((int)cArray[i]);
        }

        env->ReleaseIntArrayElements(values, cArray, 0);
        (**obj) = vecValues;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (J[F)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3F(JNIEnv *env, jclass, jlong self, jfloatArray values);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3F (JNIEnv *env, jclass, jlong self, jfloatArray values)
{
    static const char method_name[] = "Variant::n_1setTo__J_3F";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector <float> vecValues;
        jfloat* array = env->GetFloatArrayElements(values, 0);
        jsize len = env->GetArrayLength(values);

        for (int i = 0; i < len; i++)
        {
            float elem = (float)array[i];
            vecValues.push_back(elem);
        }

        env->ReleaseFloatArrayElements(values, array, 0);
        (**obj) = vecValues;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JLjava_lang_String_2(JNIEnv *env, jclass, jlong self, jstring str);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring str)
{
    static const char method_name[] = "Variant::n_1setTo__JLjava_lang_String_2";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        const char* tmp = env->GetStringUTFChars(str, NULL);
        std::string value(tmp);
        (**obj) = (vmf_string)value;

        env->ReleaseStringUTFChars(str, tmp);
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (JD)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JD(JNIEnv *env, jclass, jlong self, jdouble value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JD(JNIEnv *env, jclass, jlong self, jdouble value)
{
    static const char method_name[] = "Variant::n_1setTo__JD";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        (**obj) = (vmf_real)value;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (JJ)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JJ(JNIEnv *env, jclass, jlong self, jlong value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JJ(JNIEnv *env, jclass, jlong self, jlong value)
{
    static const char method_name[] = "Variant::n_1setTo__JJ";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        (**obj) = (vmf_integer)value;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (J[Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3Ljava_lang_String_2(JNIEnv *env, jclass, jlong self, jobjectArray stringArray);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3Ljava_lang_String_2(JNIEnv *env, jclass, jlong self, jobjectArray stringArray)
{
    static const char method_name[] = "Variant::n_1setTo__J_3Ljava_lang_String_2";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector<vmf_string> stringVector;

        int stringCount = env->GetArrayLength(stringArray);

        for (int i = 0; i < stringCount; i++)
        {
            jstring string = (jstring)env->GetObjectArrayElement(stringArray, i);
            const char* rawString = env->GetStringUTFChars(string, NULL);
            vmf_string tmp(rawString);

            env->ReleaseStringUTFChars(string, rawString);
            stringVector.push_back(tmp);
        }

        (**obj) = stringVector;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (J[D)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3D(JNIEnv *env, jclass, jlong self, jdoubleArray values);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3D(JNIEnv *env, jclass, jlong self, jdoubleArray values)
{
    static const char method_name[] = "Variant::n_1setTo__J_3D";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector <vmf_real> vecValues;
        jdouble* array = env->GetDoubleArrayElements(values, 0);
        jsize len = env->GetArrayLength(values);

        for (int i = 0; i < len; i++)
        {
            vmf_real elem = (vmf_real)array[i];
            vecValues.push_back(elem);
        }

        env->ReleaseDoubleArrayElements(values, array, 0);
        (**obj) = vecValues;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (J[J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3J(JNIEnv *env, jclass, jlong self, jlongArray values);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3J (JNIEnv *env, jclass, jlong self, jlongArray values)
{
    static const char method_name[] = "Variant::n_1setTo__J_3J";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector <vmf_integer> vecValues;
        jlong* cArray = env->GetLongArrayElements (values, 0);
        jsize len = env->GetArrayLength(values);

        for (int i = 0; i < len; i++)
        {
            vmf_integer elem = (vmf_integer)cArray[i];
            vecValues.push_back(elem);
        }

        env->ReleaseLongArrayElements(values, cArray, 0);
        (**obj) = vecValues;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setTo
* Signature: (J[B)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3B(JNIEnv *env, jclass, jlong self, jbyteArray buffer);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3B(JNIEnv *env, jclass, jlong self, jbyteArray buffer)
{
    static const char method_name[] = "Variant::n_1setTo__J_3B";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        const char* array = (char*)env->GetByteArrayElements(buffer, 0);
        jsize len = env->GetArrayLength(buffer);
        vmf_rawbuffer rb(array, (size_t)len);
        env->ReleaseByteArrayElements(buffer, (jbyte*)array, 0);
        (**obj) = rb;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec2d
* Signature: (JJ)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec2d(JNIEnv *env, jclass, jlong self, jlong vec2dAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec2d (JNIEnv *env, jclass, jlong self, jlong vec2dAddr)
{
    static const char method_name[] = "Variant::n_1setToVec2d";
    
    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::shared_ptr <vmf_vec2d>* vec2d = (std::shared_ptr <vmf_vec2d>*) vec2dAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        if ((vec2d == NULL) || (vec2d->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Vec2d is null pointer.");

        (**obj) = (**vec2d);
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec3d
* Signature: (JJ)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec3d(JNIEnv *env, jclass, jlong self, jlong vec3dAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec3d (JNIEnv *env, jclass, jlong self, jlong vec3dAddr)
{
    static const char method_name[] = "Variant::n_1setToVec3d";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::shared_ptr <vmf_vec3d>* vec3d = (std::shared_ptr <vmf_vec3d>*) vec3dAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        if ((vec3d == NULL) || (vec3d->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Vec3d is null pointer.");

        (**obj) = (**vec3d);
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec4d
* Signature: (JJ)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec4d(JNIEnv *env, jclass, jlong self, jlong vec4dAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec4d (JNIEnv *env, jclass, jlong self, jlong vec4dAddr)
{
    static const char method_name[] = "Variant::n_1setToVec4d";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::shared_ptr <vmf_vec4d>* vec4d = (std::shared_ptr <vmf_vec4d>*) vec4dAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        if ((vec4d == NULL) || (vec4d->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Vec4d is null pointer.");

        (**obj) = (**vec4d);
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec2dArray
* Signature: (J[J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec2dArray(JNIEnv *env, jclass, jlong self, jlongArray vec2dArray);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec2dArray (JNIEnv *env, jclass, jlong self, jlongArray vec2dArray)
{
    static const char method_name[] = "Variant::n_1setToVec2dArray";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector<vmf_vec2d> vec;

        jlong* nativeAddrs = env->GetLongArrayElements(vec2dArray, 0);
        jsize len = env->GetArrayLength(vec2dArray);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<vmf_vec2d>* p = (std::shared_ptr<vmf_vec2d>*)nativeAddrs[i];
            vec.push_back(**p);
        }

        env->ReleaseLongArrayElements(vec2dArray, nativeAddrs, 0);
        (**obj) = vec;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec3dArray
* Signature: (J[J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec3dArray(JNIEnv *env, jclass, jlong self, jlongArray vec3dArray);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec3dArray (JNIEnv *env, jclass, jlong self, jlongArray vec3dArray)
{
    static const char method_name[] = "Variant::n_1setToVec3dArray";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector<vmf_vec3d> vec;

        jlong* nativeAddrs = env->GetLongArrayElements(vec3dArray, 0);
        jsize len = env->GetArrayLength(vec3dArray);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<vmf_vec3d>* p = (std::shared_ptr<vmf_vec3d>*)nativeAddrs[i];
            vec.push_back(**p);
        }

        env->ReleaseLongArrayElements(vec3dArray, nativeAddrs, 0);
        (**obj) = vec;
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
* Class:     com_intel_vmf_Variant
* Method:    n_setToVec4dArray
* Signature: (J[J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec4dArray(JNIEnv *env, jclass, jlong self, jlongArray vec4dArray);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setToVec4dArray (JNIEnv *env, jclass, jlong self, jlongArray vec4dArray)
{
    static const char method_name[] = "Variant::n_1setToVec4dArray";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        std::vector<vmf_vec4d> vec;

        jlong* nativeAddrs = env->GetLongArrayElements(vec4dArray, 0);
        jsize len = env->GetArrayLength(vec4dArray);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<vmf_vec4d>* p = (std::shared_ptr<vmf_vec4d>*)nativeAddrs[i];
            vec.push_back(**p);
        }

        env->ReleaseLongArrayElements(vec4dArray, nativeAddrs, 0);
        (**obj) = vec;
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
* Class:     com_intel_vmf_Variant
* Method:    n_set
* Signature: (JJ)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1set(JNIEnv *env, jclass, jlong self, jlong otherAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1set (JNIEnv *env, jclass, jlong self, jlong otherAddr)
{
    static const char method_name[] = "Variant::n_1set";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::shared_ptr <Variant>* other = (std::shared_ptr <Variant>*)otherAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        if ((other == NULL) || (other->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Other variant is null pointer.");

        (**obj) = (**other);
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1equals(JNIEnv *env, jclass, jlong self, jlong otherAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1equals (JNIEnv *env, jclass, jlong self, jlong otherAddr)
{
    static const char method_name[] = "Variant::n_1equals";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::shared_ptr <Variant>* other = (std::shared_ptr <Variant>*)otherAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        if ((other == NULL) || (other->get() == NULL))
            return JNI_FALSE;

        return (**obj) == (**other) ? JNI_TRUE : JNI_FALSE;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_toString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1toString(JNIEnv *env, jclass, jlong self, jboolean withType);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1toString(JNIEnv *env, jclass, jlong self, jboolean withType)
{
    static const char method_name[] = "Variant::n_1toString";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::string str = (*obj)->toString(withType == JNI_TRUE);
        return env->NewStringUTF(str.c_str());
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_fromString
 * Signature: (JILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1fromString(JNIEnv *env, jclass, jlong self, jint type, jstring value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1fromString (JNIEnv *env, jclass, jlong self, jint type, jstring value)
{
    static const char method_name[] = "Variant::n_1fromString";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        const char* tmp = env->GetStringUTFChars(value, NULL);
        std::string str (tmp);
        env->ReleaseStringUTFChars(value, tmp);

        Variant::Type Type = (Variant::Type) type;

        (*obj)->fromString (Type, str);   
    } 
    catch(const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    } 
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

// fromString(string)
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1fromString2(JNIEnv *env, jclass, jlong self, jstring value);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1fromString2(JNIEnv *env, jclass, jlong self, jstring value)
{
    static const char method_name[] = "Variant::n_1fromString2";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        if (obj == NULL || *obj == NULL) VMF_EXCEPTION(NullPointerException, "Variant (self) is null pointer.");

        const char* tmp = env->GetStringUTFChars(value, NULL);
        std::string str(tmp);
        env->ReleaseStringUTFChars(value, tmp);

        (*obj)->fromString(str);
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1getType(JNIEnv *env, jclass, jlong self);


JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1getType (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1getType";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jint) (*obj)->getType ();
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_isEmpty
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isEmpty(JNIEnv *env, jclass, jlong self);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isEmpty (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1isEmpty";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_TRUE;

        return (jboolean) (*obj)->isEmpty ();
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_getTypeName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1getTypeName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1getTypeName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1getTypeName";

    try
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::string str = (*obj)->getTypeName();
        return env->NewStringUTF(str.c_str());
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_convertTo
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1convertTo(JNIEnv *env, jclass, jlong self, jint type);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1convertTo (JNIEnv *env, jclass, jlong self, jint type)
{
    static const char method_name[] = "Variant::n_1convertTo";
    
    try 
    {
        Variant::Type Type = (Variant::Type) type;
        if ((Type >= Variant::Type::type_empty) && (Type <= Variant::Type::type_vec4d_vector))
        {
            std::shared_ptr <FieldValue>* obj = (std::shared_ptr <FieldValue>*)self;

            if ((obj == NULL) || (obj->get() == NULL))
                VMF_EXCEPTION(NullPointerException, "Field value is null pointer.");

            (*obj)->convertTo(Type);
        }
        else
            VMF_EXCEPTION(OutOfRangeException, "Non-existent type.");
    }
    catch(const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    } 
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_typeToString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1typeToString(JNIEnv *env, jclass, jint type);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1typeToString (JNIEnv *env, jclass, jint type)
{
    static const char method_name[] = "Variant::n_1typeToString";
    
    try 
    {
        Variant::Type Type = (Variant::Type) type;

        if (Type >= Variant::Type::type_empty && Type <= Variant::Type::type_vec4d_vector)
        {
            std::string str = Variant::typeToString (Type);
            return env->NewStringUTF (str.c_str());
        }
        else
            VMF_EXCEPTION(OutOfRangeException, "Non-existent type.");
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_typeFromString
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1typeFromString(JNIEnv *env, jclass, jstring type);


JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1typeFromString (JNIEnv *env, jclass, jstring type)
{
    static const char method_name[] = "Variant::n_1typeFromString";
     
    try 
    {
        const char* tmp = env->GetStringUTFChars(type, NULL);
        std::string sType (tmp);
        env->ReleaseStringUTFChars(type, tmp);

        return (jint) Variant::typeFromString (sType);
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_isConvertible
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isConvertible(JNIEnv *env, jclass, jint srcType, jint dstType);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isConvertible (JNIEnv *env, jclass, jint srcType, jint dstType)
{
    static const char method_name[] = "Variant::n_1isConvertible";
    
    try 
    {
        Variant::Type src = (Variant::Type) srcType;
        Variant::Type dst = (Variant::Type) dstType;
        if ((src >= Variant::Type::type_empty) && (src <= Variant::Type::type_vec4d_vector) && (dst >= Variant::Type::type_empty) && (dst <= Variant::Type::type_vec4d_vector))
        {
            return (jboolean)Variant::isConvertible(src, dst);
        }
        else
            VMF_EXCEPTION(OutOfRangeException, "Non-existent type.");
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_base64Encode
 * Signature: ([BI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1base64Encode(JNIEnv *env, jclass, jbyteArray buf, jint length);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1base64Encode (JNIEnv *env, jclass, jbyteArray buf, jint length)
{
    static const char method_name[] = "Variant::n_1base64Encode";
    try 
    {
        char* cArray = (char*) env->GetByteArrayElements (buf, 0);
        vmf_rawbuffer rb(cArray, (size_t)length);

        std::string str = Variant::base64encode(rb);

        env->ReleaseByteArrayElements (buf, (jbyte*)cArray, 0);
        return env->NewStringUTF(str.c_str());
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_base64Decode
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_intel_vmf_Variant_n_1base64Decode(JNIEnv *env, jclass, jstring src);


JNIEXPORT jbyteArray JNICALL Java_com_intel_vmf_Variant_n_1base64Decode (JNIEnv *env, jclass, jstring src)
{
    static const char method_name[] = "Variant::n_1base64Decode";
    try 
    {
        const char* tmp = env->GetStringUTFChars(src, NULL);
        std::string str (tmp);
        vmf_rawbuffer buf = Variant::base64decode (str);
        jsize size = (jsize)(buf.size());
        jbyteArray byteArray = env->NewByteArray (size);
        jbyte* cArray = env->GetByteArrayElements (byteArray, 0);
        
        for (std::size_t i = 0; i < buf.size(); i++)
        {
            cArray[i] = (jbyte)(buf[i]);
        }
        
        env->ReleaseByteArrayElements (byteArray, cArray, 0);
        env->ReleaseStringUTFChars(src, tmp);
        return byteArray;
    }
    catch(const std::exception &e)
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1delete";

    try
    {
        std::shared_ptr<Variant>* obj = (std::shared_ptr<Variant>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

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
