#include<string>
#include<vector>
#include <map>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_Metadata
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1Metadata(JNIEnv *env, jclass, jlong mdDescAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1Metadata (JNIEnv *env, jclass, jlong mdDescAddr)
{
    static const char method_name[] = "Metadata::n_1Metadata";

    try
    {
        std::shared_ptr <MetadataDesc>* desc = (std::shared_ptr <MetadataDesc>*) mdDescAddr;

        if (desc == NULL)
            return 0;

        return (jlong) new std::shared_ptr<Metadata>(new Metadata(*desc));
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_MetadataCopy
 * Signature: (J)J
 */
/*JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1MetadataCopy(JNIEnv *env, jclass, jlong otherAddr);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1MetadataCopy (JNIEnv *env, jclass, jlong otherAddr)
{
    static const char method_name[] = "Metadata::n_1MetadataCopy";

    try
    {
        std::shared_ptr <Metadata>* other = (std::shared_ptr <Metadata>*) otherAddr;

        if ((other == NULL) || (other->get() == NULL))
            return 0;

        return (jlong) new std::shared_ptr<Metadata>(new Metadata(**other));
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
}*/

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getID
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getID(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getID (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getID";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong) (*obj)->getId();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFrameIndex
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFrameIndex(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFrameIndex (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getFrameIndex";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong)(*obj)->getFrameIndex();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getNumOfFrames
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getNumOfFrames(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getNumOfFrames (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getNumOfFrames";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong) (*obj)->getNumOfFrames();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setFrameIndex
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFrameIndex(JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFrameIndex (JNIEnv *env, jclass, jlong self, jlong frameIndex, jlong numOfFrames)
{
    static const char method_name[] = "Metadata::n_1setFrameIndex";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        (*obj)->setFrameIndex ((long long)frameIndex, (long long)numOfFrames);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setTimestamp
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setTimestamp(JNIEnv *env, jclass, jlong self, jlong time, jlong duration);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setTimestamp (JNIEnv *env, jclass, jlong self, jlong time, jlong duration)
{
    static const char method_name[] = "Metadata::n_1setTimestamp";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        (*obj)->setTimestamp ((long long)time, (long long)duration);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getTime
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getTime(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getTime (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getTime";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong)(*obj)->getTime();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getDuration
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDuration(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDuration (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getDuration";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (jlong) (*obj)->getDuration();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getName";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (env->NewStringUTF(((*obj)->getName()).c_str()));
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getSchemaName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getSchemaName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_Metadata_n_1getSchemaName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getSchemaName";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        return (env->NewStringUTF(((*obj)->getSchemaName()).c_str()));
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getDesc
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDesc(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getDesc (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getDesc";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::shared_ptr <MetadataDesc> mdDesc = (*obj)->getDesc();

        if (mdDesc == NULL)
            return 0;

        std::shared_ptr <MetadataDesc>* retVal = new std::shared_ptr <MetadataDesc>(mdDesc);
        return (jlong) retVal;
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFieldNames
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Metadata_n_1getFieldNames(JNIEnv *env, jclass, jlong self);


JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Metadata_n_1getFieldNames (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getFieldNames";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector<std::string> vecNames = (*obj)->getFieldNames();

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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFieldValue
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFieldValue(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFieldValue (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1getFieldValue";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
        return (jlong) new std::shared_ptr <Variant>(new Variant((*obj)->getFieldValue (sName)));
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_hasField
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1hasField(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1hasField (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1hasField";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
        return (jboolean) (*obj)->hasField (sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1equals(JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1equals (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "Metadata::n_1equals";

    try
    {
        std::shared_ptr <Metadata>* self = (std::shared_ptr <Metadata>*)selfAddr;
        std::shared_ptr <Metadata>* other = (std::shared_ptr <Metadata>*)otherAddr;

        if ((self == NULL) || (self->get() == NULL))
            return JNI_FALSE;

        if ((other == NULL) || (other->get() == NULL))
            return JNI_FALSE;
    
        return (**self) == (**other) ? JNI_TRUE : JNI_FALSE;
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
* Class:     com_intel_vmf_Metadata
* Method:    n_findField
* Signature: (JLjava/lang/String;)J
*/
/*JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1findField (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1findField";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;
        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
       
        return (jlong) new std::shared_ptr <FieldValue>(new FieldValue(*((*obj)->findField(sName))));
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
}*/

/*
 * Class:     com_intel_vmf_Metadata
 * Method:    n_lessThan
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1lessThan(JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1lessThan (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "Metadata::n_1lessThan";

    try
    {
        std::shared_ptr <Metadata>* self = (std::shared_ptr <Metadata>*)selfAddr;
        std::shared_ptr <Metadata>* other = (std::shared_ptr <Metadata>*)otherAddr;

        if ((self == NULL) || (self->get() == NULL))
            return JNI_FALSE;

        if ((other == NULL) || (other->get() == NULL))
            return JNI_FALSE;
    
        return (**self) < (**other) ? JNI_TRUE : JNI_FALSE; 
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getFirstReference
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFirstReference(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getFirstReference (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1getFirstReference";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);

        std::shared_ptr <Metadata> md = (*obj)->getFirstReference(sName);

        if (md == NULL)
            return 0;
            
        return (jlong) new std::shared_ptr <Metadata>(md);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getReferencesByMetadata
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByMetadata(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByMetadata (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1getReferencesByMetadata";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);
        env->ReleaseStringUTFChars(name, str);
        
        std::shared_ptr <MetadataSet>* mdSet = new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->getReferencesByMetadata (sName)));
        return (jlong) mdSet;
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getReferencesByName
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByName(JNIEnv *env, jclass, jlong self, jstring name);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Metadata_n_1getReferencesByName (JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Metadata::n_1getReferencesByName";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
        return (jlong) new std::shared_ptr <MetadataSet>(new MetadataSet((*obj)->getReferencesByName(sName)));
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_getAllReferences
 * Signature: (J)[Lcom/intel/vmf/Reference;
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_Metadata_n_1getAllReferences(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_Metadata_n_1getAllReferences (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1getAllReferences";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector<Reference> refs = (*obj)->getAllReferences();
    
        jlongArray nObjs = env->NewLongArray((jsize)refs.size());
        jlong* cArray = env->GetLongArrayElements(nObjs, 0);

        for (std::size_t i = 0; i < refs.size(); i++)
        {
            std::shared_ptr<ReferenceDesc> rd = refs[i].getReferenceDescription();
            std::weak_ptr<Metadata> md = refs[i].getReferenceMetadata();
            cArray[i] = (jlong) new std::shared_ptr<Reference>(new Reference(rd, md));
        }
   
        env->ReleaseLongArrayElements(nObjs, cArray, 0);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isReferenceById
 * Signature: (JJLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceById(JNIEnv *env, jclass, jlong self, jlong id, jstring name);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceById (JNIEnv *env, jclass, jlong self, jlong id, jstring name)
{
    static const char method_name[] = "Metadata::n_1isReferenceById";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);

        return (jboolean) (*obj)->isReference ((IdType)id, sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isReferenceByMd
 * Signature: (JJLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceByMd(JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isReferenceByMd (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    static const char method_name[] = "Metadata::n_1isReferenceByMd";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;
        std::shared_ptr <Metadata>* md = (std::shared_ptr <Metadata>*) mdAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        if ((md == NULL) || (md->get() == NULL))
            return JNI_FALSE;

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
    
        return (jboolean) (*obj)->isReference (*md, sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_addReference
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addReference(JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addReference (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    static const char method_name[] = "Metadata::n_1addReference";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;
        std::shared_ptr <Metadata>* md = (std::shared_ptr <Metadata>*) mdAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        if ((md == NULL) || (md->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Referenced metadata is null pointer.");

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
    
        (*obj)->addReference (*md, sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_removeReferenceById
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceById(JNIEnv *env, jclass, jlong self, jlong id, jstring name);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceById (JNIEnv *env, jclass, jlong self, jlong id, jstring name)
{
    static const char method_name[] = "Metadata::n_1removeReferenceById";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
        (*obj)->removeReference((IdType) id, sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_removeReferenceByMd
 * Signature: (JJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceByMd(JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1removeReferenceByMd (JNIEnv *env, jclass, jlong self, jlong mdAddr, jstring name)
{
    static const char method_name[] = "Metadata::n_1removeReferenceByMd";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;
        std::shared_ptr <Metadata>* md = (std::shared_ptr <Metadata>*) mdAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        if ((md == NULL) || (md->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Referenced metadata is null pointer.");

        const char* str = env->GetStringUTFChars(name, NULL);
        std::string sName(str);

        env->ReleaseStringUTFChars(name, str);
    
        (*obj)->removeReference(*md, sName);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_setFieldValue
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFieldValue(JNIEnv *env, jclass, jlong self, jstring fieldName, jlong variantAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1setFieldValue (JNIEnv *env, jclass, jlong self, jstring fieldName, jlong variantAddr)
{
    static const char method_name[] = "Metadata::n_1setFieldValue";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;
        std::shared_ptr <Variant>* variant = (std::shared_ptr <Variant>*)variantAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        if ((variant == NULL) || (variant->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        const char* str = env->GetStringUTFChars(fieldName, NULL);
        std::string sName(str);
        
        (*obj)->setFieldValue (sName, (**variant));
        env->ReleaseStringUTFChars(fieldName, str);
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_addValue
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addValue(JNIEnv *env, jclass, jlong self, jlong variantAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1addValue (JNIEnv *env, jclass, jlong self, jlong variantAddr)
{
    static const char method_name[] = "Metadata::n_1addValue";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;
        std::shared_ptr <Variant>* variant = (std::shared_ptr <Variant>*) variantAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        if ((variant == NULL) || (variant->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Variant is null pointer.");

        (*obj)->addValue((**variant)); 
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_validate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1validate(JNIEnv *env, jclass, jlong self);

JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1validate (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1validate";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        (*obj)->validate ();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_isValid
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isValid(JNIEnv *env, jclass, jlong self);


JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Metadata_n_1isValid (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1isValid";

    try
    {
        std::shared_ptr <Metadata>* obj = (std::shared_ptr <Metadata>*)self;

        if ((obj == NULL) || (obj->get() == NULL))
            return JNI_FALSE;

        return (jboolean) (*obj)->isValid ();
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
 * Class:     com_intel_vmf_Metadata
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_Metadata_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Metadata::n_1delete";

    try
    {
        std::shared_ptr<Metadata>* obj = (std::shared_ptr<Metadata>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

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
