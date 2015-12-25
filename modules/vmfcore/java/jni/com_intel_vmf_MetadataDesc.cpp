#include<string>
#include<vector>
#include "../com_intel_vmf_MetadataDesc.h"

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__ (JNIEnv *, jclass)
{
    return (jlong) new MetadataDesc ();
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2J (JNIEnv *env, jclass, jstring mdName, jlong type)
{
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    Variant::Type Type = (Variant::Type) type;
    return (jlong) new MetadataDesc (sName, Type);
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
        FieldDesc* pFieldDesc = (FieldDesc*) cArray[i];
        descs.push_back (*pFieldDesc);
    }
    
    env->ReleaseLongArrayElements (fieldDescs, cArray, 0);
    return (jlong) new MetadataDesc (sName, descs);    
}

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;[J[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J_3J (JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs, jlongArray refDesc)
{
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    std::vector <FieldDesc> fdVec;
    std::vector<std::shared_ptr<ReferenceDesc>> rdVec;
    jlong* descArray = env->GetLongArrayElements (fieldDescs, 0);
    jlong* refArray = env->GetLongArrayElements (refDesc, 0);
    jsize lenDescs = env->GetArrayLength (fieldDescs);
    jsize lenRefs = env->GetArrayLength (refDesc);
    
    for (int i = 0; i < lenDescs; i++)
    {
        FieldDesc* pFieldDesc = (FieldDesc*) descArray[i];
        fdVec.push_back (*pFieldDesc);
    }
    
    for (int i = 0; i < lenRefs; i++)
    {
       ReferenceDesc* pRefDesc = (ReferenceDesc*) refArray[i];
       std::shared_ptr<ReferenceDesc> spRefDesc = pRefDesc;
       rdVec.push_back (spRefDesc);
    }
    
    env->ReleaseLongArrayElements (fieldDescs, descArray, 0);
    env->ReleaseLongArrayElements (refDesc, refArray, 0);
    return (jlong) new MetadataDesc (sName, fdVec, rdVec);  
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
        MetadataDesc* obj = (MetadataDesc*) self;
        std::string str = obj->getMetadataName ();
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
        MetadataDesc* obj = (MetadataDesc*) self;
        std::string str = obj->getSchemaName ();
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
        MetadataDesc* obj = (MetadataDesc*) self;
        std::vector<FieldDesc> vec = obj->getFields ();
        jlongArray nObjs = env->NewLongArray (vec.size());
        jlong* body = env->GetLongArrayElements (nObjs, 0);
        
        for (int i = 0; i < vec.size(); i++)
        {
            body[i] = (jlong) new FieldDesc (vec[i].name, vec[i].type, vec[i].optional);
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
        MetadataDesc* obj = (MetadataDesc*) self;
        std::vector<std::shared_ptr<ReferenceDesc>> vec = obj->getAllReferenceDescs ();
        jlongArray nObjs = env->NewLongArray (vec.size());
        jlong* body = env->GetLongArrayElements (nObjs, 0);
        
        for (int i = 0; i < vec.size(); i++)
        {
            body[i] = (jlong) vec[i].get();
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
        MetadataDesc* obj = (MetadataDesc*) self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        obj->declareCustomReference (sName, (bool) isUnique);
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
 * Method:    n_getReferenceDesc
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getReferenceDesc (JNIEnv *env, jclass, jlong self, jstring refName)
{
    static const char method_name[] = "MetadataDesc::n_1getReferenceDesc";
    
    try 
    {
        MetadataDesc* obj = (MetadataDesc*) self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        return (jlong) (obj->getReferenceDesc (sName)).get();
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
        MetadataDesc* obj = (MetadataDesc*) self;
        FieldDesc* fieldDesc = (FieldDesc*) fieldDescAddr;
        std::string sName (env->GetStringUTFChars (fieldName, NULL));
        return (jboolean) obj->getFieldDesc (*fieldDesc, sName);
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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1delete (JNIEnv *, jclass, jlong self)
{
    delete (MetadataDesc*) self;
}
