#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "FieldValue::n_1FieldValue__";

    try
    {
        std::shared_ptr<FieldValue>* obj = new std::shared_ptr<FieldValue>(new FieldValue());
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: (J)J
 */
/*JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__J(JNIEnv *env, jclass, jlong otherAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__J (JNIEnv *env, jclass, jlong otherAddr)
{
    static const char method_name[] = "FieldValue::n_1FieldValue__J";

    try
    {
        std::shared_ptr<FieldValue>* other = (std::shared_ptr<FieldValue>*) otherAddr;
        std::shared_ptr<FieldValue>* obj = new std::shared_ptr<FieldValue>(new FieldValue(**other));
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
*/

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__Ljava_lang_String_2J(JNIEnv *env, jclass, jstring name, jlong variantAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__Ljava_lang_String_2J (JNIEnv *env, jclass, jstring name, jlong variantAddr)
{
    static const char method_name[] = "FieldValue::n_1FieldValue__Ljava_lang_String_2J";

    try
    {
        std::shared_ptr<Variant>* variant = (std::shared_ptr<Variant>*) variantAddr;
        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);

        std::shared_ptr<FieldValue>* obj = new std::shared_ptr<FieldValue>(new FieldValue(sName, (**variant)));

        env->ReleaseStringUTFChars(name, tmp);

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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldValue_n_1getName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldValue_n_1getName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1getName";
    
    try 
    {
        std::shared_ptr<FieldValue>* obj = (std::shared_ptr<FieldValue>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const std::string str = (*obj)->getName ();
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1setTo(JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1setTo (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldValue::n_1setTo";
    
    try 
    {
        std::shared_ptr<FieldValue>* self = (std::shared_ptr<FieldValue>*) selfAddr;
        std::shared_ptr<FieldValue>* other = (std::shared_ptr<FieldValue>*) otherAddr;

        if ((self == NULL) || (self->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Field value is null pointer.");

        if ((other == NULL) || (other->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Other field value is null pointer.");

        (**self) = (**other);
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldValue_n_1equals(JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldValue_n_1equals (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldValue::n_1equals";
    
    try 
    {
        std::shared_ptr<FieldValue>* self = (std::shared_ptr<FieldValue>*) selfAddr;
        std::shared_ptr<FieldValue>* other = (std::shared_ptr<FieldValue>*) otherAddr;

        if ((self == NULL) || (self->get() == NULL))
            return JNI_FALSE;

        if ((other == NULL) || (other->get() == NULL))
            return JNI_FALSE;
        
        return (**self == **other) ? JNI_TRUE : JNI_FALSE;     
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
* Class:     com_intel_vmf_FieldValue
* Method:    n_getType
* Signature: (J)I
*/
JNIEXPORT jint JNICALL Java_com_intel_vmf_FieldValue_n_1getType(JNIEnv *env, jclass, jlong self);


JNIEXPORT jint JNICALL Java_com_intel_vmf_FieldValue_n_1getType (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1getType";

    try
    {
        std::shared_ptr <FieldValue>* obj = (std::shared_ptr <FieldValue>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jint)(*obj)->getType();
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
* Class:     com_intel_vmf_FieldValue
* Method:    n_isEmpty
* Signature: (J)Z
*/
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldValue_n_1isEmpty(JNIEnv *env, jclass, jlong self);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldValue_n_1isEmpty (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1isEmpty";

    try
    {
        std::shared_ptr <FieldValue>* obj = (std::shared_ptr <FieldValue>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean)(*obj)->isEmpty();
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
* Class:     com_intel_vmf_FieldValue
* Method:    n_getTypeName
* Signature: (J)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldValue_n_1getTypeName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldValue_n_1getTypeName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1getTypeName";

    try
    {
        std::shared_ptr <FieldValue>* obj = (std::shared_ptr <FieldValue>*)self;

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
* Class:     com_intel_vmf_FieldValue
* Method:    n_convertTo
* Signature: (JI)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1convertTo(JNIEnv *env, jclass, jlong self, jint type);


JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1convertTo (JNIEnv *env, jclass, jlong self, jint type)
{
    static const char method_name[] = "FieldValue::n_1convertTo";

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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1delete";
    
    try 
    {
        std::shared_ptr<FieldValue>* obj = (std::shared_ptr<FieldValue>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Field description is null pointer.");

        delete obj;
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


}
