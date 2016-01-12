#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_FieldDesc.h"
#include "throwJavaException.hpp"

using namespace vmf;

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldDesc_n_1FieldDesc__Ljava_lang_String_2IZ(JNIEnv *env, jclass, jstring name, jint type, jboolean isOptional)
{
    std::string sName (env->GetStringUTFChars (name, NULL));
    std::shared_ptr<FieldDesc>* p = new std::shared_ptr<FieldDesc>(new FieldDesc(sName, (Variant::Type) type, (isOptional == JNI_TRUE) ? true : false));
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
        
        return ((*(*self) == *(*other)) ? JNI_TRUE : JNI_FALSE);
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
        return (jint)((*(*obj)).type);
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
        return (jboolean)(*(*obj)).optional;
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
        (*(*obj)).optional = ((isOptional == JNI_TRUE) ? true : false);
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
