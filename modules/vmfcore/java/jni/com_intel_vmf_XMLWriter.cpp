#include<string>
#include<vector>
#include "vmf/xmlwriter.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_XMLWriter
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_XMLWriter_n_1XMLWriter(JNIEnv *env, jclass);

JNIEXPORT jlong JNICALL Java_com_intel_vmf_XMLWriter_n_1XMLWriter (JNIEnv *env, jclass)
{
    static const char method_name[] = "XMLWriter::n_1XMLWriter";

    try
    {
        std::shared_ptr<XMLWriter>* obj = new std::shared_ptr<XMLWriter>(new XMLWriter());
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
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_storeSchemas
 * Signature: (J[J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeSchemas(JNIEnv *env, jclass, jlong self, jlongArray schemaAddrs);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeSchemas (JNIEnv *env, jclass, jlong self, jlongArray schemaAddrs)
{
    static const char method_name[] = "XMLWriter::n_1storeSchemas";

    try
    {
        std::shared_ptr<XMLWriter>* obj = (std::shared_ptr<XMLWriter>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;

        jlong* schemasArray = env->GetLongArrayElements(schemaAddrs, 0);
        jsize lenSchemas = env->GetArrayLength(schemaAddrs);

        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back(*pSpSchema);
        }

        env->ReleaseLongArrayElements(schemaAddrs, schemasArray, 0);

        std::string str = (*obj)->store(vecSchemas);
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
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_storeMetadataSet
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeMetadataSet(JNIEnv *env, jclass, jlong self, jlong setAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeMetadataSet (JNIEnv *env, jclass, jlong self, jlong setAddr)
{
    static const char method_name[] = "XMLWriter::n_1storeMetadataSet";

    try
    {
        std::shared_ptr<XMLWriter>* obj = (std::shared_ptr<XMLWriter>*) self;
        std::shared_ptr<MetadataSet>* set = (std::shared_ptr<MetadataSet>*) setAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((set == NULL) || (set->get() == NULL))
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
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_storeAll
 * Signature: (JJLjava/lang/String;Ljava/lang/String;[J[JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeAll(JNIEnv *env, jclass, jlong self, jlong nextId, jstring path, jstring checksum, jlongArray segAddrs, jlongArray schemaAddrs, jlong setAddr);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeAll (JNIEnv *env, jclass, jlong self, jlong nextId, jstring path, jstring checksum, jlongArray segAddrs, jlongArray schemaAddrs, jlong setAddr)
{
    static const char method_name[] = "XMLWriter::n_1storeAll";

    try
    {
        std::shared_ptr<XMLWriter>* obj = (std::shared_ptr<XMLWriter>*) self;
        std::shared_ptr<MetadataSet>* set = (std::shared_ptr<MetadataSet>*) setAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        if ((set == NULL) || (set->get() == NULL))
            return 0;

        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;

        std::string sPath(env->GetStringUTFChars(path, NULL));
        std::string sChecksum(env->GetStringUTFChars(checksum, NULL));

        jlong* segmentsArray = env->GetLongArrayElements(segAddrs, 0);
        jlong* schemasArray = env->GetLongArrayElements(schemaAddrs, 0);

        jsize lenSegments = env->GetArrayLength(segAddrs);
        jsize lenSchemas = env->GetArrayLength(schemaAddrs);

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
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_storeSegments
 * Signature: (J[J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeSegments(JNIEnv *env, jclass, jlong self, jlongArray segAddrs);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_XMLWriter_n_1storeSegments(JNIEnv *env, jclass, jlong self, jlongArray segAddrs)
{
    static const char method_name[] = "XMLWriter::n_1storeSegments";

    try
    {
        std::shared_ptr<XMLWriter>* obj = (std::shared_ptr<XMLWriter>*) self;
        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        jlong* segmentsArray = env->GetLongArrayElements(segAddrs, 0);
        jsize lenSegments = env->GetArrayLength(segAddrs);

        for (int i = 0; i < lenSegments; i++)
        {
            std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
            vecSegments.push_back(*pSpSegment);
        }

        env->ReleaseLongArrayElements(segAddrs, segmentsArray, 0);

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
 * Class:     com_intel_vmf_XMLWriter
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_XMLWriter_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_XMLWriter_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "XMLWriter::n_1delete";

    try
    {
        std::shared_ptr<XMLWriter>* obj = (std::shared_ptr<XMLWriter>*) self;

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
