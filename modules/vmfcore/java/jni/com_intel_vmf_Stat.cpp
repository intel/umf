#include<string>
#include<vector>
#include <memory>
#include <stdexcept>
#include "vmf/statistics.hpp"
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

/*
* Class:     com_intel_vmf_Stat
* Method:    n_delete
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1delete(JNIEnv *env, jclass, jlong self);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1delete";

    try 
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        delete obj;
    }
    catch(const std::exception &e) { throwJavaException(env, &e, method_name); } 
    catch (...) { throwJavaException(env, 0, method_name); }
}


//long n_Stat(String name, long[] fields);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1Stat(JNIEnv *env, jclass, jstring name, jlongArray fields);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1Stat(JNIEnv *env, jclass, jstring name, jlongArray fields)
{
    static const char method_name[] = "Stat::n_1Stat";

    try
    {
        if (name == 0 || fields == 0) VMF_EXCEPTION(NullPointerException, "Neither name nor fields array can be null.");
        std::vector <StatField> sfs;
        jsize len;
        jlong* addrArray;
        len = env->GetArrayLength(fields);
        addrArray = env->GetLongArrayElements(fields, 0);
        if (addrArray)
        {
            for (int i = 0; i < len; i++)
            {
                std::shared_ptr<StatField>* f = (std::shared_ptr<StatField>*) addrArray[i];
                sfs.push_back(StatField(**f));
            }
            env->ReleaseLongArrayElements(fields, addrArray, 0);
        }
        return (jlong) new std::shared_ptr<Stat>(new Stat(getJavaStirng(env, name), sfs));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//String n_getName(long nativeObj);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Stat_n_1getName(JNIEnv *env, jclass, jlong self);
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Stat_n_1getName(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1getName";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return env->NewStringUTF((*obj)->getName().c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


//int n_getState(long nativeObj);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getState(JNIEnv *env, jclass, jlong self);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getState(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1getState";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return (*obj)->getState();
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//void n_setUpdateMode(long nativeObj, int updateMode);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1setUpdateMode(JNIEnv *env, jclass, jlong self, jint updateMode);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1setUpdateMode(JNIEnv *env, jclass, jlong self, jint updateMode)
{
    static const char method_name[] = "Stat::n_1setUpdateMode";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        (*obj)->setUpdateMode((Stat::UpdateMode::Type) updateMode);
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

//int n_getUpdateMode(long nativeObj);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getUpdateMode(JNIEnv *env, jclass, jlong self);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getUpdateMode(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1getUpdateMode";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return (*obj)->getUpdateMode();
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//void n_setUpdateTimeout(long nativeObj, int ms);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1setUpdateTimeout(JNIEnv *env, jclass, jlong self, jint ms);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1setUpdateTimeout(JNIEnv *env, jclass, jlong self, jint ms)
{
    static const char method_name[] = "Stat::n_1setUpdateTimeout";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        (*obj)->setUpdateTimeout(ms);
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

//int n_getUpdateTimeout(long nativeObj);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getUpdateTimeout(JNIEnv *env, jclass, jlong self);
JNIEXPORT jint JNICALL Java_com_intel_vmf_Stat_n_1getUpdateTimeout(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1getUpdateTimeout";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return (*obj)->getUpdateTimeout();
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//void n_update(long nativeObj, boolean doWait);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1update(JNIEnv *env, jclass, jlong self, jboolean doWait);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1update(JNIEnv *env, jclass, jlong self, jboolean doWait)
{
    static const char method_name[] = "Stat::n_1update";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        (*obj)->update(doWait == JNI_TRUE);
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

//String[] n_getAllFieldNames(long nativeObj);
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Stat_n_1getAllFieldNames(JNIEnv *env, jclass, jlong self);
JNIEXPORT jobjectArray JNICALL Java_com_intel_vmf_Stat_n_1getAllFieldNames(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "MetadataStream::n_1getAllFieldNames";
    try
    {
       std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
       if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");

        std::vector<std::string> names = (*obj)->getAllFieldNames();
        jobjectArray result = (jobjectArray)env->NewObjectArray((jsize)names.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
        for (jsize i = 0; i<names.size(); i++)
            env->SetObjectArrayElement(result, i, env->NewStringUTF(names[i].c_str()));

        return result;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//long n_getField(long nativeObj, String name);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1getField(JNIEnv *env, jclass, jlong self, jstring name);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1getField(JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Stat::n_1getField";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return (jlong) new std::shared_ptr<StatField>( new StatField((*obj)->getField(getJavaStirng(env, name))) );
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1getValue(JNIEnv *env, jclass, jlong self, jstring name);
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Stat_n_1getValue(JNIEnv *env, jclass, jlong self, jstring name)
{
    static const char method_name[] = "Stat::n_1getValue";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        return (jlong) new std::shared_ptr<Variant>(new Variant((*obj)->getField(getJavaStirng(env, name)).getValue()));
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

//void n_clear(long nativeObj);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1clear(JNIEnv *env, jclass, jlong self);
JNIEXPORT void JNICALL Java_com_intel_vmf_Stat_n_1clear(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Stat::n_1clear";

    try
    {
        std::shared_ptr<Stat>* obj = (std::shared_ptr<Stat>*) self;
        if (obj == NULL || *obj == NULL)  VMF_EXCEPTION(NullPointerException, "Stat (self) is null pointer.");
        (*obj)->clear();
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}

}
