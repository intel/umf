#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_Reference.h"

using namespace vmf;

/// throw java exception
static void throwJavaException(JNIEnv *env, const std::exception *e, const char *method) {
    std::string what = "unknown exception";
    jclass je = 0;

    if (e) {
        std::string exception_type = "std::exception";

        if (dynamic_cast<const Exception*>(e)) {
            exception_type = "vmf::Exception";
            //je = env->FindClass("org/opencv/core/CvException");
        }

        what = exception_type + ": " + e->what();
    }

    if (!je) je = env->FindClass("java/lang/Exception");
    env->ThrowNew(je, what.c_str());

    //LOGE("%s caught %s", method, what.c_str());
    (void)method;        // avoid "unused" warning
}

/*
 * Class:     com_intel_vmf_Reference
 * Method:    n_Reference
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1Reference__ (JNIEnv *env, jclass)
{
    std::shared_ptr<Reference>* p = new std::shared_ptr<Reference>(new Reference ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_Reference
 * Method:    n_Reference
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1Reference__JJ (JNIEnv *env, jclass, jlong refDescAddr, jlong mdAddr)
{
    std::shared_ptr<ReferenceDesc>* refDesc = (std::shared_ptr<ReferenceDesc>*) refDescAddr;
    std::shared_ptr<Metadata>* md = (std::shared_ptr<Metadata>*) mdAddr;
    std::shared_ptr<Reference>* p = new std::shared_ptr<Reference>(new Reference ((*refDesc), (*md)));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_Reference
 * Method:    n_ReferenceWeak
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Reference_n_1ReferenceWeak (JNIEnv *env, jclass, jlong refDescAddr, jlong mdAddr)
{
    std::shared_ptr<ReferenceDesc>* refDesc = (std::shared_ptr<ReferenceDesc>*) refDescAddr;
    std::shared_ptr<Metadata>* md = (std::shared_ptr<Metadata>*) mdAddr;
    std::weak_ptr<Metadata>* mdWeak = new std::weak_ptr<Metadata>(*md);
    std::shared_ptr<Reference>* p = new std::shared_ptr<Reference>(new Reference ((*refDesc), (*mdWeak)));
    return (jlong) p;
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
        std::shared_ptr<Metadata>* md = new std::shared_ptr<Metadata>((*obj)->getReferenceMetadata());
        return (jlong) md;
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
        std::shared_ptr<ReferenceDesc>* refDesc = new std::shared_ptr<ReferenceDesc>((*obj)->getReferenceDescription ());
        return (jlong) refDesc;
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
        std::shared_ptr<Reference>* p = (std::shared_ptr<Reference>*) self;
        delete p;
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
