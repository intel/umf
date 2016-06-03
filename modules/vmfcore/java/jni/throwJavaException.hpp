#include <memory>
#include <stdexcept>
#include <jni.h>

void throwJavaException (JNIEnv *env, const std::exception *e, const char *method);

