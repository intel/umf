#include<string>
#include<vector>
#include "../com_intel_vmf_Metadata.h"

using namespace vmf;
/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_Metadata
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1Metadata (JNIEnv *env, jclass, jlong mdDescAddr)
{
    MetadataDesc* addr = (MetadataDesc*) mdDescAddr;
    const std::shared_ptr <MetadataDesc>  spDesc = std::make_shared <MetadataDesc>(addr->getMetadataName(), addr->getFields(), addr->getAllReferenceDescs());
    
    return (jlong) new Metadata (spDesc);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_MetadataCopy
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1MetadataCopy (JNIEnv *env, jclass, jlong other)
{
    Metadata* otherAddr = (Metadata*) other;
    return (jlong) new Metadata (*otherAddr);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getID
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getID (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getId();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFrameIndex
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFrameIndex (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getFrameIndex ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getNumOfFrames
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getNumOfFrames (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getNumOfFrames ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setFrameIndex
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFrameIndex (JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames)
{
    Metadata* obj = (Metadata*) self;
    obj->setFrameIndex ((long long) frameIndex, (long long) numOfFrames);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setTimestamp
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setTimestamp (JNIEnv *env, jclass, jlong self, jlong time, jlong duration)
{
    Metadata* obj = (Metadata*) self;
    obj->setTimestamp((long long) time, (long long) duration);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getTime
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getTime (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getTime();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getDuration
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDuration (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getDuration();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getName (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (env->NewStringUTF(env, (obj->getName()).c_str()));
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getSchemaName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getSchemaName (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (env->NewStringUTF(env, (obj->getSchemaName()).c_str()));
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getDesc
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDesc (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jlong) obj->getDesc().get();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFieldNames
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Metadata_n_1getFieldNames (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    std::vector<std::string> vecNames = obj->getFieldNames();
    
    jclass stringClass = env->FindClass(env, "java/lang/String");
    jobjectArray objArray = env->NewObjectArray(vecNames.size(), stringClass, NULL);
    
    int counter = 0;
    for (auto it = vecNames.begin(); it != vecNames.end(); it++, counter++)
    {
        jstring str = env->NewStringUTF(env, it->c_str());
        env->SetObjectArrayElement(env, objArray, counter, str);
    }
    
    return objArray;
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFieldValue
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFieldValue (JNIEnv *env, jclass, jlong self, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) new vmf::Variant(obj->getFieldValue (sName));
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_hasField
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1hasField (JNIEnv *env, jclass, jlong self, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jboolean) obj->hasField (sName);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1equals (JNIEnv *env, jclass, jlong self, jlong other)
{
    Metadata* selfAddr = (Metadata*) self;
    Metadata* otherAddr = (Metadata*) other;
    
    if (*selfAddr == *otherAddr)
        return JNI_TRUE;
    else
        return JNI_FALSE;
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_lessThan
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1lessThan (JNIEnv *env, jclass, jlong self, jlong other)
{
    Metadata* selfAddr = (Metadata*) self;
    Metadata* otherAddr = (Metadata*) other;
    
    if (*selfAddr < *otherAddr)
        return JNI_TRUE;
    else
        return JNI_FALSE;
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFirstReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFirstReference (JNIEnv *env, jclass, jlong self, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jlong) obj->getFirstReference (sName).get();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getReferencesByMetadata
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByMetadata (JNIEnv *env, jclass, jlong self, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    MetadataSet* mdSet = new MetadataSet (obj->getReferencesByMetadata (sName));
    return (jlong) mdSet;
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getReferencesByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByName (JNIEnv *env, jclass, jlong self, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    MetadataSet* mdSet = new MetadataSet (obj->getReferencesByName (sName));
    return (jlong) mdSet;
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getAllReferences
 * Signature: (J)[Lcom/intel/vmf/Reference;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Metadata_n_1getAllReferences (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    std::vector<vmf::Reference> refs = obj->getAllReferences();
    
    /*jclass refClass = env->FindClass(env, "com/intel/vmf/Reference");
    jobjectArray objArray = (jobjectArray)env->NewObjectArray (env, refs.size(), refClass, 0); 
     
    jmethodID constructor = env->GetMethodID (env, refClass, "<init>", "(J)V");
    int counter = 0;
    for (std::vector<vmf::Reference>::iterator it = refs.begin(); refs != refs.end(); it++, counter++)
    {
        vmf::Reference* vmfRef = new vmf::Reference (it->getReferenceDescription(), it->getReferenceMetadata ());
        jobject ref =  env->NewObject (env, refClass, constructor, (jlong)vmfRef);
        env->SetObjectArrayElement (env, objArray, counter, ref);
    }
    return objArray;*/
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isReferenceById
 * Signature: (JJLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceById (JNIEnv *env, jclass, jlong self, jlong id, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jboolean) obj->isReference ((long long)id, sName);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isReferenceByMd
 * Signature: (JJLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceByMd (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    Metadata* obj = (Metadata*) self;
    Metadata* md = (Metadata*) mdAddr;
    std::shared_ptr<Metadata> sp = std::make_shared <Metadata> (*md);
    std::string sName (env->GetStringUTFChars (name, NULL));
    return (jboolean) obj->isReference (sp, sName);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_addReference
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addReference (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    Metadata* md = (Metadata*) mdAddr;
    std::shared_ptr<Metadata> sp = std::make_shared <Metadata> (*md);
    obj->addReference (sp, sName);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_removeReferenceById
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceById (JNIEnv *env, jclass, jlong self, jlong id, jstring name)
{
    Metadata* obj = (Metadata*) self;
    std::string sName (env->GetStringUTFChars (name, NULL));
    obj->removeReference((long long) id, sName);
}
  
/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_removeReferenceByMd
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceByMd (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    Metadata* obj = (Metadata*) self;
    Metadata* md = (Metadata*) mdAddr;
    std::shared_ptr<Metadata> sp = std::make_shared <Metadata> (*md);
    std::string sName (env->GetStringUTFChars (name, NULL));
    
    obj->removeReference(sp, sName);
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setFieldValue
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFieldValue (JNIEnv *env, jclass, jlong self, jstring fieldName, jlong variantAddr)
{
    Metadata* obj = (Metadata*) self;
    Variant* variant = (Variant*) variantAddr;
    std::string sName (env->GetStringUTFChars (fieldName, NULL));
    obj->setFieldValue (sName, (*variant));
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_addValue
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addValue (JNIEnv *env, jclass, jlong self, jlong variantAddr)
{
    Metadata* obj = (Metadata*) self;
    Variant* variant = (Variant*) variantAddr;
    obj->addValue((*variant)); 
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_validate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1validate (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    obj->validate ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isValid
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isValid (JNIEnv *env, jclass, jlong self)
{
    Metadata* obj = (Metadata*) self;
    return (jboolean) obj->isValid ();
}

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1delete (JNIEnv *env, jclass, jlong self)
{
    delete (Metadata*) self;
}
