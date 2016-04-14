#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"
#include <iostream>

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_MetadataSchema
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1MetadataSchema(JNIEnv *env, jclass, jstring name, jstring author);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1MetadataSchema (JNIEnv *env, jclass, jstring name, jstring author)
{
    static const char method_name[] = "MetadataSchema::n_1MetadataSchema";

    try
    {
        const char* cName = env->GetStringUTFChars(name, NULL);
        std::string sName(cName);
        const char* cAuthor = env->GetStringUTFChars(author, NULL);
        std::string sAuthor(cAuthor);

        std::shared_ptr<MetadataSchema>* obj = new std::shared_ptr<MetadataSchema>(new MetadataSchema(sName, sAuthor));

        env->ReleaseStringUTFChars(name, cName);
        env->ReleaseStringUTFChars(author, cAuthor);
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getName(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSchema::n_1getName";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::string str = (*obj)->getName ();
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_getAuthor
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getAuthor(JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getAuthor (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSchema::n_1getAuthor";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::string str = (*obj)->getAuthor ();
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_size
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1size(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1size (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSchema::n_1size";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Schema is null pointer.");

        return (jlong) (*obj)->size();
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_add
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSchema_n_1add(JNIEnv *env, jclass, jlong self, jlong mdDescAddr);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSchema_n_1add (JNIEnv *env, jclass, jlong self, jlong mdDescAddr)
{
    static const char method_name[] = "MetadataSchema::n_1add";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;
        std::shared_ptr<MetadataDesc>* mdDesc = (std::shared_ptr<MetadataDesc>*) mdDescAddr;
        
        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Schema is null pointer.");

        if ((mdDesc == NULL) || (mdDesc->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata description is null pointer.");

        (*obj)->add(*mdDesc);
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

/*
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_findMetadataDesc
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1findMetadataDesc(JNIEnv *env, jclass, jlong self, jstring mdName);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1findMetadataDesc (JNIEnv *env, jclass, jlong self, jstring mdName)
{
    static const char method_name[] = "MetadataSchema::n_1findMetadataDesc";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        const char* tmp = env->GetStringUTFChars(mdName, NULL);
        std::string sName(tmp);
        env->ReleaseStringUTFChars(mdName, tmp);

        const std::shared_ptr<MetadataDesc> spMdDesc = (*obj)->findMetadataDesc (sName);

        if (spMdDesc == NULL)
            return 0;

        return (jlong) new std::shared_ptr<MetadataDesc>(spMdDesc);
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_getAll
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataSchema_n_1getAll(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_MetadataSchema_n_1getAll (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSchema::n_1getAll";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::vector<std::shared_ptr<MetadataDesc>> spMdDescVec = (*obj)->getAll ();
        jlongArray nObjs = env->NewLongArray ((jsize)spMdDescVec.size());
        jlong* body = env->GetLongArrayElements (nObjs, 0);
        
        for (std::size_t i = 0; i < spMdDescVec.size(); i++)
        {
            body[i] = (jlong) new std::shared_ptr<MetadataDesc> (spMdDescVec[i]);
        }
        
        env->ReleaseLongArrayElements (nObjs, body, 0);
        return nObjs;
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_getStdSchemaName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getStdSchemaName(JNIEnv *env, jclass, jint kind);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataSchema_n_1getStdSchemaName (JNIEnv *env, jclass, jint kind)
{
    static const char method_name[] = "MetadataSchema::n_1getStdSchemaName";
    
    try 
    {
        std::string str = MetadataSchema::getStdSchemaName ((MetadataSchema::StdSchemaKind) kind);
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_getStdSchema
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1getStdSchema(JNIEnv *env, jclass, jint kind);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataSchema_n_1getStdSchema (JNIEnv *env, jclass, jint kind)
{
    static const char method_name[] = "MetadataSchema::n_1getStdSchema";
    
    try 
    {
        return (jlong) new std::shared_ptr<MetadataSchema> (MetadataSchema::getStdSchema ((MetadataSchema::StdSchemaKind) kind));
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
 * Class:     com_intel_vmf_MetadataSchema
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSchema_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataSchema_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataSchema::n_1delete";
    
    try 
    {
        std::shared_ptr<MetadataSchema>* obj = (std::shared_ptr<MetadataSchema>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Schema is null pointer.");

        delete obj;
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


}
