#include<string>
#include<vector>
#include <memory>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_MetadataStream.h"
#include "throwJavaException.hpp"

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_MetadataStream
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1MetadataStream (JNIEnv *env, jclass)
{
    std::shared_ptr <MetadataStream>* p = new std::shared_ptr <MetadataStream>(new MetadataStream ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_open
 * Signature: (JLjava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1open (JNIEnv *env, jclass, jlong self, jstring filePath, jint mode)
{
    static const char method_name[] = "Metadata::n_1open";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::string sPath(env->GetStringUTFChars(filePath, NULL));
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
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1reopen (JNIEnv *env, jclass, jlong self, jint mode)
{
    static const char method_name[] = "Metadata::n_1reopen";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
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
 * Method:    n_load
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    static const char method_name[] = "Metadata::n_1load__JLjava_lang_String_2";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
        std::string sName (env->GetStringUTFChars (schemaName, NULL));
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
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName, jstring mdName)
{
    static const char method_name[] = "Metadata::n_1load__JLjava_lang_String_2Ljava_lang_String_2";

    try
    {
        std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
        std::string sName(env->GetStringUTFChars (schemaName, NULL));
        std::string mName(env->GetStringUTFChars (mdName, NULL));
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
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1save (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    return (jboolean) (*obj)->save();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_saveTo
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1saveTo (JNIEnv *env, jclass, jlong self, jstring filePath)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::string sPath (env->GetStringUTFChars (filePath, NULL));
    return (jboolean) (*obj)->saveTo (sPath);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1close (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    (*obj)->close();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getById
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getById (JNIEnv *env, jclass, jlong self, jlong id)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::shared_ptr <Metadata>* md = new std::shared_ptr <Metadata>(new Metadata (*(*obj)->getById((long long)id)));
    return (jlong) md;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_add
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1add (JNIEnv *env, jclass, jlong self, jlong mdAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::shared_ptr <Metadata>* md = (std::shared_ptr <Metadata>*) mdAddr;
    
    return (jlong) (*obj)->add (*md);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addInternal
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1addInternal (JNIEnv *env, jclass, jlong self, jlong mdIntAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataInternal>* mdInt = (std::shared_ptr <MetadataInternal>*) mdIntAddr;
    
    return (jlong) (*obj)->add (*mdInt);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeMdById
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1removeMdById (JNIEnv *env, jclass, jlong self, jlong id)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    return (jboolean) (*obj)->remove ((long long) id);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSet
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSet (JNIEnv *env, jclass, jlong self, jlong mdSetAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataSet>* mdSet = (std::shared_ptr <MetadataSet>*) mdSetAddr;
    (*obj)->remove ((**mdSet));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataSchema>* mdSchema = (std::shared_ptr <MetadataSchema>*) schemaAddr;
    
    (*obj)->remove (*mdSchema);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeAllMd
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeAllMd (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    (*obj)->remove ();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataSchema>* mdSchema = (std::shared_ptr <MetadataSchema>*) schemaAddr;
    
    (*obj)->addSchema (*mdSchema);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getSchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getSchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    std::shared_ptr <MetadataSchema>* mdSchema = new std::shared_ptr <MetadataSchema>((*obj)->getSchema (sName));
    return (jlong) mdSchema;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllSchemaNames
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllSchemaNames (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::vector <std::string> vecNames = (*obj)->getAllSchemaNames ();
    
    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray objArray = env->NewObjectArray((jsize)vecNames.size(), stringClass, NULL);
    
    int counter = 0;
    for (auto it = vecNames.begin(); it != vecNames.end(); it++, counter++)
    {
        jstring str = env->NewStringUTF (it->c_str());
        env->SetObjectArrayElement (objArray, counter, str);
    }
    
    return objArray;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAll
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getAll (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataSet>* mdSet = new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->getAll()));
    return (jlong) mdSet;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_importSet
 * Signature: (JJJJJJJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1importSet (JNIEnv *env, jclass, jlong self, jlong srcStreamAddr,
                                                                           jlong srcSetAddr, jlong dstFrameIndex, jlong srcFrameIndex,
                                                                           jlong numOfFrames, jlong setFalureAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataStream>* srcStream = (std::shared_ptr <MetadataStream>*) srcStreamAddr;
    std::shared_ptr <MetadataSet>* srcSet = (std::shared_ptr <MetadataSet>*) srcSetAddr;
    std::shared_ptr <MetadataSet>* setFalure = (std::shared_ptr <MetadataSet>*) setFalureAddr;
    
    return (jboolean) (*obj)->import ((**srcStream), (**srcSet), (long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames, (*setFalure).get());
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_sortMdSetById
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1sortMdSetById (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    (*obj)->sortById ();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_serialize
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1serialize (JNIEnv *env, jclass, jlong self, jlong IWriterAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <IWriter>* writer = (std::shared_ptr <IWriter>*)IWriterAddr;
    jstring str = env->NewStringUTF (((*obj)->serialize ((**writer))).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_deserialize
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1deserialize (JNIEnv *env, jclass, jlong self, jstring text, jlong IReaderAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::shared_ptr <IReader>* reader = (std::shared_ptr <IReader>*) IReaderAddr;
    std::string sText (env->GetStringUTFChars (text, NULL));
    (*obj)->deserialize (sText, (**reader));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_computeChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    jstring str = env->NewStringUTF (((*obj)->computeChecksum()).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_computeChecksum
 * Signature: (JJJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__JJJ (JNIEnv *env, jclass, jlong self, jlong XMPPacketSize, jlong XMPPacketOffset)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    long long packetSize = (long long)XMPPacketSize;
    long long packetOffset = (long long)XMPPacketOffset;
    std::string result = (*obj)->computeChecksum (packetSize, packetOffset);
    return env->NewStringUTF(result.c_str());
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1getChecksum (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    jstring str = env->NewStringUTF (((*obj)->getChecksum()).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_setChecksum
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1setChecksum (JNIEnv *env, jclass, jlong self, jstring checksum)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::string str (env->GetStringUTFChars (checksum, NULL));
    (*obj)->setChecksum (str);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addVideoSegment
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addVideoSegment (JNIEnv *env, jclass, jlong self, jlong segmentAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::shared_ptr <MetadataStream::VideoSegment>* segment = (std::shared_ptr <MetadataStream::VideoSegment>*) segmentAddr;
    
    (*obj)->addVideoSegment (*segment);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllVideoSegment
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllVideoSegments (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::vector <std::shared_ptr<MetadataStream::VideoSegment>> segments;
    
    segments = (*obj)->getAllVideoSegments();
    jlongArray nObjs = env->NewLongArray ((jsize)segments.size());
    
    jlong* body = env->GetLongArrayElements (nObjs, 0);
    int counter = 0;
    for (auto it = segments.begin(); it != segments.end(); it++, counter++)
        body[counter] = (jlong) new std::shared_ptr<MetadataStream::VideoSegment>(*it);
        
    env->ReleaseLongArrayElements (nObjs, body, 0);
    
    return nObjs;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_convertTimestampToFrameIndex
 * Signature: (JJJJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1convertTimestampToFrameIndex (JNIEnv *env, jclass, jlong self,
                                                                                          jlong timestamp, jlong duration,
                                                                                          jlongArray frameIndexAndNumOfFrames)
{
    static const char method_name[] = "MetadataStream::n_1convertTimestampToFrameIndex";
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    jlong *cArray = env->GetLongArrayElements (frameIndexAndNumOfFrames, 0);
    jsize len = env->GetArrayLength(frameIndexAndNumOfFrames);
    long long frameIndex = 0, numOfFrames = 0;

    if (len == 2)
    {
        (*obj)->convertTimestampToFrameIndex((long long)timestamp, (long long)duration, frameIndex, numOfFrames);
    }
    else
        throwJavaException(env, 0, method_name);

    cArray[0] = (jlong)frameIndex;
    cArray[1] = (jlong)numOfFrames;
    env->ReleaseLongArrayElements(frameIndexAndNumOfFrames, cArray, 0);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_convertFrameIndexToTimestamp
 * Signature: (JJJJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1convertFrameIndexToTimestamp(JNIEnv *env, jclass, jlong self,
    jlong frameIndex, jlong numOfFrames,
    jlongArray timestampAndDuration)
{
    static const char method_name[] = "MetadataStream::n_1convertFrameIndexToTimestamp";

    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    jlong *cArray = env->GetLongArrayElements(timestampAndDuration, 0);
    jsize len = env->GetArrayLength(timestampAndDuration);
    long long timestamp = 0, duration = 0;

    if (len == 2)
    {
        
        (*obj)->convertFrameIndexToTimestamp((long long)frameIndex, (long long)numOfFrames, timestamp, duration);
    }
    else
    {
        throwJavaException(env, 0, method_name);
    }

    cArray[0] = (jlong)timestamp;
    cArray[1] = (jlong)duration;
    env->ReleaseLongArrayElements(timestampAndDuration, cArray, 0);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByFrameIndex
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByFrameIndex (JNIEnv *env, jclass, jlong self, jlong id)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    size_t index = (size_t) id;
    return (jlong) new std::shared_ptr <MetadataSet> (new MetadataSet ((*obj)->queryByFrameIndex (index)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByTime
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByTime (JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByTime((long long)startTime, (long long)endTime)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryBySchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryBySchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryBySchema(sName)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByName (JNIEnv *env, jclass, jlong self, jstring name)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByName(sName)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndValue
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                  jlong fieldValueAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*) self;
    FieldValue* value = (FieldValue*) fieldValueAddr;
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByNameAndValue (sName, (*value))));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndFields
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndFields (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                   jlongArray fieldValues)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::vector <FieldValue> values;
    
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    jlong *body =env->GetLongArrayElements (fieldValues, 0);
    jsize len = env->GetArrayLength (fieldValues);
    
    for (int i = 0; i < len; i++)
    {
        std::shared_ptr <FieldValue>* addr = (std::shared_ptr <FieldValue>*)body[i];
        values.push_back (**addr);
    }
    
    env->ReleaseLongArrayElements (fieldValues, body, 0);
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByNameAndFields (sName, values)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring name)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference(sName)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                      jlong valueAddr)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::shared_ptr <FieldValue>* value = (std::shared_ptr <FieldValue>*)valueAddr;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference(sName, (**value))));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2_3J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                        jlongArray fieldValues)
{
    std::shared_ptr <MetadataStream>* obj = (std::shared_ptr <MetadataStream>*)self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    jlong *body = env->GetLongArrayElements (fieldValues, 0);
    std::vector <FieldValue> values;
    jsize len = env->GetArrayLength (fieldValues);
    
    for (int i = 0; i < len; i++)
    {
        std::shared_ptr <FieldValue>* addr = (std::shared_ptr <FieldValue>*) body[i];
        values.push_back (**addr);
    }
    
    env->ReleaseLongArrayElements (fieldValues, body, 0);
    return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->queryByReference (sName, values)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1delete";

    try
    {
        std::shared_ptr<MetadataStream>* p = (std::shared_ptr<MetadataStream>*) self;
        delete p;
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
