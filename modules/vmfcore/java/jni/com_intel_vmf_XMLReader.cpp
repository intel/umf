#include<string>
#include<vector>
#include "vmf/xmlreader.hpp"
#include "../com_intel_vmf_XMLReader.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
 * Class:     com_intel_vmf_XMLReader
 * Method:    n_XMLReader
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_XMLReader_n_1XMLReader (JNIEnv *env, jclass)
{
    std::shared_ptr<XMLReader>* p = new std::shared_ptr<XMLReader>(new XMLReader());
    return (jlong)p;
}

/*
* Class:     com_intel_vmf_XMLReader
* Method:    n_parseSchemas
* Signature: (JLjava/lang/String;)[J
*/
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_XMLReader_n_1parseSchemas (JNIEnv *env, jclass, jlong self, jstring text)
{
    static const char method_name[] = "XMLReader::n_1parseSchemas";

    try
    {
        std::shared_ptr<XMLReader>* obj = (std::shared_ptr<XMLReader>*) self;
        const char* tmp = env->GetStringUTFChars(text, NULL);
        std::string sText(tmp);
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;

        env->ReleaseStringUTFChars(text, tmp);

        bool result = (*obj)->parseSchemas(sText, vecSchemas);

        if (result == JNI_TRUE)
        {
            jsize length = (jsize)vecSchemas.size();
            jlongArray schemaNativeAddrs = env->NewLongArray(length);
            jlong* schemaArray = env->GetLongArrayElements(schemaNativeAddrs, 0);

            for (int j = 0; j < length; j++)
                schemaArray[j] = (jlong)new std::shared_ptr<MetadataSchema>(vecSchemas[j]);

            env->ReleaseLongArrayElements(schemaNativeAddrs, schemaArray, 0);
            return schemaNativeAddrs;
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Schemas can not be parsed.");
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
* Class:     com_intel_vmf_XMLReader
* Method:    n_parseMetadata
* Signature: (JLjava/lang/String;[J)[J
*/
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_XMLReader_n_1parseMetadata (JNIEnv *env, jclass, jlong self, jstring text, jlongArray schemaNativeAddrs)
{
    static const char method_name[] = "XMLReader::n_1parseMetadata";

    try
    {
        std::shared_ptr <XMLReader>* obj = (std::shared_ptr <XMLReader>*) self;

        const char* tmp = env->GetStringUTFChars(text, NULL);
        std::string sText(tmp);
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;
        std::vector <std::shared_ptr<MetadataInternal>> vecMdInt;

        env->ReleaseStringUTFChars(text, tmp);

        jlong* schemaArray = env->GetLongArrayElements (schemaNativeAddrs, 0);
        jsize schemaArrayLength = env->GetArrayLength (schemaNativeAddrs);

        for (int i = 0; i < schemaArrayLength; i++)
        {
            std::shared_ptr<MetadataSchema>* schema = (std::shared_ptr<MetadataSchema>*)schemaArray[i];
            vecSchemas.push_back(*schema);
        }

        bool result = (*obj)->parseMetadata(sText, vecSchemas, vecMdInt);

        env->ReleaseLongArrayElements(schemaNativeAddrs, schemaArray, 0);

        if (result == JNI_TRUE)
        {
            jsize mdIntArrayLength = (jsize)vecMdInt.size();
            jlongArray mdIntNativeAddrs = env->NewLongArray(mdIntArrayLength);
            jlong* mdIntArray = env->GetLongArrayElements(mdIntNativeAddrs, 0);

            for (int i = 0; i < mdIntArrayLength; i++)
                mdIntArray[i] = (jlong)new std::shared_ptr<MetadataInternal>(vecMdInt[i]);

            env->ReleaseLongArrayElements (mdIntNativeAddrs, mdIntArray, 0);
            return mdIntNativeAddrs;
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Metadata can not be parsed.");
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
* Class:     com_intel_vmf_XMLReader
* Method:    n_parseVideoSegments
* Signature: (JLjava/lang/String;)[J
*/
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_XMLReader_n_1parseVideoSegments (JNIEnv *env, jclass, jlong self, jstring text)
{
    static const char method_name[] = "XMLReader::n_1parseVideoSegments";

    try
    {
        std::shared_ptr<XMLReader>* obj = (std::shared_ptr<XMLReader>*) self;

        const char* tmp = env->GetStringUTFChars(text, NULL);
        std::string sText(tmp);
        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;

        env->ReleaseStringUTFChars(text, tmp);

        bool result = (*obj)->parseVideoSegments(sText, vecSegments);


        if (result == JNI_TRUE)
        {
            jsize length = (jsize)vecSegments.size();
            jlongArray segmentsNativeAddrs = env->NewLongArray(length);
            jlong* segmentsArray = env->GetLongArrayElements(segmentsNativeAddrs, 0);

            for (int i = 0; i < length; i++)
                segmentsArray[i] = (jlong) new std::shared_ptr<MetadataStream::VideoSegment>(vecSegments[i]);

            env->ReleaseLongArrayElements(segmentsNativeAddrs, segmentsArray, 0);
            return segmentsNativeAddrs;
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Segments can not be parsed.");
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
 * Class:     com_intel_vmf_XMLReader
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_XMLReader_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "XMLReader::n_1delete";

    try
    {
        std::shared_ptr<XMLReader>* p = (std::shared_ptr<XMLReader>*) self;
        delete p;
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
