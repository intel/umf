#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_MetadataSet
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1MetadataSet__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1MetadataSet__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "MetadataSet::n_1MetadataSet__";

    try
    {
        std::shared_ptr<MetadataSet>* obj = new std::shared_ptr<MetadataSet>(new MetadataSet());
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
* Class:     com_intel_vmf_MetadataSet
* Method:    n_getElement
* Signature: (JJ)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1getElement(JNIEnv *env, jclass, jlong self, jlong number);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1getElement (JNIEnv *env, jclass, jlong self, jlong number)
{
    static const char method_name[] = "MetadataSet::n_1getElement";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        size_t num = (size_t)number;

        if ((**obj).size() > num)
            return (jlong) new std::shared_ptr<Metadata> ((**obj)[num]);
        else
            VMF_EXCEPTION (OutOfRangeException, "Number is out of range.");
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
 * Class:     com_intel_vmf_MetadataSet
 * Method:    n_queryByFrameIndex
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByFrameIndex(JNIEnv *env, jclass, jlong self, jlong id);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByFrameIndex (JNIEnv *env, jclass, jlong self, jlong id)
{
    static const char method_name[] = "MetadataSet::n_1queryByFrameIndex";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByTime(JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByTime (JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime)
{
    static const char method_name[] = "MetadataSet::n_1queryByTime";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryBySchema(JNIEnv *env, jclass, jlong self, jstring schemaName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryBySchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "MetadataSet::n_1queryBySchema";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(schemaName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(schemaName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByName(JNIEnv *env, jclass, jlong self, jstring mdName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByName (JNIEnv *env, jclass, jlong self, jstring mdName)
{
    static const char method_name[] = "MetadataSet::n_1queryByName";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(mdName, tmp);

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndValue(JNIEnv *env, jclass, jlong self, jstring mdName, jlong fieldValueAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName, jlong fieldValueAddr)
{
    static const char method_name[] = "MetadataSet::n_1queryByNameAndValue";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<FieldValue>* value = (std::shared_ptr<FieldValue>*)fieldValueAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((value == NULL) || (*value == NULL) || (value->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(mdName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndFields(JNIEnv *env, jclass, jlong self, jstring mdName, jlongArray fieldValues);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByNameAndFields (JNIEnv *env, jclass, jlong self, jstring mdName, jlongArray fieldValues)
{
    static const char method_name[] = "MetadataSet::n_1queryByNameAndFields";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <FieldValue> values;

        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);

        jlong* cArray= env->GetLongArrayElements (fieldValues, 0);
        jsize len = env->GetArrayLength (fieldValues);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<FieldValue>* addr = (std::shared_ptr<FieldValue>*)cArray[i];
            values.push_back (**addr);
        }

        env->ReleaseLongArrayElements (fieldValues, cArray, 0);
        env->ReleaseStringUTFChars(mdName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2(JNIEnv *env, jclass, jlong self, jstring refName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring refName)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(refName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(refName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2J(JNIEnv *env, jclass, jlong self, jstring refName, jlong valueAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2J (JNIEnv *env, jclass, jlong self, jstring refName, jlong valueAddr)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2J";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<FieldValue>* value = (std::shared_ptr<FieldValue>*)valueAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((value == NULL) || (*value == NULL) || (value->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars (refName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(refName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2_3J(JNIEnv *env, jclass, jlong self, jstring refName, jlongArray fieldValues);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1queryByReference__JLjava_lang_String_2_3J (JNIEnv *env, jclass, jlong self, jstring refName, jlongArray fieldValues)
{
    static const char method_name[] = "MetadataSet::n_1queryByReference__JLjava_lang_String_2_3J";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(refName, NULL);
        std::string sName(tmp);

        jlong *body = env->GetLongArrayElements (fieldValues, 0);
        std::vector <FieldValue> values;
        jsize len = env->GetArrayLength (fieldValues);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr<FieldValue>* addr = (std::shared_ptr<FieldValue>*)body[i];
            values.push_back (**addr);
        }

        env->ReleaseLongArrayElements (fieldValues, body, 0);
        env->ReleaseStringUTFChars(refName, tmp);
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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1shift(JNIEnv *env, jclass, jlong self, jlong dstFrameIndex, jlong srcFrameIndex, jlong numOfFrames, jlong setFailureAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1shift (JNIEnv *env, jclass, jlong self, jlong dstFrameIndex, jlong srcFrameIndex, jlong numOfFrames, jlong setFailureAddr)
{
    static const char method_name[] = "MetadataSet::n_1shift";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::shared_ptr<MetadataSet>* setFailure = (std::shared_ptr<MetadataSet>*)setFailureAddr;

        if ((setFailure != NULL) && (*setFailure != NULL))
            return (jlong)(*obj)->shift((long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames, setFailure->get());
        else
            return (jlong)(*obj)->shift((long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames);
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
 * Method:    n_getSize
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1getSize(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSet_n_1getSize (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSet::n_1getSize";

    try 
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong) (*obj)->size();
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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSet::n_1delete";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata set is null pointer.");

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

//long n_clear(long nativeObj);
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1clear(JNIEnv *env, jclass, jlong self);

JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1clear(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSet::n_1clear";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if (obj == NULL || *obj == NULL)
            VMF_EXCEPTION(NullPointerException, "MetadataSet (this) is null pointer.");

        (*obj)->clear();
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

//long n_push_back(long nativeObj, long metadataObj);
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1push_1back(JNIEnv *env, jclass, jlong self, jlong metadataAddr);

JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1push_1back(JNIEnv *env, jclass, jlong self, jlong metadataAddr)
{
    static const char method_name[] = "MetadataSet::n_1push_1back";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;
        std::shared_ptr<Metadata>* md = (std::shared_ptr<Metadata>*)metadataAddr;

        if (obj == NULL || *obj == NULL)
            VMF_EXCEPTION(NullPointerException, "MetadataSet (this) is null pointer.");

        if (md == NULL || *md == NULL)
            VMF_EXCEPTION(NullPointerException, "Metadata to push_back is null pointer.");

        (*obj)->push_back(*md);
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

//long n_erase(long nativeObj, long idx);
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1erase(JNIEnv *env, jclass, jlong self, jlong idx);

JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSet_n_1erase(JNIEnv *env, jclass, jlong self, jlong idx)
{
    static const char method_name[] = "MetadataSet::n_1erase";

    try
    {
        std::shared_ptr<MetadataSet>* obj = (std::shared_ptr<MetadataSet>*)self;

        if (obj == NULL || *obj == NULL)
            VMF_EXCEPTION(NullPointerException, "MetadataSet (this) is null pointer.");

        if (idx < 0 || idx >= (jlong)(*obj)->size()) VMF_EXCEPTION(IncorrectParamException, "Item index is out of range: " + to_string(idx));
        else (*obj)->erase((*obj)->begin() + idx);
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

} //extern "C"
