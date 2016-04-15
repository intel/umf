#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/encryptor.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

//n_delete(long nativeObj);
JNIEXPORT void JNICALL Java_com_intel_vmf_Encryptor_n_1delete(JNIEnv *env, jclass, jlong self);
JNIEXPORT void JNICALL Java_com_intel_vmf_Encryptor_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Encryptor::n_1delete";

    try
    {
        std::shared_ptr<Encryptor>* obj = (std::shared_ptr<Encryptor>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Encryptor (self) is null pointer.");
        delete obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

}
