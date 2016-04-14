#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/statistics.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
* Class:     com_intel_vmf_StatField
* Method:    n_delete
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_StatField_n_1delete(JNIEnv *env, jclass, jlong self);

JNIEXPORT void JNICALL Java_com_intel_vmf_StatField_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1delete";

    try 
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        delete obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

//long n_StatField(String name, String schemaName, String metadataName, String fieldName, String opName);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_StatField_n_1StatField(JNIEnv *env, jclass,
    jstring name, jstring schemaName, jstring metadataName, jstring fieldName, jstring opName);

#define GET_JAVA_STR(x) const char* _ ## x = env->GetStringUTFChars(x, NULL); std::string __ ## x(_ ## x); env->ReleaseStringUTFChars(x, _ ## x)
JNIEXPORT jlong JNICALL Java_com_intel_vmf_StatField_n_1StatField(JNIEnv *env, jclass,
    jstring name, jstring schemaName, jstring metadataName, jstring fieldName, jstring opName)
{
    static const char method_name[] = "StatField::n_1StatField";

    try
    {
        GET_JAVA_STR(name);
        GET_JAVA_STR(schemaName);
        GET_JAVA_STR(metadataName);
        GET_JAVA_STR(fieldName);
        GET_JAVA_STR(opName);

        std::shared_ptr<StatField>* obj = new std::shared_ptr<StatField> (new StatField(__name, __schemaName, __metadataName, __fieldName, __opName));
        return (jlong)obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


//String n_getName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getName(JNIEnv *env, jclass, jlong self);

JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getName";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return env->NewStringUTF((*obj)->getName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getSchemaName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getSchemaName(JNIEnv *env, jclass, jlong self);

JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getSchemaName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getSchemaName";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return env->NewStringUTF((*obj)->getSchemaName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getMetadataName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getMetadataName(JNIEnv *env, jclass, jlong self);

JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getMetadataName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getMetadataName";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return env->NewStringUTF((*obj)->getMetadataName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getFieldName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getFieldName(JNIEnv *env, jclass, jlong self);

JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getFieldName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getFieldName";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return env->NewStringUTF((*obj)->getFieldName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getOpName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getOpName(JNIEnv *env, jclass, jlong self);

JNIEXPORT jstring JNICALL Java_com_intel_vmf_StatField_n_1getOpName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getOpName";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return env->NewStringUTF((*obj)->getOpName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_getValue(long nativeObj);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_StatField_n_1getValue(JNIEnv *env, jclass, jlong self);

JNIEXPORT jlong JNICALL Java_com_intel_vmf_StatField_n_1getValue(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "StatField::n_1getValue";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");
        return (jlong) new std::shared_ptr<Variant>(new Variant((*obj)->getValue()));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//boolean n_equals(long self, long other);
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_StatField_n_1equals(JNIEnv *env, jclass, jlong self, jlong other);

JNIEXPORT jboolean JNICALL Java_com_intel_vmf_StatField_n_1equals(JNIEnv *env, jclass, jlong self, jlong other)
{
    static const char method_name[] = "StatField::n_1equals";

    try
    {
        std::shared_ptr<StatField>* obj = (std::shared_ptr<StatField>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField (self) is null pointer.");

        std::shared_ptr<StatField>* otherObj = (std::shared_ptr<StatField>*) other;
        if (otherObj == NULL || *otherObj == NULL)  VMF_EXCEPTION(NullPointerException, "StatField to compare is null pointer.");

        return **obj == **otherObj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}
}
