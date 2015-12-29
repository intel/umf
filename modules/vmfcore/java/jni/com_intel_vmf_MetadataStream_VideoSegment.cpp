#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_MetadataStream_VideoSegment.h"

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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_VideoSegment
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__ (JNIEnv *env, jclass)
{
    std::shared_ptr<MetadataStream::VideoSegment>* p = new std::shared_ptr<MetadataStream::VideoSegment>(new MetadataStream::VideoSegment ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_VideoSegment
 * Signature: (Ljava/lang/String;DJJJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__Ljava_lang_String_2DJJJJ (JNIEnv *env, jclass, jstring title, jdouble fps,
                                                                                                                       jlong startTime, jlong duration, jint width, jint height)
{
    std::string sTitle (env->GetStringUTFChars(title, NULL));
    std::shared_ptr<MetadataStream::VideoSegment>* p = new std::shared_ptr<MetadataStream::VideoSegment>(new MetadataStream::VideoSegment(sTitle, (double)fps, (long long)startTime, (long long)duration, (long)width, (long)height));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_getTitle
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTitle (JNIEnv *env, jclass, jlong self)
{

    static const char method_name[] = "MetadataStream::VideoSegment::n_1getTitle";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        std::string str = (*obj)->getTitle();
        return env->NewStringUTF(str.c_str());
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_setTitle
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTitle (JNIEnv *env, jclass, jlong self, jstring title)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setTitle";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        std::string sTitle(env->GetStringUTFChars (title, NULL));
        (*obj)->setTitle (sTitle);
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_getFPS
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getFPS (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getFPS";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self; 
        return (jdouble) (*obj)->getFPS();
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_setFPS
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setFPS(JNIEnv *env, jclass, jlong self, jdouble fps)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setFPS";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        (*obj)->setFPS ((jdouble)fps);
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_getDuration
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getDuration (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getDuration";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        return (jlong)(*obj)->getDuration();
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_setDuration
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setDuration(JNIEnv *env, jclass, jlong self, jlong duration)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setDuration";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        (*obj)->setDuration((long long) duration);
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_getTime
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTime (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getTime";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        return (jlong) (*obj)->getTime();
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_setTime
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTime(JNIEnv *env, jclass, jlong self, jlong startTime)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setTime";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        (*obj)->setTime((long long) startTime);
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
* Class:     com_intel_vmf_MetadataStream_VideoSegment
* Method:    n_getResolution
* Signature: (J[J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getResolution (JNIEnv *env, jclass, jlong self, jintArray resolutionArray)
{

    static const char method_name[] = "MetadataStream::VideoSegment::n_1getResolution";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        jint* cArray = env->GetIntArrayElements (resolutionArray, 0);
        jsize len = env->GetArrayLength (resolutionArray);
        
        if (len == 2)
            (*obj)->getResolution ((long&)cArray[0], (long&)cArray[1]);
        else
            throwJavaException (env, 0, method_name);
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
* Class:     com_intel_vmf_MetadataStream_VideoSegment
* Method:    n_setResolution
* Signature: (JII)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setResolution (JNIEnv *env, jclass, jlong self, jint w, jint h)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setResolution";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
        (*obj)->setResolution ((long) w, (long) h);
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1delete";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* p = (std::shared_ptr<MetadataStream::VideoSegment>*) self;
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

