#include<string>
#include<vector>
#include "vmf/jsonwriter.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_JSONWriter
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_JSONWriter_n_1JSONWriter(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_JSONWriter_n_1JSONWriter (JNIEnv *env, jclass)
{
    static const char method_name[] = "JSONWriter::n_1JSONWriter";

    try
    {
        std::shared_ptr<JSONWriter>* obj = new std::shared_ptr<JSONWriter>(new JSONWriter());
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_storeSchemas
 * Signature: (J[J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSchemas(JNIEnv *env, jclass, jlong self, jlongArray schemaAddrs);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSchemas (JNIEnv *env, jclass, jlong self, jlongArray schemaAddrs)
{
    static const char method_name[] = "JSONWriter::n_1storeSchemas";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;

        if((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;

        jlong* schemasArray = env->GetLongArrayElements(schemaAddrs, 0);
        jsize lenSchemas = env->GetArrayLength (schemaAddrs);

        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back (*pSpSchema);
        }

        env->ReleaseLongArrayElements (schemaAddrs, schemasArray, 0);

        std::string str = (*obj)->store (vecSchemas);
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_storeMetadataSet
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeMetadataSet(JNIEnv *env, jclass, jlong self, jlong setAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeMetadataSet (JNIEnv *env, jclass, jlong self, jlong setAddr)
{
    static const char method_name[] = "JSONWriter::n_1storeMetadataSet";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;
        std::shared_ptr<MetadataSet>* set = (std::shared_ptr<MetadataSet>*) setAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((set == NULL) || (*set == NULL) || (set->get() == NULL))
            return 0;

        std::string str = (*obj)->store(**set);
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_storeAll
 * Signature: (JJLjava/lang/String;Ljava/lang/String;[J[JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeAll(JNIEnv *env, jclass, jlong self, jlong nextId, jstring path, jstring checksum, jlongArray segAddrs, jlongArray schemaAddrs, jlong setAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeAll (JNIEnv *env, jclass, jlong self, jlong nextId, jstring path, jstring checksum, jlongArray segAddrs, jlongArray schemaAddrs, jlong setAddr)
{
    static const char method_name[] = "JSONWriter::n_1storeAll";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;
        std::shared_ptr<MetadataSet>* set = (std::shared_ptr<MetadataSet>*) setAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((set == NULL) || (*set == NULL) || (set->get() == NULL))
            return 0;

        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;

        const char* file = env->GetStringUTFChars(path, NULL);
        const char* sum = env->GetStringUTFChars(checksum, NULL);

        std::string sPath (file);
        std::string sChecksum (sum);

        env->ReleaseStringUTFChars (path, file);
        env->ReleaseStringUTFChars (checksum, sum);

        jlong* segmentsArray = env->GetLongArrayElements (segAddrs, 0);
        jlong* schemasArray = env->GetLongArrayElements (schemaAddrs, 0);

        jsize lenSegments = env->GetArrayLength (segAddrs);
        jsize lenSchemas = env->GetArrayLength (schemaAddrs);

        for (int i = 0; i < lenSegments; i++)
        {
            std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
            vecSegments.push_back(*pSpSegment);
        }

        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back(*pSpSchema);
        }

        env->ReleaseLongArrayElements(segAddrs, segmentsArray, 0);
        env->ReleaseLongArrayElements(schemaAddrs, schemasArray, 0);
                
        //TODO: fix it later
        const std::string hint = "";

        std::string str = (*obj)->store((IdType&)nextId, sPath, sChecksum, vecSegments, vecSchemas, (**set), hint);
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_storeSegment
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSegment(JNIEnv *env, jclass, jlong self, jlong segAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSegment (JNIEnv *env, jclass, jlong self, jlong segAddr)
{
    static const char method_name[] = "JSONWriter::n_1storeSegment";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;
        std::shared_ptr<MetadataStream::VideoSegment>* segment = (std::shared_ptr<MetadataStream::VideoSegment>*) segAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((segment == NULL) || (*segment == NULL) || (segment->get() == NULL))
            return 0;

        std::string str = (*obj)->store(*segment);
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_storeSegments
 * Signature: (J[J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSegments(JNIEnv *env, jclass, jlong self, jlongArray segAddrs);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_JSONWriter_n_1storeSegments (JNIEnv *env, jclass, jlong self, jlongArray segAddrs)
{
    static const char method_name[] = "JSONWriter::n_1storeSegments";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;

        jlong* segmentsArray = env->GetLongArrayElements(segAddrs, 0);
        jsize lenSegments = env->GetArrayLength(segAddrs);

        for (int i = 0; i < lenSegments; i++)
        {
            std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
            vecSegments.push_back(*pSpSegment);
        }

        env->ReleaseLongArrayElements (segAddrs, segmentsArray, 0);

        std::string str = (*obj)->store(vecSegments);
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
 * Class:     com_intel_vmf_JSONWriter
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_JSONWriter_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_JSONWriter_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "JSONWriter::n_1delete";

    try
    {
        std::shared_ptr<JSONWriter>* obj = (std::shared_ptr<JSONWriter>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Writer is null pointer.");

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
