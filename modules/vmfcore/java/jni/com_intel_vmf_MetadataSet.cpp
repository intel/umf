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
    return (jlong) new MetadataSet ();
}

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_MetadataSet
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1MetadataSet__J (JNIEnv *, jclass, jlong otherAddr)
{
    MetadataSet* other = (MetadataSet*) otherAddr;
    return (jlong) new MetadataSet (*other);
}

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1setTo (JNIEnv *, jclass, jlong selfAddr, jlong otherAddr)
{
    MetadataSet* self = (MetadataSet*) selfAddr;
    MetadataSet* other = (MetadataSet*) otherAddr;
    *self = *other;
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
        MetadataSet* obj = (MetadataSet*) self;
        size_t index = (size_t) id;
        return (jlong) new MetadataSet (obj->queryByFrameIndex (index));
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
        MetadataSet* obj = (MetadataSet*) self;
        return (jlong) new MetadataSet (obj->queryByTime((long long) startTime, (long long) endTime));
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
        MetadataSet* obj = (MetadataSet*) self;
        std::string sName (env->GetStringUTFChars (schemaName, NULL));
        return (jlong) new MetadataSet (obj->queryBySchema(sName));
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
        MetadataSet* obj = (MetadataSet*) self;
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        return (jlong) new MetadataSet (obj->queryByName(sName));
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
        MetadataSet* obj = (MetadataSet*) self;
        FieldValue* value = (FieldValue*) fieldValueAddr;
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        return (jlong) new MetadataSet (obj->queryByNameAndValue (sName, (*value)));
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
        MetadataSet* obj = (MetadataSet*) self;
        std::vector <FieldValue> values;
    
        std::string sName (env->GetStringUTFChars (mdName, NULL));
        jlong* array= env->GetLongArrayElements (fieldValues, 0);
        jsize len = env->GetArrayLength (fieldValues);
    
        for (int i = 0; i < len; i++)
        {
            FieldValue* addr = (FieldValue*) array[i];
            values.push_back (*addr);
        }
    
        env->ReleaseLongArrayElements (fieldValues, array, 0);
        return (jlong) new MetadataSet (obj->queryByNameAndFields (sName, values));
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
        MetadataSet* obj = (MetadataSet*) self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        return (jlong) new MetadataSet (obj->queryByReference (sName));
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
        MetadataSet* obj = (MetadataSet*) self;
        FieldValue* value = (FieldValue*) valueAddr;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        return (jlong) new MetadataSet (obj->queryByReference (sName, *value));
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
        MetadataSet* obj = (MetadataSet*) self;
        std::string sName (env->GetStringUTFChars (refName, NULL));
        jlong *body = env->GetLongArrayElements (fieldValues, 0);
        std::vector <FieldValue> values;
        jsize len = env->GetArrayLength (fieldValues);
    
        for (int i = 0; i < len; i++)
        {
            FieldValue* addr = (FieldValue*) body[i];
            values.push_back (*addr);
        }
    
        env->ReleaseLongArrayElements (fieldValues, body, 0);
        return (jlong) new MetadataSet (obj->queryByReference (sName, values));
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
        MetadataSet* obj = (MetadataSet*) self;
        MetadataSet* setFailure = (MetadataSet*) setFailureAddr;
        
        return (jlong) obj->shift((long long) dstFrameIndex, (long long) srcFrameIndex, (long long) numOfFrames, setFailure);
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
    delete (MetadataSet*) self;
}
