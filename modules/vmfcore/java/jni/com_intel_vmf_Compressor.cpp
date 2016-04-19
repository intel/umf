#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/compressor.hpp"
#include "throwJavaException.hpp"

static std::string getJavaStirng(JNIEnv *env, jstring str)
{
    const char* tmp = env->GetStringUTFChars(str, NULL);
    std::string result(tmp);
    env->ReleaseStringUTFChars(str, tmp);
    return result;
}

extern "C" {

using namespace vmf;

//n_delete(long nativeObj);
JNIEXPORT void JNICALL Java_com_intel_vmf_Compressor_n_1delete(JNIEnv *env, jclass, jlong self);
JNIEXPORT void JNICALL Java_com_intel_vmf_Compressor_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Compressor::n_1delete";

    try
    {
        std::shared_ptr<Compressor>* obj = (std::shared_ptr<Compressor>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Compressor (self) is null pointer.");
        delete obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

//long n_create(String str);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1create(JNIEnv *env, jclass, jstring id);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1create(JNIEnv *env, jclass, jstring id)
{
    static const char method_name[] = "Compressor::n_1create";

    try
    {
        if (id == 0) VMF_EXCEPTION(NullPointerException, "Compressor ID can't be null.");
        return (jlong) new std::shared_ptr<Compressor>(Compressor::create(getJavaStirng(env, id)));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getId(long self);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Compressor_n_1getId(JNIEnv *env, jclass, jlong self);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Compressor_n_1getId(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Compressor::n_1getId";

    try
    {
        std::shared_ptr<Compressor>* obj = (std::shared_ptr<Compressor>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Compressor (self) is null pointer.");
        return env->NewStringUTF((*obj)->getId().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_createNewInstance(long self);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1createNewInstance(JNIEnv *env, jclass, jlong self);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1createNewInstance(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Compressor::n_1createNewInstance";

    try
    {
        std::shared_ptr<Compressor>* obj = (std::shared_ptr<Compressor>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Compressor (self) is null pointer.");
        return (jlong) new std::shared_ptr<Compressor>( (*obj)->createNewInstance() );
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_compress(long self, String str);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1compress(JNIEnv *env, jclass, jlong self, jstring str);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Compressor_n_1compress(JNIEnv *env, jclass, jlong self, jstring str)
{
    static const char method_name[] = "Compressor::n_1compress";

    try
    {
        std::shared_ptr<Compressor>* obj = (std::shared_ptr<Compressor>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Compressor (self) is null pointer.");
        if (str == 0) VMF_EXCEPTION(NullPointerException, "String to compress can't be null.");
        vmf_rawbuffer rbuf;
        (*obj)->compress(getJavaStirng(env, str), rbuf);
        return (jlong) new std::shared_ptr<Variant>( new Variant( rbuf ) );
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_decompress(long self, long variantAddr);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Compressor_n_1decompress(JNIEnv *env, jclass, jlong self, jlong variantAddr);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Compressor_n_1decompress(JNIEnv *env, jclass, jlong self, jlong variantAddr)
{
    static const char method_name[] = "Compressor::n_1decompress";

    try
    {
        std::shared_ptr<Compressor>* obj = (std::shared_ptr<Compressor>*) self;
        std::shared_ptr<Variant>* var = (std::shared_ptr<Variant>*) variantAddr;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Compressor (self) is null pointer.");
        if (var == NULL || *var == NULL)  VMF_EXCEPTION(NullPointerException, "Input data is null pointer.");
        std::string text;
        (*obj)->decompress((*var)->get_rawbuffer(), text);
        return env->NewStringUTF(text.c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

}
