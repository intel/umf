#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_FieldDesc.h"

using namespace vmf;

static void throwJavaException(JNIEnv *env, const std::exception *e, const char *method)
{
    std::string what = "unknown exception";
    jclass je = 0;

    if (e)
    {
        std::string exception_type = "std::exception";

        if (dynamic_cast<const Exception*>(e))
        {
            exception_type = "vmf::Exception";
            je = env->FindClass("com/intel/vmf/VmfException");
        }

        what = exception_type + ": " + e->what();
    }

    if (!je)
        je = env->FindClass("java/lang/Exception");

    env->ThrowNew(je, what.c_str());

    VMF_LOG_ERROR(what.c_str());
}

/*
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_FieldDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__ (JNIEnv *env, jclass)
{
    std::shared_ptr<FieldDesc>* p = new std::shared_ptr<FieldDesc>(new FieldDesc ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_FieldDesc
 * Signature: (Ljava/lang/String;JZ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__Ljava_lang_String_2JZ (JNIEnv *env, jclass, jstring name, jint type, jboolean isOptional)
{
    std::string sName (env->GetStringUTFChars (name, NULL));
    std::shared_ptr<FieldDesc>* p = new std::shared_ptr<FieldDesc>(new FieldDesc(sName, (Variant::Type) type, (isOptional == 1) ? true : false));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1equals (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldDesc::n_1equals";
    
    try 
    {
        std::shared_ptr<FieldDesc>* self = (std::shared_ptr<FieldDesc>*) selfAddr;
        std::shared_ptr<FieldDesc>* other = (std::shared_ptr<FieldDesc>*) otherAddr;
        
        if (*(*self) == *(*other))
            return JNI_TRUE;
        else
            return JNI_FALSE;
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
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldDesc_n_1getName (JNIEnv *env, jclass, jlong self)
{
    
    static const char method_name[] = "FieldDesc::n_1getName";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        std::string str = (*(*obj)).name;
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
JNIEXPORT jint JNICALL Java_com_intel_vmf_FieldDesc_n_1getType (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1getType";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        jint type = (jint)((*(*obj)).type);
        return type;
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
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldDesc_n_1isOptional (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1isOptional";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        jboolean isOptional = (jboolean)(*(*obj)).optional;
        return isOptional;
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
 * Method:    n_setName
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1setName (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "FieldDesc::n_1setName";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        std::string sName (env->GetStringUTFChars (name, NULL));
        (*(*obj)).name = sName;
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_setType
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1setType (JNIEnv *env, jclass, jlong self, jint type)
{
    static const char method_name[] = "FieldDesc::n_1setType";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        (*(*obj)).type = (Variant::Type) type;
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_setOptional
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1setOptional (JNIEnv *env, jclass, jlong self, jboolean isOptional)
{
    static const char method_name[] = "FieldDesc::n_1setOptional";
    
    try 
    {
        std::shared_ptr<FieldDesc>* obj = (std::shared_ptr<FieldDesc>*) self;
        (*(*obj)).optional = (isOptional == 1) ? true : false;
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
 * Class:     com_intel_vmf_FieldDesc
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldDesc_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldDesc::n_1delete";
    
    try 
    {
        std::shared_ptr<FieldDesc>* p = (std::shared_ptr<FieldDesc>*) self;
        delete p;
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
