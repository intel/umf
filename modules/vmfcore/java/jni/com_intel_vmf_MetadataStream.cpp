#include<string>
#include<vector>
#include <memory>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"
#include <iostream>

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_MetadataStream
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1MetadataStream(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1MetadataStream (JNIEnv *env, jclass)
{
    static const char method_name[] = "MetadataStream::n_1MetadataStream";

    try
    {
        std::shared_ptr <MetadataStream>* obj = new std::shared_ptr <MetadataStream>(new MetadataStream ());
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_open
 * Signature: (JLjava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1open(JNIEnv *env, jclass, jlong self, jstring filePath, jint mode);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1open (JNIEnv *env, jclass, jlong self, jstring filePath, jint mode)
{
    static const char method_name[] = "MetadataStream::n_1open";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        const char* tmp = env->GetStringUTFChars(filePath, NULL);
        std::string sPath(tmp);
        env->ReleaseStringUTFChars (filePath, tmp);

        return (jboolean) (*obj)->open (sPath, (MetadataStream::OpenMode) mode);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_reopen
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1reopen(JNIEnv *env, jclass, jlong self, jint mode);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1reopen (JNIEnv *env, jclass, jlong self, jint mode)
{
    static const char method_name[] = "MetadataStream::n_1reopen";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean) (*obj)->reopen (((MetadataStream::OpenMode)mode));
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_clear
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1clear(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1clear (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1clear";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

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


/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_load
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2(JNIEnv *env, jclass, jlong self, jstring schemaName);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "MetadataStream::n_1load__JLjava_lang_String_2";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        const char* tmp = env->GetStringUTFChars(schemaName, NULL);
        std::string sName (tmp);
        env->ReleaseStringUTFChars(schemaName, tmp);

        return (jboolean) (*obj)->load (sName);
    }
    catch (const std::exception &e)
    {
        throwJavaException (env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException (env, 0, method_name);
    }

    return 0;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_load
 * Signature: (JLjava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2Ljava_lang_String_2(JNIEnv *env, jclass, jlong self, jstring schemaName, jstring mdName);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName, jstring mdName)
{
    static const char method_name[] = "MetadataStream::n_1load__JLjava_lang_String_2Ljava_lang_String_2";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        const char* schema = env->GetStringUTFChars(schemaName, NULL);
        const char* metadata = env->GetStringUTFChars(mdName, NULL);
        std::string sName(schema);
        std::string mName(metadata);
        env->ReleaseStringUTFChars(schemaName, schema);
        env->ReleaseStringUTFChars(mdName, metadata);
        return (jboolean)(*obj)->load (sName, mName);
    }
    catch (const std::exception &e)
    {
        throwJavaException (env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException (env, 0, method_name);
    }

    return 0;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_save
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1save(JNIEnv *env, jclass, jlong self);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1save (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1save";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean)(*obj)->save();
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_saveTo
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1saveTo(JNIEnv *env, jclass, jlong self, jstring filePath);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1saveTo (JNIEnv *env, jclass, jlong self, jstring filePath)
{
    static const char method_name[] = "MetadataStream::n_1saveTo";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        const char* tmp = env->GetStringUTFChars(filePath, NULL);
        std::string sPath(tmp);
        env->ReleaseStringUTFChars(filePath, tmp);

        return (jboolean)(*obj)->saveTo(sPath);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1close(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1close (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1close";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        (*obj)->close();
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getById
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getById(JNIEnv *env, jclass, jlong self, jlong id);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getById (JNIEnv *env, jclass, jlong self, jlong id)
{
    static const char method_name[] = "MetadataStream::n_1getById";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        IdType Id = (IdType)id;
        std::shared_ptr <Metadata> spMd = (*obj)->getById (Id);

        if (spMd == NULL)
            return 0;
        
        std::shared_ptr <Metadata>* retVal = new std::shared_ptr <Metadata>(spMd);
        return (jlong)retVal;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_add
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1add(JNIEnv *env, jclass, jlong self, jlong mdAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1add (JNIEnv *env, jclass, jlong self, jlong mdAddr)
{
    static const char method_name[] = "MetadataStream::n_1add";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
        std::shared_ptr <Metadata>* md = (std::shared_ptr <Metadata>*) mdAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((md == NULL) || (md->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        return (jlong)(*obj)->add(*md);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addInternal
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1addInternal(JNIEnv *env, jclass, jlong self, jlong mdIntAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1addInternal (JNIEnv *env, jclass, jlong self, jlong mdIntAddr)
{
    static const char method_name[] = "MetadataStream::n_1addInternal";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        MetadataInternal* mdInt = (MetadataInternal*) mdIntAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if (mdInt == NULL)
            VMF_EXCEPTION(NullPointerException, "Metadata internal is null pointer.");

        return (jlong)(*obj)->add(*mdInt);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeMdById
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1removeMdById(JNIEnv *env, jclass, jlong self, jlong id);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1removeMdById (JNIEnv *env, jclass, jlong self, jlong id)
{
    static const char method_name[] = "MetadataStream::n_1removeMdById";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean)(*obj)->remove((IdType)id);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSet
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSet(JNIEnv *env, jclass, jlong self, jlong mdSetAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSet (JNIEnv *env, jclass, jlong self, jlong mdSetAddr)
{
    static const char method_name[] = "MetadataStream::n_1removeSet";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <MetadataSet>* mdSet = (std::shared_ptr <MetadataSet>*) mdSetAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((mdSet == NULL) || (*mdSet == NULL) || (mdSet->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata set is null pointer.");

        (*obj)->remove((**mdSet));
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSchema(JNIEnv *env, jclass, jlong self, jlong schemaAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    static const char method_name[] = "MetadataStream::n_1removeSchema";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <MetadataSchema>* schema = (std::shared_ptr <MetadataSchema>*) schemaAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((schema == NULL) || (*schema == NULL) || (schema->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Schema is null pointer.");

        (*obj)->remove (*schema);
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeAllMd
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeAllMd(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeAllMd (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1removeAllMd";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        (*obj)->remove();
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addSchema(JNIEnv *env, jclass, jlong self, jlong schemaAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    static const char method_name[] = "MetadataStream::n_1addSchema";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <MetadataSchema>* schema = (std::shared_ptr <MetadataSchema>*) schemaAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((schema == NULL) || (*schema == NULL) || (schema->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Schema is null pointer.");

        (*obj)->addSchema (*schema);
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getSchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getSchema(JNIEnv *env, jclass, jlong self, jstring schemaName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getSchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "MetadataStream::n_1getSchema";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* name = env->GetStringUTFChars(schemaName, NULL);
        std::string sName(name);
        env->ReleaseStringUTFChars(schemaName, name);

        std::shared_ptr <MetadataSchema> schema = (*obj)->getSchema(sName);

        if (schema == NULL)
            return 0;

        std::shared_ptr <MetadataSchema>* mdSchema = new std::shared_ptr <MetadataSchema>(schema);
        return (jlong) mdSchema;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllSchemaNames
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllSchemaNames(JNIEnv *env, jclass, jlong self);


JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllSchemaNames (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getAllSchemaNames";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <std::string> vecNames = (*obj)->getAllSchemaNames();

        jclass stringClass = env->FindClass("java/lang/String");
        jobjectArray objArray = env->NewObjectArray((jsize)vecNames.size(), stringClass, NULL);

        int counter = 0;
        for (auto it = vecNames.begin(); it != vecNames.end(); it++, counter++)
        {
            jstring str = env->NewStringUTF(it->c_str());
            env->SetObjectArrayElement(objArray, counter, str);
        }

        return objArray;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAll
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getAll(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getAll (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getAll";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::shared_ptr <MetadataSet>* mdSet = new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->getAll()));
        return (jlong)mdSet;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_importSet
 * Signature: (JJJJJJJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1importSet (JNIEnv *env, jclass, jlong self, jlong srcStreamAddr,
                                                                          jlong srcSetAddr, jlong dstFrameIndex, jlong srcFrameIndex, jlong numOfFrames, jlong setFailureAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1importSet (JNIEnv *env, jclass, jlong self, jlong srcStreamAddr,
                                                                           jlong srcSetAddr, jlong dstFrameIndex, jlong srcFrameIndex,
                                                                           jlong numOfFrames, jlong setFailureAddr)
{
    static const char method_name[] = "MetadataStream::n_1importSet";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <MetadataStream>* srcStream = (std::shared_ptr <MetadataStream>*) srcStreamAddr;
        std::shared_ptr <MetadataSet>* srcSet = (std::shared_ptr <MetadataSet>*) srcSetAddr;

        if ((obj == NULL) || (obj->get() == NULL) || (srcStream == NULL) || (srcStream->get() == NULL))
            return JNI_FALSE;

        if ((srcSet == NULL) || (srcSet->get() == NULL))
            return JNI_FALSE;

        std::shared_ptr <MetadataSet>* setFailure = (std::shared_ptr <MetadataSet>*) setFailureAddr;

        if ((setFailure != NULL) && (*setFailure != NULL))
            return (jboolean)(*obj)->import((**srcStream), (**srcSet), (long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames, setFailure->get());
        else
            return (jboolean)(*obj)->import((**srcStream), (**srcSet), (long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames);
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_serialize
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1serialize(JNIEnv *env, jclass, jlong self, jlong IWriterAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1serialize (JNIEnv *env, jclass, jlong self, jlong IWriterAddr)
{
    static const char method_name[] = "MetadataStream::n_1serialize";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <IWriter>* writer = (std::shared_ptr <IWriter>*)IWriterAddr;

        if ((obj == NULL) || (obj->get() == NULL))
           return 0;

        if ((writer == NULL) || (*writer == NULL) || (writer->get() == NULL))
            return 0;

        jstring str = env->NewStringUTF(""/*((*obj)->serialize((**writer))).c_str()*/);
        return str;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_deserialize
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1deserialize(JNIEnv *env, jclass, jlong self, jstring text, jlong IReaderAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1deserialize (JNIEnv *env, jclass, jlong self, jstring text, jlong IReaderAddr)
{
    static const char method_name[] = "MetadataStream::n_1deserialize";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
        std::shared_ptr <IReader>* reader = (std::shared_ptr <IReader>*) IReaderAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((reader == NULL) || (reader->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Reader is null pointer.");

        const char* tmp = env->GetStringUTFChars(text, NULL);
        std::string sText(tmp);
        env->ReleaseStringUTFChars(text, tmp);

        //(*obj)->deserialize(sText, (**reader));
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_computeChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1computeChecksum__J";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        jstring str = env->NewStringUTF(((*obj)->computeChecksum()).c_str());
        return str;
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_computeChecksum
* Signature: (J[J)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J_3J (JNIEnv *env, jclass, jlong self, jlongArray options);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J_3J(JNIEnv *env, jclass, jlong self, jlongArray options)
{
    static const char method_name[] = "MetadataStream::n_1computeChecksum__JJJ";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        jlong* cArray = env->GetLongArrayElements(options, 0);
        long long packetSize;
        long long packetOffset;

        std::string result = (*obj)->computeChecksum (packetSize, packetOffset);
        cArray[0] = (jlong)packetSize;
        cArray[1] = (jlong)packetOffset;

        env->ReleaseLongArrayElements(options, cArray, 0);

        return env->NewStringUTF(result.c_str());
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1getChecksum(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1getChecksum (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getChecksum";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return env->NewStringUTF(((*obj)->getChecksum()).c_str());
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_setChecksum
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1setChecksum(JNIEnv *env, jclass, jlong self, jstring checksum);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1setChecksum (JNIEnv *env, jclass, jlong self, jstring checksum)
{
    static const char method_name[] = "MetadataStream::n_1setChecksum";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        const char* tmp = env->GetStringUTFChars(checksum, NULL);
        std::string str(tmp);
        (*obj)->setChecksum(str);
        env->ReleaseStringUTFChars(checksum, tmp);
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addVideoSegment
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addVideoSegment(JNIEnv *env, jclass, jlong self, jlong segmentAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addVideoSegment (JNIEnv *env, jclass, jlong self, jlong segmentAddr)
{
    static const char method_name[] = "MetadataStream::n_1addVideoSegment";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <MetadataStream::VideoSegment>* segment = (std::shared_ptr <MetadataStream::VideoSegment>*) segmentAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        if ((segment == NULL) || (segment->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

        (*obj)->addVideoSegment(*segment);
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllVideoSegment
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllVideoSegments(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllVideoSegments (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getAllVideoSegments";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> segments;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        segments = (*obj)->getAllVideoSegments();

        jlongArray nObjs = env->NewLongArray((jsize)segments.size());
        jlong* body = env->GetLongArrayElements(nObjs, 0);

        int counter = 0;
        for (auto it = segments.begin(); it != segments.end(); it++, counter++)
            body[counter] = (jlong) new std::shared_ptr<MetadataStream::VideoSegment>(*it);

        env->ReleaseLongArrayElements(nObjs, body, 0);

        return nObjs;
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_convertTimestampToFrameIndex
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertTimestampToFrameIndex(JNIEnv *env, jclass, jlong self, jlong timestamp, jlong duration);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertTimestampToFrameIndex (JNIEnv *env, jclass, jlong self, jlong timestamp, jlong duration)
{
    static const char method_name[] = "MetadataStream::n_1convertTimestampToFrameIndex";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long long frameIndex = 0, numOfFrames = 0;

        (*obj)->convertTimestampToFrameIndex((long long)timestamp, (long long)duration, frameIndex, numOfFrames);
        return (jlong)frameIndex;
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_convertDurationToNumOfFrames
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertDurationToNumOfFrames(JNIEnv *env, jclass, jlong self, jlong timestamp, jlong duration);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertDurationToNumOfFrames (JNIEnv *env, jclass, jlong self, jlong timestamp, jlong duration)
{
    static const char method_name[] = "MetadataStream::n_1convertDurationToNumOfFrames";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long long frameIndex = 0, numOfFrames = 0;

        (*obj)->convertTimestampToFrameIndex((long long)timestamp, (long long)duration, frameIndex, numOfFrames);
        return (jlong)numOfFrames;
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_convertFrameIndexToTimestamp
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertFrameIndexToTimestamp(JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertFrameIndexToTimestamp (JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames)
{
    static const char method_name[] = "MetadataStream::n_1convertFrameIndexToTimestamp";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long long timestamp = 0, duration = 0;
        (*obj)->convertFrameIndexToTimestamp((long long)frameIndex, (long long)numOfFrames, timestamp, duration);
        return (jlong)timestamp;
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
* Class:     com_intel_vmf_MetadataStream
* Method:    n_convertNumOfFramesToDuration
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertNumOfFramesToDuration(JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1convertNumOfFramesToDuration(JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames)
{
    static const char method_name[] = "MetadataStream::n_1convertNumOfFramesToDuration";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long long timestamp = 0, duration = 0;
        (*obj)->convertFrameIndexToTimestamp((long long)frameIndex, (long long)numOfFrames, timestamp, duration);
        return (jlong)duration;
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByFrameIndex
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByFrameIndex(JNIEnv *env, jclass, jlong self, jlong id);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByFrameIndex (JNIEnv *env, jclass, jlong self, jlong id)
{
    static const char method_name[] = "MetadataStream::n_1queryByFrameIndex";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        size_t index = (size_t)id;
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByFrameIndex(index)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByTime
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByTime(JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByTime (JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime)
{
    static const char method_name[] = "MetadataStream::n_1queryByTime";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByTime((long long)startTime, (long long)endTime)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryBySchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryBySchema(JNIEnv *env, jclass, jlong self, jstring schemaName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryBySchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "MetadataStream::n_1queryBySchema";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars (schemaName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(schemaName, tmp);

        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryBySchema(sName)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByName(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByName (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "MetadataStream::n_1queryByName";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(name, tmp);
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByName(sName)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndValue
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                  jlong fieldValueAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                  jlong fieldValueAddr)
{
    static const char method_name[] = "MetadataStream::n_1queryByNameAndValue";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::shared_ptr <FieldValue>* value = (std::shared_ptr <FieldValue>*)fieldValueAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((value == NULL) || (*value == NULL) || (value->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(mdName, tmp);
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByNameAndValue(sName, (**value))));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndFields
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndFields(JNIEnv *env, jclass, jlong self, jstring mdName, jlongArray fieldValues);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndFields (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                   jlongArray fieldValues)
{
    static const char method_name[] = "MetadataStream::n_1queryByNameAndFields";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <FieldValue> values;
        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        jlong *body = env->GetLongArrayElements(fieldValues, 0);
        jsize len = env->GetArrayLength(fieldValues);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr <FieldValue>* addr = (std::shared_ptr <FieldValue>*)body[i];
            values.push_back(**addr);
        }

        env->ReleaseLongArrayElements(fieldValues, body, 0);
        env->ReleaseStringUTFChars(mdName, tmp);
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByNameAndFields(sName, values)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "MetadataStream::n_1queryByReference__JLjava_lang_String_2";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(name, tmp);

        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference(sName)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2J(JNIEnv *env, jclass, jlong self, jstring name, jlong valueAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                      jlong valueAddr)
{
    static const char method_name[] = "MetadataStream::n_1queryByReference__JLjava_lang_String_2J";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
        std::shared_ptr <FieldValue>* value = (std::shared_ptr <FieldValue>*)valueAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(name, tmp);

        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference(sName, (**value))));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2_3J(JNIEnv *env, jclass, jlong self, jstring name, jlongArray fieldValues);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2_3J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                        jlongArray fieldValues)
{
    static const char method_name[] = "MetadataStream::n_1queryByReference__JLjava_lang_String_2_3J";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(name, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(name, tmp);

        jlong *body = env->GetLongArrayElements(fieldValues, 0);
        std::vector <FieldValue> values;
        jsize len = env->GetArrayLength(fieldValues);

        for (int i = 0; i < len; i++)
        {
            std::shared_ptr <FieldValue>* addr = (std::shared_ptr <FieldValue>*) body[i];
            values.push_back(**addr);
        }

        env->ReleaseLongArrayElements(fieldValues, body, 0);
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference(sName, values)));
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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1delete";

    try
    {
        std::shared_ptr<MetadataStream>* obj = (std::shared_ptr<MetadataStream>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Stream is null pointer.");

        delete obj;
    }
    catch (const std::exception &e)
    {
        throwJavaException (env, &e, method_name);
    }
    catch (...)
    {
        throwJavaException (env, 0, method_name);
    }
}

//void n_addStat(long self, long stat);
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addStat(JNIEnv *env, jclass, jlong self, jlong stat);

JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addStat(JNIEnv *env, jclass, jlong self, jlong stat)
{
    static const char method_name[] = "MetadataStream::n_1addStat";
    try
    {
        std::shared_ptr<MetadataStream>* obj = (std::shared_ptr<MetadataStream>*) self;
        if (obj == NULL || *obj == NULL) VMF_EXCEPTION(NullPointerException, "Stream (self) is null pointer.");

        std::shared_ptr<Stat>* statObj = (std::shared_ptr<Stat>*)stat;
        if (statObj == NULL || *statObj == NULL) VMF_EXCEPTION(NullPointerException, "Stat object to add is null pointer.");

        (*obj)->addStat(*statObj);
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name);}
    catch (...) { throwJavaException(env, 0, method_name); }
}

//String[] n_getAllStatNames(long self);
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllStatNames(JNIEnv *env, jclass, jlong self);

JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllStatNames(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getAllStatNames";
    try
    {
        std::shared_ptr<MetadataStream>* obj = (std::shared_ptr<MetadataStream>*) self;
        if (obj == NULL || *obj == NULL) VMF_EXCEPTION(NullPointerException, "Stream (self) is null pointer.");

        std::vector<std::string> names = (*obj)->getAllStatNames();
        jobjectArray result = (jobjectArray)env->NewObjectArray((jsize)names.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (jsize i = 0; i<names.size(); i++)
            env->SetObjectArrayElement(result, i, env->NewStringUTF(names[i].c_str()));

        return result;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_getStat(long self, String name);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getStat(JNIEnv *env, jclass, jlong self, jstring name);

JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getStat(JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "MetadataStream::n_1getStat";
    try
    {
        std::shared_ptr<MetadataStream>* obj = (std::shared_ptr<MetadataStream>*) self;
        if (obj == NULL || *obj == NULL) VMF_EXCEPTION(NullPointerException, "Stream (self) is null pointer.");

        const char* _name = env->GetStringUTFChars(name, NULL);
        std::string _nameS(_name);
        env->ReleaseStringUTFChars(name, _name);

        return (jlong) new std::shared_ptr<Stat>( (*obj)->getStat(_nameS) );
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//void n_recalcStat(long self);
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1recalcStat(JNIEnv *env, jclass, jlong self);

JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1recalcStat(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1recalcStat";
    try
    {
        std::shared_ptr<MetadataStream>* obj = (std::shared_ptr<MetadataStream>*) self;
        if (obj == NULL || *obj == NULL) VMF_EXCEPTION(NullPointerException, "Stream (self) is null pointer.");

        (*obj)->recalcStat();
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

}
