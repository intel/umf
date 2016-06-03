#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/encryptor_default.hpp"
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

//long n_encrypt(long self, String str);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_EncryptorDefault_n_1encrypt(JNIEnv *env, jclass, jlong self, jstring str);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_EncryptorDefault_n_1encrypt(JNIEnv *env, jclass, jlong self, jstring str)
{
    static const char method_name[] = "EncryptorDefault::n_1encrypt";

    try
    {
        std::shared_ptr<EncryptorDefault>* obj = (std::shared_ptr<EncryptorDefault>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "EncryptorDefault (self) is null pointer.");
        if (str == 0) VMF_EXCEPTION(NullPointerException, "String to compress can't be null.");
        vmf_rawbuffer rbuf;
        (*obj)->encrypt(getJavaStirng(env, str), rbuf);
        return (jlong) new std::shared_ptr<Variant>(new Variant(rbuf));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_decrypt(long self, long variantAddr);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_EncryptorDefault_n_1decrypt(JNIEnv *env, jclass, jlong self, jlong variantAddr);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_EncryptorDefault_n_1decrypt(JNIEnv *env, jclass, jlong self, jlong variantAddr)
{
    static const char method_name[] = "EncryptorDefault::n_1decrypt";

    try
    {
        std::shared_ptr<EncryptorDefault>* obj = (std::shared_ptr<EncryptorDefault>*) self;
        std::shared_ptr<Variant>* var = (std::shared_ptr<Variant>*) variantAddr;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "EncryptorDefault (self) is null pointer.");
        if (var == NULL || *var == NULL)  VMF_EXCEPTION(NullPointerException, "Input data is null pointer.");
        std::string text;
        (*obj)->decrypt((*var)->get_rawbuffer(), text);
        return env->NewStringUTF(text.c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getHint(long self);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_EncryptorDefault_n_1getHint(JNIEnv *env, jclass, jlong self);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_EncryptorDefault_n_1getHint(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "EncryptorDefault::n_1getHint";

    try
    {
        std::shared_ptr<EncryptorDefault>* obj = (std::shared_ptr<EncryptorDefault>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "EncryptorDefault (self) is null pointer.");
        return env->NewStringUTF((*obj)->getHint().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_EncryptorDefault(passphrase);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_EncryptorDefault_n_1EncryptorDefault(JNIEnv *env, jclass, jstring passphrase);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_EncryptorDefault_n_1EncryptorDefault(JNIEnv *env, jclass, jstring passphrase)
{
    static const char method_name[] = "EncryptorDefault::n_1EncryptorDefault";

    try
    {
        if (passphrase == 0) VMF_EXCEPTION(NullPointerException, "EncryptorDefault passphrase can't be null.");
        return (jlong) new std::shared_ptr<EncryptorDefault>(new EncryptorDefault(getJavaStirng(env, passphrase)));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


}
