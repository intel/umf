#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_VideoSegment
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__ (JNIEnv *env, jclass)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1VideoSegment__";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = new std::shared_ptr<MetadataStream::VideoSegment>(new MetadataStream::VideoSegment());
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
* Class:     com_intel_vmf_MetadataStream_VideoSegment
* Method:    n_VideoSegment
* Signature: (Ljava/lang/String;DJJII)J
*/
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__Ljava_lang_String_2DJJII (JNIEnv *env, jclass, jstring title, jdouble fps, jlong startTime, jlong duration, jint width, jint height);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1VideoSegment__Ljava_lang_String_2DJJII (JNIEnv *env, jclass, jstring title, jdouble fps,
                                                                                                                       jlong startTime, jlong duration, jint width, jint height)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1VideoSegment__Ljava_lang_String_2DJJII";

    try
    {
        const char* tmp = env->GetStringUTFChars(title, NULL);
        std::string sTitle(tmp);
        env->ReleaseStringUTFChars(title, tmp);

        std::shared_ptr<MetadataStream::VideoSegment>* obj = new std::shared_ptr<MetadataStream::VideoSegment>(new MetadataStream::VideoSegment(sTitle, (double)fps, (long long)startTime, (long long)duration, (long)width, (long)height));
        
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
 * Class:     com_intel_vmf_MetadataStream_VideoSegment
 * Method:    n_getTitle
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTitle (JNIEnv *env, jclass, jlong self);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTitle (JNIEnv *env, jclass, jlong self)
{

    static const char method_name[] = "MetadataStream::VideoSegment::n_1getTitle";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTitle (JNIEnv *env, jclass, jlong self, jstring title);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTitle (JNIEnv *env, jclass, jlong self, jstring title)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setTitle";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;


        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

        const char* tmp = env->GetStringUTFChars(title, NULL);
        std::string sTitle(tmp);
        (*obj)->setTitle (sTitle);
        env->ReleaseStringUTFChars(title, tmp);
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
JNIEXPORT jdouble JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getFPS(JNIEnv *env, jclass, jlong self);


JNIEXPORT jdouble JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getFPS (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getFPS";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self; 

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setFPS(JNIEnv *env, jclass, jlong self, jdouble fps);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setFPS(JNIEnv *env, jclass, jlong self, jdouble fps)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setFPS";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getDuration(JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getDuration (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getDuration";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setDuration(JNIEnv *env, jclass, jlong self, jlong duration);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setDuration(JNIEnv *env, jclass, jlong self, jlong duration)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setDuration";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

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
JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTime (JNIEnv *env, jclass, jlong self);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getTime (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getTime";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTime(JNIEnv *env, jclass, jlong self, jlong startTime);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setTime(JNIEnv *env, jclass, jlong self, jlong startTime)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setTime";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

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
* Method:    n_getWidth
* Signature: (J)I
*/
JNIEXPORT jint JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getWidth(JNIEnv *env, jclass, jlong self);


JNIEXPORT jint JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getWidth(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getWidth";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long w = 0, h = 0;

        (*obj)->getResolution(w, h);
        return (jint) w;
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
* Method:    n_getHeight
* Signature: (J)I
*/
JNIEXPORT jint JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getHeight(JNIEnv *env, jclass, jlong self);


JNIEXPORT jint JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1getHeight (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1getHeight";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            return 0;

        long w = 0, h = 0;

        (*obj)->getResolution(w, h);
        return (jint) h;
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
* Method:    n_setResolution
* Signature: (JII)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setResolution(JNIEnv *env, jclass, jlong self, jint w, jint h);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1setResolution (JNIEnv *env, jclass, jlong self, jint w, jint h)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1setResolution";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

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
JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1delete(JNIEnv *env, jclass, jlong self);


JNIEXPORT void JNICALL Java_com_intel_vmf_MetadataStream_00024VideoSegment_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::VideoSegment::n_1delete";

    try
    {
        std::shared_ptr<MetadataStream::VideoSegment>* obj = (std::shared_ptr<MetadataStream::VideoSegment>*) self;

        if ((obj == NULL) || (obj->get() == NULL))
            VMF_EXCEPTION(NullPointerException, "Segment is null pointer.");

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
