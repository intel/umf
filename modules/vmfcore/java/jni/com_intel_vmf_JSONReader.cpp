#include<string>
#include<vector>
#include "../com_intel_vmf_ReferenceDesc.h"

/*
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_JSONReader
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_JSONReader_n_1JSONReader (JNIEnv *env, jclass)
{
    std::shared_ptr<JSONReader>* p = new std::shared_ptr<JSONReader>(new JSONReader ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_parseAll
 * Signature: (JLjava/lang/String;JLjava/lang/String;Ljava/lang/String;[J[J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_JSONReader_n_1parseAll (JNIEnv *, jclass, jlong self, jstring text, jlong nextID, jstring path, jstring checksum, jlongArray segments, jlongArray schemas, jlongArray md)
{
    static const char method_name[] = "JSONReader::n_1parseAll";
    
    try 
    {
        std::shared_ptr<JSONReader>* obj = (std::shared_ptr<JSONReader>*) self;
        
        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;
        std::vector <std::shared_ptr<MetadataInternal>> vecMdInt;
        
        std::string sText (env->GetStringUTFChars (text, NULL));
        std::string sPath (env->GetStringUTFChars (path, NULL));
        std::string sChecksum (env->GetStringUTFChars (checksum, NULL));
        
        jlong* segmentsArray = env->GetLongArrayElements (segments, 0);
        jlong* schemasArray = env->GetLongArrayElements (schemas, 0);
        jlong* mdIntArray = env->GetLongArrayElements (md, 0);
        
        jsize lenSegments = env->GetArrayLength (segments);
        jsize lenSchemas = env->GetArrayLength (schemas);
        jsize lenMd = env->GetArrayLength (md);
        
        for (int i = 0; i < lenSegments; i++)
        {
            std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
            vecSegments.push_back (*pSpSegment);
        }
        
        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back (*pSpSchema);
        }
        
        for (int k = 0; k < lenMd; k++)
        {
            std::shared_ptr<MetadataInternal>* pSpMdInt = (std::shared_ptr<MetadataInternal>*) mdIntArray[k];
            vecMdInt.push_back (*pSpMdInt);
        }
        
        env->ReleaseLongArrayElements (segments, segmentsArray, 0);
        env->ReleaseLongArrayElements (schemas, schemasArray, 0);
        env->ReleaseLongArrayElements (md, mdIntArray, 0);
        
        return (jboolean) (*obj)->parseAll (sText, (int64_t)nextID, sPath, sChecksum, vecSegments, vecSchemas, vecMdInt);
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
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_parseSchemas
 * Signature: (JLjava/lang/String;[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_JSONReader_n_1parseSchemas (JNIEnv *env, jclass, jlong self, jstring text, jlongArray schemas)
{
    static const char method_name[] = "JSONReader::n_1parseSchemas";
    
    try 
    {
        std::shared_ptr<JSONReader>* obj = (std::shared_ptr<JSONReader>*) self;
        std::string sText (env->GetStringUTFChars (text, NULL));
        
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;
        
        jlong* schemasArray = env->GetLongArrayElements (schemas, 0);
        jsize lenSchemas = env->GetArrayLength (schemas);
        
        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back (*pSpSchema);
        }
        
        env->ReleaseLongArrayElements (schemas, schemasArray, 0);
        
        return (jboolean) (*obj)->parseSchemas (sText, vecSchemas);
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
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_parseMetadata
 * Signature: (JLjava/lang/String;[J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_JSONReader_n_1parseMetadata (JNIEnv *env, jclass, jlong self, jstring text, jlongArray schemas, jlongArray md)
{
    static const char method_name[] = "JSONReader::n_1parseMetadata";
    
    try 
    {
        std::shared_ptr<JSONReader>* obj = (std::shared_ptr<JSONReader>*) self;
        std::string sText (env->GetStringUTFChars (text, NULL));
        
        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;
        std::vector <std::shared_ptr<MetadataInternal>> vecMdInt;
        
        jlong* schemasArray = env->GetLongArrayElements (schemas, 0);
        jlong* mdIntArray = env->GetLongArrayElements (md, 0);
        
        jsize lenSchemas = env->GetArrayLength (schemas);
        jsize lenMd = env->GetArrayLength (md);
        
        for (int j = 0; j < lenSchemas; j++)
        {
            std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
            vecSchemas.push_back (*pSpSchema);
        }
        
        for (int k = 0; k < lenMd; k++)
        {
            std::shared_ptr<MetadataInternal>* pSpMdInt = (std::shared_ptr<MetadataInternal>*) mdIntArray[k];
            vecMdInt.push_back (*pSpMdInt);
        }
        
        env->ReleaseLongArrayElements (schemas, schemasArray, 0);
        env->ReleaseLongArrayElements (md, mdIntArray, 0);
        
        return (jboolean) (*obj)->parseMetadata (sText, vecSchemas, vecMdInt);
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
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_parseVideoSegments
 * Signature: (JLjava/lang/String;[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_JSONReader_n_1parseVideoSegments (JNIEnv *env, jclass, jlong self, jstring text, jlongArray segments)
{
    static const char method_name[] = "JSONReader::n_1parseMetadata";
    
    try 
    {
        std::shared_ptr<JSONReader>* obj = (std::shared_ptr<JSONReader>*) self;
        std::string sText (env->GetStringUTFChars (text, NULL));
        
        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;
        
        jlong* segmentsArray = env->GetLongArrayElements (segments, 0);
        jsize lenSegments = env->GetArrayLength (segments);
        
        for (int i = 0; i < lenSegments; i++)
        {
            std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
            vecSegments.push_back (*pSpSegment);
        }
        
        env->ReleaseLongArrayElements (segments, segmentsArray, 0);
        
        return (jboolean) (*obj)->parseVideoSegments (sText, vecSegments);
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
 * Class:     com_intel_vmf_JSONReader
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_JSONReader_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "JSONReader::n_1delete";
    
    try 
    {
        std::shared_ptr<JSONReader>* p = (std::shared_ptr<JSONReader>*) self;
        delete (*p);
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
