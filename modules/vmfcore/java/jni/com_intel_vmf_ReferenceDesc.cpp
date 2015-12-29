#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_ReferenceDesc.h"

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
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_ReferenceDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_ReferenceDesc_n_1ReferenceDesc__ (JNIEnv *env, jclass)
{
    std::shared_ptr<ReferenceDesc>* p = new std::shared_ptr<ReferenceDesc>(new ReferenceDesc ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_ReferenceDesc
 * Signature: (Ljava/lang/String;ZZ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_ReferenceDesc_n_1ReferenceDesc__Ljava_lang_String_2ZZ (JNIEnv *env, jclass, jstring name, jboolean isUnique, jboolean isCustom)
{
    std::string sName (env->GetStringUTFChars (name, NULL));
    std::shared_ptr<ReferenceDesc>* p = new std::shared_ptr<ReferenceDesc>(new ReferenceDesc(sName, (isUnique == 1) ? true : false, (isCustom == 1) ? true : false));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_ReferenceDesc_n_1getName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "ReferenceDesc::n_1getName";
    
    try 
    {
        std::shared_ptr<ReferenceDesc>* obj = (std::shared_ptr<ReferenceDesc>*) self;
        std::string str = (*(*obj)).name;
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
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_isUnique
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_ReferenceDesc_n_1isUnique (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "ReferenceDesc::n_1isUnique";
    
    try 
    {
        std::shared_ptr<ReferenceDesc>* obj = (std::shared_ptr<ReferenceDesc>*) self;
        jboolean isUnique = (jboolean)(*(*obj)).isUnique;
        return isUnique;
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
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_isCustom
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_ReferenceDesc_n_1isCustom (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "ReferenceDesc::n_1isCustom";
    
    try 
    {
        std::shared_ptr<ReferenceDesc>* obj = (std::shared_ptr<ReferenceDesc>*) self;
        jboolean isCustom = (jboolean)(*(*obj)).isCustom;
        return isCustom;
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
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_ReferenceDesc_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "ReferenceDesc::n_1delete";
    
    try 
    {
        std::shared_ptr<ReferenceDesc>* p = (std::shared_ptr<ReferenceDesc>*) self;
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
