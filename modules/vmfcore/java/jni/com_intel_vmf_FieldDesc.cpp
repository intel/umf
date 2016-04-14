#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_FieldDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__ (JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "FieldDesc::n_1FieldDesc__";

    try
    {
        std::shared_ptr<FieldDesc>* obj = new std::shared_ptr<FieldDesc>(new FieldDesc());
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_FieldDesc
 * Signature: (Ljava/lang/String;JZ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__Ljava_lang_String_2IZ (JNIEnv *env, jclass, jstring name, jint type, jboolean isOptional);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__Ljava_lang_String_2IZ (JNIEnv *env, jclass, jstring name, jint type, jboolean isOptional)
{
    static const char method_name[] = "FieldDesc::n_1FieldDesc__Ljava_lang_String_2IZ";

    try
    {
        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(name, tmp);

        std::shared_ptr<FieldDesc>* obj = new std::shared_ptr<FieldDesc>(new FieldDesc(sName, (Variant::Type) type, (isOptional == JNI_TRUE) ? true : false));

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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1equals (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1equals (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldDesc::n_1equals";
    
    try 
    {
        std::shared_ptr<FieldDesc>* self = (std::shared_ptr<FieldDesc>*) selfAddr;
        std::shared_ptr<FieldDesc>* other = (std::shared_ptr<FieldDesc>*) otherAddr;
        
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldDesc_n_1getName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldDesc_n_1getName (JNIEnv *env, jclass, jlong self)
{
    
    static const char method_name[] = "FieldDesc::n_1getName";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;

        if (obj == NULL || *obj == NULL)
            return 0;

        std::string str = (**obj).name;
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_FieldDesc_n_1getType(JNIEnv *env, jclass, jlong self);


JNIEXPORT jint JNICALL Java_com_intel_vmf_FieldDesc_n_1getType (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1getType";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jint)((**obj).type);
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_isOptional
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1isOptional(JNIEnv *env, jclass, jlong self);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1isOptional (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1isOptional";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean)(**obj).optional;
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1delete";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;

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

