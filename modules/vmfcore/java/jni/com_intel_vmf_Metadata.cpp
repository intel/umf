#include "../com_intel_vmf_Metadata.h"

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_Metadata
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1Metadata (JNIEnv *, jobject, jlong mdDescAddr)
{
    MetadataDesc* addr = (MetadataDesc*) mdDescAddr;
    const std::shared_ptr <MetadataDesc>  spDesc = std::make_shared <MetadataDesc>(*addr);
    
    return (jlong) new Metadata (spDesc);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_MetadataCopy
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1MetadataCopy (JNIEnv *, jclass, jlong other)
{
    Metadata* otherAddr = (Metadata*) other;
    return (jlong) new Metadata (*otherAddr);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getID
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getID (JNIEnv *, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getId();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFrameIndex
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFrameIndex (JNIEnv *, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getFrameIndex ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getNumOfFrames
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getNumOfFrames (JNIEnv *, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getNumOfFrames ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setFrameIndex
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFrameIndex (JNIEnv *, jclass, jlong self, jlong frameIndex, jlong numOfFrames)
{
    Metadata* obj = (Metadata*) self;
    obj->setFrameIndex ((long long) frameIndex, (long long) numOfFrames);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setTimestamp
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setTimestamp (JNIEnv *, jclass, jlong self, jlong time, jlong duration)
{
    Metadata* obj = (Metadata*) self;
    obj->setTimestamp((long long) time, (long long) duration);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getTime
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getTime (JNIEnv *, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getTime();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getDuration
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDuration (JNIEnv *, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getDuration();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getName (JNIEnv *, jclass, jlong)
{
    
}