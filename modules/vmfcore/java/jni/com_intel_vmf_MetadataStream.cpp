#include<string>
#include<vector>
#include <memory>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_MetadataStream.h"

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
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_MetadataStream
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1MetadataStream (JNIEnv *env, jclass)
{
    return (jlong) new MetadataStream ();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_open
 * Signature: (JLjava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1open (JNIEnv *env, jclass, jlong self, jstring filePath, jint mode)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sPath (env->GetStringUTFChars (filePath, NULL));
    return (jboolean)obj->open(sPath, (MetadataStream::OpenMode) mode);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_reopen
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1reopen (JNIEnv *env, jclass, jlong self, jint mode)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jboolean)obj->reopen(((MetadataStream::OpenMode)mode));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_load
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    return (jboolean) obj->load (sName);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_load
 * Signature: (JLjava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1load__JLjava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring schemaName, jstring mdName)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    std::string mName (env->GetStringUTFChars (mdName, NULL));
    return (jboolean) obj->load (sName, mName);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_save
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1save (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jboolean) obj->save();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_saveTo
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1saveTo (JNIEnv *env, jclass, jlong self, jstring filePath)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sPath (env->GetStringUTFChars (filePath, NULL));
    return (jboolean) obj->saveTo(sPath);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1close (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    obj->close();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getById
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getById (JNIEnv *env, jclass, jlong self, jlong id)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jlong) obj->getById((long long) id).get();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_add
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1add (JNIEnv *env, jclass, jlong self, jlong mdAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    Metadata* md = (Metadata*) mdAddr;
    std::shared_ptr <Metadata> spMetadata = std::make_shared <Metadata>((*md)); 
    return (jlong) obj->add (spMetadata);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addInternal
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1addInternal (JNIEnv *env, jclass, jlong self, jlong mdIntAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    MetadataInternal* mdInt = (MetadataInternal*) mdIntAddr;
    std::shared_ptr <MetadataInternal> spMetadataInt = std::make_shared <MetadataInternal>((*mdInt));
    return (jlong) obj->add (spMetadataInt);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeMdById
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_MetadataStream_n_1removeMdById (JNIEnv *env, jclass, jlong self, jlong id)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jboolean) obj->remove ((long long)id);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSet
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSet (JNIEnv *env, jclass, jlong self, jlong mdSetAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    MetadataSet* mdSet = (MetadataSet*) mdSetAddr;
    obj->remove ((*mdSet));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    MetadataSchema* mdSchema = (MetadataSchema*) schemaAddr;
    std::shared_ptr <MetadataSchema> spSchema = std::make_shared <MetadataSchema>(mdSchema->getName(), mdSchema->getAuthor());
    obj->remove (spSchema);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_removeAllMd
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1removeAllMd (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    obj->remove ();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addSchema
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addSchema (JNIEnv *env, jclass, jlong self, jlong schemaAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    MetadataSchema* mdSchema = (MetadataSchema*) schemaAddr;
    std::shared_ptr <MetadataSchema> spSchema = std::make_shared <MetadataSchema>(mdSchema->getName(), mdSchema->getAuthor());
    obj->addSchema (spSchema);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getSchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getSchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    return (jlong) obj->getSchema (sName).get();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllSchemaNames
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllSchemaNames (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::vector <std::string> vecNames = obj->getAllSchemaNames();
    
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAll
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1getAll (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jlong) new MetadataSet (obj->getAll());
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
    MetadataStream* obj = (MetadataStream*) self;
    MetadataStream* srcStream = (MetadataStream*) srcStreamAddr;
    MetadataSet* srcSet = (MetadataSet*) srcSetAddr;
    MetadataSet* setFalure = (MetadataSet*) setFalureAddr;
    
    return (jboolean) obj->import (*srcStream, *srcSet, (long long)dstFrameIndex, (long long)srcFrameIndex, (long long)numOfFrames, setFalure);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_sortMdSetById
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1sortMdSetById (JNIEnv *, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    obj->sortById();
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_serialize
 * Signature: (JJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1serialize (JNIEnv *env, jclass, jlong self, jlong IWriterAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    IWriter* writer = (IWriter*) IWriterAddr;
    jstring str = env->NewStringUTF ((obj->serialize ((*writer))).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_deserialize
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1deserialize (JNIEnv *env, jclass, jlong self, jstring text, jlong IReaderAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    IReader* reader = (IReader*) IReaderAddr;
    std::string sText (env->GetStringUTFChars (text, NULL));
    obj->deserialize(sText, (*reader));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_computeChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__J (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    jstring str = env->NewStringUTF ((obj->computeChecksum()).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_computeChecksum
 * Signature: (JJJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1computeChecksum__JJJ (JNIEnv *env, jclass, jlong self, jlong XMPPacketSize, jlong XMPPacketOffset)
{
    MetadataStream* obj = (MetadataStream*) self;
    jstring str = env->NewStringUTF ((obj->computeChecksum((long long)XMPPacketSize, (long long) XMPPacketOffset)).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getChecksum
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_n_1getChecksum (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    jstring str = env->NewStringUTF ((obj->getChecksum()).c_str ());
    return str;
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_setChecksum
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1setChecksum (JNIEnv *env, jclass, jlong self, jstring checksum)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string str (env->GetStringUTFChars (checksum, NULL));
    obj->setChecksum (str);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_addVideoSegment
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1addVideoSegment (JNIEnv *env, jclass, jlong self, jlong segmentAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    MetadataStream::VideoSegment* segment = (MetadataStream::VideoSegment*)segmentAddr;
    long w = 0;
    long h = 0;
    segment->getResolution (w, h);
    std::shared_ptr <MetadataStream::VideoSegment> spSegment = std::make_shared <MetadataStream::VideoSegment>(segment->getTitle(), segment->getFPS(), segment->getTime(), segment->getDuration(), w, h);
    obj->addVideoSegment (spSegment);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_getAllVideoSegment
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataStream_n_1getAllVideoSegments (JNIEnv *env, jclass, jlong self)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::vector <std::shared_ptr<MetadataStream::VideoSegment>> segments;
    
    segments = obj->getAllVideoSegments();
    jlongArray nObjs = env->NewLongArray ((jsize)segments.size());
    
    jlong* body = env->GetLongArrayElements (nObjs, 0);
    int counter = 0;
    for (auto it = segments.begin(); it != segments.end(); it++, counter++)
        body [counter] = (jlong) (it->get()); 
        
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
    MetadataStream* obj = (MetadataStream*) self;
    jlong *body = env->GetLongArrayElements (frameIndexAndNumOfFrames, 0);
    obj->convertTimestampToFrameIndex ((long long)timestamp, (long long)duration, body[0], body[1]);
    env->ReleaseLongArrayElements(frameIndexAndNumOfFrames, body, 0);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_convertFrameIndexToTimestamp
 * Signature: (JJJJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1convertFrameIndexToTimestamp (JNIEnv *env, jclass, jlong self,
                                                                                          jlong frameIndex, jlong numOfFrames,
                                                                                          jlongArray timestampAndDuration)
{
    MetadataStream* obj = (MetadataStream*) self;
    jlong *body = env->GetLongArrayElements(timestampAndDuration, 0);
    obj->convertFrameIndexToTimestamp((long long)frameIndex, (long long)numOfFrames, body[0], body[1]);
    env->ReleaseLongArrayElements(timestampAndDuration, body, 0);
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByFrameIndex
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByFrameIndex (JNIEnv *env, jclass, jlong self, jlong id)
{
    MetadataStream* obj = (MetadataStream*) self;
    size_t index = (size_t) id;
    return (jlong) new MetadataSet (obj->queryByFrameIndex (index));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByTime
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByTime (JNIEnv *env, jclass, jlong self, jlong startTime, jlong endTime)
{
    MetadataStream* obj = (MetadataStream*) self;
    return (jlong) new MetadataSet (obj->queryByTime((long long) startTime, (long long) endTime));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryBySchema
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryBySchema (JNIEnv *env, jclass, jlong self, jstring schemaName)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (schemaName, NULL));
    return (jlong) new MetadataSet (obj->queryBySchema(sName));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByName (JNIEnv *env, jclass, jlong self, jstring name)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new MetadataSet (obj->queryByName(sName));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndValue
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndValue (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                  jlong fieldValueAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    FieldValue* value = (FieldValue*) fieldValueAddr;
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    return (jlong) new MetadataSet (obj->queryByNameAndValue (sName, (*value)));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByNameAndFields
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByNameAndFields (JNIEnv *env, jclass, jlong self, jstring mdName,
                                                                                   jlongArray fieldValues)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::vector <FieldValue> values;
    
    std::string sName (env->GetStringUTFChars (mdName, NULL));
    jlong *body =env->GetLongArrayElements (fieldValues, 0);
    jsize len = env->GetArrayLength (fieldValues);
    
    for (int i = 0; i < len; i++)
    {
        FieldValue* addr = (FieldValue*) body[i];
        values.push_back (*addr);
    }
    
    env->ReleaseLongArrayElements (fieldValues, body, 0);
    return (jlong) new MetadataSet (obj->queryByNameAndFields (sName, values));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring name)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new MetadataSet (obj->queryByReference (sName));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                      jlong valueAddr)
{
    MetadataStream* obj = (MetadataStream*) self;
    FieldValue* value = (FieldValue*) valueAddr;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new MetadataSet (obj->queryByReference (sName, *value));
}

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_queryByReference
 * Signature: (JLjava/lang/String;[J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_n_1queryByReference__JLjava_lang_String_2_3J (JNIEnv *env, jclass, jlong self, jstring name,
                                                                                                        jlongArray fieldValues)
{
    MetadataStream* obj = (MetadataStream*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
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

/*
 * Class:     com_intel_vmf_MetadataStream
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_n_1delete (JNIEnv *env, jclass, jlong self)
{
    delete (MetadataStream*) self;
}
