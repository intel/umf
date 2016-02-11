#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_Reference
 * Method:    n_Reference
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1Reference__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1Reference__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "Reference::n_1Reference__";

    try
    {
        std::shared_ptr<Reference>* obj = new std::shared_ptr<Reference>(new Reference());
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
 * Class:     com_intel_vmf_Reference
 * Method:    n_Reference
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1Reference__JJ (JNIEnv *env, jclass, jlong refDescAddr, jlong mdAddr)
{
    static const char method_name[] = "Reference::n_1Reference__JJ";

    try
    {
        std::shared_ptr<ReferenceDesc>* refDesc = (std::shared_ptr<ReferenceDesc>*) refDescAddr;
        std::shared_ptr<Metadata>* md = (std::shared_ptr<Metadata>*) mdAddr;

        if ((refDesc == NULL) || (refDesc->get() == NULL))
            return 0;

        if ((md == NULL) || (md->get() == NULL))
            return 0;

        std::shared_ptr<Reference>* obj = new std::shared_ptr<Reference>(new Reference((*refDesc), (*md)));
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
 * Class:     com_intel_vmf_Reference
 * Method:    n_getMetadata
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1getMetadata (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Reference::n_1getMetadata";
    
    try 
    {
        std::shared_ptr<Reference>* obj = (std::shared_ptr<Reference>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::weak_ptr<Metadata> md = (*obj)->getReferenceMetadata();

        if (md.lock() == NULL)
            return 0;

        std::shared_ptr<Metadata>* retVal = new std::shared_ptr<Metadata>(md);
        return (jlong) retVal;
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
 * Class:     com_intel_vmf_Reference
 * Method:    n_setMetadata
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Reference_n_1setMetadata (JNIEnv *env, jclass, jlong self, jlong mdAddr)
{
    static const char method_name[] = "Reference::n_1setMetadata";
    
    try 
    {
        std::shared_ptr<Reference>* obj = (std::shared_ptr<Reference>*) self;
        std::shared_ptr<Metadata>* md = (std::shared_ptr<Metadata>*) mdAddr;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Reference is null pointer.");

        if ((md == NULL) || (md->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Metadata is null pointer.");

        (*obj)->setReferenceMetadata ((*md));
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
 * Class:     com_intel_vmf_Reference
 * Method:    n_getReferenceDescription
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1getReferenceDescription (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Reference::n_1getReferenceDescription";
    
    try 
    {
        std::shared_ptr<Reference>* obj = (std::shared_ptr<Reference>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        std::shared_ptr<ReferenceDesc> refDesc = (*obj)->getReferenceDescription();

        if (refDesc == NULL)
            return 0;

        std::shared_ptr<ReferenceDesc>* retVal = new std::shared_ptr<ReferenceDesc>(refDesc);
        return (jlong) retVal;
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
 * Class:     com_intel_vmf_Reference
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Reference_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Reference::n_1delete";
    
    try 
    {
        std::shared_ptr<Reference>* obj = (std::shared_ptr<Reference>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Reference is null pointer.");

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
