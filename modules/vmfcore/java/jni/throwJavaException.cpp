#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "umf/metadatastream.hpp"
#include "throwJavaException.hpp"

using namespace umf;

void throwJavaException (JNIEnv *env, const std::exception *e, const char *method)
{
    std::string what = "unknown exception";
    jclass je = 0;

    if (e)
    {
        std::string exception_type = "std::exception";

        if (dynamic_cast<const Exception*>(e))
        {
            exception_type = "umf::Exception";
            je = env->FindClass("com/intel/umf/Exception");
        }

        what = exception_type + ": " + e->what();
    }

    if (!je)
        je = env->FindClass("java/lang/Exception");

    env->ThrowNew(je, what.c_str());

    UMF_LOG_ERROR(what.c_str());
}