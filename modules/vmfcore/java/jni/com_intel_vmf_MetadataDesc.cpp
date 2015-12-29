#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_MetadataDesc.h"

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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__ (JNIEnv *, jclass)
{
    std::shared_ptr<MetadataDesc>* p = new std::shared_ptr<MetadataDesc>(new MetadataDesc());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2J (JNIEnv *env, jclass, jstring mdName, jlong type)
{
    std::string sName (env->GetStringUTFChars (mdName, NULL));
   
    std::shared_ptr<MetadataDesc>* p = new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, (Variant::Type) type));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J (JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs)
{
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    std::vector <FieldDesc> descs;
    jlong* cArray = env->GetLongArrayElements (fieldDescs, 0);
    jsize len = env->GetArrayLength (fieldDescs);
    
    for (int i = 0; i < len; i++)
    {
        std::shared_ptr<FieldDesc>* pFieldDesc = (std::shared_ptr<FieldDesc>*)cArray[i];
        descs.push_back (**pFieldDesc);
    }
    
    env->ReleaseLongArrayElements (fieldDescs, cArray, 0);
    return (jlong) new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, descs));
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;[J[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J_3J (JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs, jlongArray refDescs)
{
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    std::vector <FieldDesc> fdVec;
    std::vector<std::shared_ptr<ReferenceDesc>> rdVec;
    jlong* descArray = env->GetLongArrayElements (fieldDescs, 0);
    jlong* refArray = env->GetLongArrayElements (refDescs, 0);
    jsize lenDescs = env->GetArrayLength (fieldDescs);
    jsize lenRefs = env->GetArrayLength (refDescs);
    
    for (int i = 0; i < lenDescs; i++)
    {
        std::shared_ptr<FieldDesc>* pFieldDesc = (std::shared_ptr<FieldDesc>*) descArray[i];
        fdVec.push_back (**pFieldDesc);
    }
    
    for (int i = 0; i < lenRefs; i++)
    {
       std::shared_ptr<ReferenceDesc>* pRefDesc = (std::shared_ptr<ReferenceDesc>*)refArray[i];
       rdVec.push_back (*pRefDesc);
    }
    
    env->ReleaseLongArrayElements (fieldDescs, descArray, 0);
    env->ReleaseLongArrayElements (refDescs, refArray, 0);
    return (jlong) new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, fdVec, rdVec));
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getMetadataName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getMetadataName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getMetadataName";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::string str = (*obj)->getMetadataName ();
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getSchemaName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getSchemaName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getSchemaName";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::string str = (*obj)->getSchemaName ();
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getFields
 * Signature: (J)[Lcom/intel/vmf/FieldDesc;
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFields (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getFields";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::vector<FieldDesc> vec = (*obj)->getFields ();
        jlongArray nObjs = env->NewLongArray ((jsize)vec.size());
        jlong* body = env->GetLongArrayElements (nObjs, 0);
        
        for (std::size_t i = 0; i < vec.size(); i++)
        {
            body[i] = (jlong) new std::shared_ptr<FieldDesc>(new FieldDesc (vec[i].name, vec[i].type, vec[i].optional));
        }
        
        env->ReleaseLongArrayElements (nObjs, body, 0);
        return nObjs;
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getAllReferenceDescs
 * Signature: (J)[Lcom/intel/vmf/ReferenceDesc;
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getAllReferenceDescs (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getAllReferenceDescs";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::vector<std::shared_ptr<ReferenceDesc>> vec = (*obj)->getAllReferenceDescs ();
        jlongArray nObjs = env->NewLongArray ((jsize)vec.size());
        jlong* body = env->GetLongArrayElements (nObjs, 0);
        
        for (std::size_t i = 0; i < vec.size(); i++)
        {
            body[i] = (jlong) new std::shared_ptr<ReferenceDesc>(new ReferenceDesc(vec[i]->name, vec[i]->isUnique, vec[i]->isCustom));
        }
        
        env->ReleaseLongArrayElements (nObjs, body, 0);
        return nObjs;
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_declareCustomReference
 * Signature: (JLjava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1declareCustomReference (JNIEnv *env, jclass, jlong self, jstring refName, jboolean isUnique)
{
    static const char method_name[] = "MetadataDesc::n_1declareCustomReference";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        (*obj)->declareCustomReference (sName, (isUnique == 1) ? true : false);
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getReferenceDesc
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getReferenceDesc (JNIEnv *env, jclass, jlong self, jstring refName)
{
    static const char method_name[] = "MetadataDesc::n_1getReferenceDesc";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        std::shared_ptr<ReferenceDesc> sp = (*obj)->getReferenceDesc(sName);
        return (jlong) new std::shared_ptr<ReferenceDesc>(new ReferenceDesc (sp->name, sp->isUnique, sp->isCustom));
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getFieldDesc
 * Signature: (JLcom/intel/vmf/FieldDesc;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFieldDesc (JNIEnv *env, jclass, jlong self, jlong fieldDescAddr, jstring fieldName)
{
    static const char method_name[] = "MetadataDesc::n_1getFieldDesc";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
        std::shared_ptr<FieldDesc>* fieldDesc = (std::shared_ptr<FieldDesc>*)fieldDescAddr;
        std::string sName (env->GetStringUTFChars (fieldName, NULL));
        return (jboolean) (*obj)->getFieldDesc (**fieldDesc, sName);
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1delete";

    try
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;
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
