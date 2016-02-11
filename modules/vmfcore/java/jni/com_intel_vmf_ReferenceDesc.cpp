#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_ReferenceDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_ReferenceDesc_n_1ReferenceDesc__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_ReferenceDesc_n_1ReferenceDesc__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "ReferenceDesc::n_1ReferenceDesc__";

    try
    {
        std::shared_ptr<ReferenceDesc>* obj = new std::shared_ptr<ReferenceDesc>(new ReferenceDesc());
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
 * Class:     com_intel_vmf_ReferenceDesc
 * Method:    n_ReferenceDesc
 * Signature: (Ljava/lang/String;ZZ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_ReferenceDesc_n_1ReferenceDesc__Ljava_lang_String_2ZZ (JNIEnv *env, jclass, jstring name, jboolean isUnique, jboolean isCustom)
{
    static const char method_name[] = "ReferenceDesc::n_1ReferenceDesc__Ljava_lang_String_2ZZ";

    try
    {
        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        std::shared_ptr<ReferenceDesc>* obj = new std::shared_ptr<ReferenceDesc>(new ReferenceDesc(sName, (isUnique == JNI_TRUE) ? true : false, (isCustom == JNI_TRUE) ? true : false));
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

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::string str = (**obj).name;
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

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        jboolean isUnique = (jboolean)(**obj).isUnique;
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

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        jboolean isCustom = (jboolean)(**obj).isCustom;
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
        std::shared_ptr<ReferenceDesc>* obj = (std::shared_ptr<ReferenceDesc>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Reference description is null pointer.");

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
