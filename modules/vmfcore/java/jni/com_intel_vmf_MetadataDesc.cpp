#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "MetadataDesc::n_1MetadataDesc__";

    try
    {
        std::shared_ptr<MetadataDesc>* obj = new std::shared_ptr<MetadataDesc>(new MetadataDesc());
        return (jlong) obj;
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2J(JNIEnv *env, jclass, jstring mdName, jlong type);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2J (JNIEnv *env, jclass, jstring mdName, jlong type)
{
    static const char method_name[] = "MetadataDesc::n_1MetadataDesc__Ljava_lang_String_2J";

    try
    {
        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);

        std::shared_ptr<MetadataDesc>* obj = new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, (Variant::Type) type));

        env->ReleaseStringUTFChars(mdName, tmp);
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J(JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J (JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs)
{
    static const char method_name[] = "MetadataDesc::n_1MetadataDesc__Ljava_lang_String_2_3J";

    try
    {
        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        std::vector <FieldDesc> descs;
        jlong* cArray = env->GetLongArrayElements(fieldDescs, 0);
        jsize len = env->GetArrayLength(fieldDescs);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<FieldDesc>* pFieldDesc = (std::shared_ptr<FieldDesc>*)cArray[i];
            descs.push_back(**pFieldDesc);
        }

        env->ReleaseLongArrayElements(fieldDescs, cArray, 0);
        env->ReleaseStringUTFChars(mdName, tmp);
        return (jlong) new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, descs));
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_MetadataDesc
 * Signature: (Ljava/lang/String;[J[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J_3J(JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs, jlongArray refDescs);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1MetadataDesc__Ljava_lang_String_2_3J_3J (JNIEnv *env, jclass, jstring mdName, jlongArray fieldDescs, jlongArray refDescs)
{
    static const char method_name[] = "MetadataDesc::n_1MetadataDesc__Ljava_lang_String_2_3J_3J";

    try
    {
        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        std::vector <FieldDesc> fdVec;
        std::vector<std::shared_ptr<ReferenceDesc>> rdVec;
        jlong* descArray = env->GetLongArrayElements(fieldDescs, 0);
        jlong* refArray = env->GetLongArrayElements(refDescs, 0);
        jsize lenDescs = env->GetArrayLength(fieldDescs);
        jsize lenRefs = env->GetArrayLength(refDescs);

        for (int i = 0; i < lenDescs; i++)
        {
            std::shared_ptr<FieldDesc>* pFieldDesc = (std::shared_ptr<FieldDesc>*) descArray[i];
            fdVec.push_back(**pFieldDesc);
        }

        for (int i = 0; i < lenRefs; i++)
        {
            std::shared_ptr<ReferenceDesc>* pRefDesc = (std::shared_ptr<ReferenceDesc>*)refArray[i];
            rdVec.push_back(*pRefDesc);
        }

        env->ReleaseLongArrayElements(fieldDescs, descArray, 0);
        env->ReleaseLongArrayElements(refDescs, refArray, 0);
        env->ReleaseStringUTFChars(mdName, tmp);
        return (jlong) new std::shared_ptr<MetadataDesc>(new MetadataDesc(sName, fdVec, rdVec));
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
 * Class:     com_intel_vmf_MetadataDesc
 * Method:    n_getMetadataName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getMetadataName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getMetadataName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getMetadataName";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getSchemaName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataDesc_n_1getSchemaName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getSchemaName";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFields(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFields (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getFields";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getAllReferenceDescs(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataDesc_n_1getAllReferenceDescs (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1getAllReferenceDescs";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1declareCustomReference(JNIEnv *env, jclass, jlong self, jstring refName, jboolean isUnique);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1declareCustomReference (JNIEnv *env, jclass, jlong self, jstring refName, jboolean isUnique)
{
    static const char method_name[] = "MetadataDesc::n_1declareCustomReference";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata description is null pointer.");

        const char* tmp = env->GetStringUTFChars(refName, NULL);
        std::string sName(tmp);
        (*obj)->declareCustomReference (sName, (isUnique == JNI_TRUE) ? true : false);
        env->ReleaseStringUTFChars(refName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getReferenceDesc(JNIEnv *env, jclass, jlong self, jstring refName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getReferenceDesc (JNIEnv *env, jclass, jlong self, jstring refName)
{
    static const char method_name[] = "MetadataDesc::n_1getReferenceDesc";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(refName, NULL);
        std::string sName(tmp);
        std::shared_ptr<ReferenceDesc> refDesc = (*obj)->getReferenceDesc(sName);

        if (refDesc == NULL)
            return 0;

        env->ReleaseStringUTFChars(refName, tmp);
        return (jlong) new std::shared_ptr<ReferenceDesc>(new ReferenceDesc(refDesc->name, refDesc->isUnique, refDesc->isCustom));
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
* Signature: (JLjava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFieldDesc(JNIEnv *env, jclass, jlong self, jstring fieldName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataDesc_n_1getFieldDesc (JNIEnv *env, jclass, jlong self, jstring fieldName)
{
    static const char method_name[] = "MetadataDesc::n_1getFieldDesc";
    
    try 
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;
        
        FieldDesc fieldDesc;
    
        const char* tmp = env->GetStringUTFChars(fieldName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars (fieldName, tmp);

        bool result = (*obj)->getFieldDesc (fieldDesc, sName);
            
        if (result == JNI_FALSE)
            return 0;

       return (jlong) new std::shared_ptr<FieldDesc>(new FieldDesc(fieldDesc.name, fieldDesc.type, fieldDesc.optional));
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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataDesc_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataDesc::n_1delete";

    try
    {
        std::shared_ptr<MetadataDesc>* obj = (std::shared_ptr<MetadataDesc>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata description is null pointer.");

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
