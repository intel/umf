#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_MetadataSet.h"

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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_MetadataSet
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1MetadataSet__ (JNIEnv *env, jclass)
{
    std::shared_ptr<MetadataSet>* p = new std::shared_ptr<MetadataSet>(new MetadataSet());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_MetadataSet
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1MetadataSet__J (JNIEnv *, jclass, jlong otherAddr)
{
    std::shared_ptr<MetadataSet>* other = (std::shared_ptr<MetadataSet>*)otherAddr;
    return (jlong) new std::shared_ptr<MetadataSet> (new MetadataSet (**other));
}

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1setTo (JNIEnv *, jclass, jlong selfAddr, jlong otherAddr)
{
    std::shared_ptr<MetadataSet>* self = (std::shared_ptr<MetadataSet>*)selfAddr;
    std::shared_ptr<MetadataSet>* other = (std::shared_ptr<MetadataSet>*)otherAddr;
    MetadataSet tmp = (*(*other));
    (*(*self)) = tmp;
}

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByFrameIndex
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByFrameIndex (JNIEnv *env, jclass, jlong self, jlong id)
{
    static const char method_name[] = "MetadataSet::n_1queryByFrameIndex";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        size_t index = (size_t) id;
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByFrameIndex(index)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByTime
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByTime (JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime)
{
    static const char method_name[] = "MetadataSet::n_1queryByTime";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByTime((long long)startTime, (long long)endTime)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryBySchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryBySchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "MetadataSet::n_1queryBySchema";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::string sName (env->GetStringUTFChars (schemaName, NULL));
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryBySchema(sName)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByName (JNIEnv *env, jclass, jlong self, jstring mdName)
{
    static const char method_name[] = "MetadataSet::n_1queryByName";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByName(sName)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByNameAndValue
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName, jlong fieldValueAddr)
{
    static const char method_name[] = "MetadataSet::n_1queryByNameAndValue";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<FieldValue>* value = (std::shared_ptr<FieldValue>*)fieldValueAddr;
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByNameAndValue(sName, (**value))));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByNameAndFields
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndFields (JNIEnv *env, jclass, jlong self, jstring mdName, jlongArray fieldValues)
{
    static const char method_name[] = "MetadataSet::n_1queryByNameAndFields";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::vector <FieldValue> values;
    
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        jlong* cArray= env->GetLongArrayElements (fieldValues, 0);
        jsize len = env->GetArrayLength (fieldValues);
    
        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<FieldValue>* addr = (std::shared_ptr<FieldValue>*)cArray[i];
            values.push_back (**addr);
        }
    
        env->ReleaseLongArrayElements (fieldValues, cArray, 0);
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet ((*obj)->queryByNameAndFields(sName, values)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring refName)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByReference(sName)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2J (JNIEnv *env, jclass, jlong self, jstring refName, jlong valueAddr)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2J";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<FieldValue>* value = (std::shared_ptr<FieldValue>*)valueAddr;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByReference(sName, **value)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2_3J (JNIEnv *env, jclass, jlong self, jstring refName, jlongArray fieldValues)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2_3J";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        jlong *body = env->GetLongArrayElements (fieldValues, 0);
        std::vector <FieldValue> values;
        jsize len = env->GetArrayLength (fieldValues);
    
        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<FieldValue>* addr = (std::shared_ptr<FieldValue>*)body[i];
            values.push_back (**addr);
        }
    
        env->ReleaseLongArrayElements (fieldValues, body, 0);
        return (jlong) new std::shared_ptr<MetadataSet>(new MetadataSet((*obj)->queryByReference(sName, values)));
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_shift
 * Signature: (JJJJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1shift (JNIEnv *env, jclass, jlong self, jlong dstFrameIndex, jlong srcFrameIndex, jlong numOfFrames, jlong setFailureAddr)
{
    static const char method_name[] = "MetadataSet::n_1shift";
    
    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<MetadataSet>* setFailure = (std::shared_ptr<MetadataSet>*)setFailureAddr;
        
        return (jlong) (*obj)->shift((long long) dstFrameIndex, (long long) srcFrameIndex, (long long) numOfFrames, (*setFailure).get());
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSet::n_1delete";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
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
