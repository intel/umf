#include<string>
#include<vector>
#include "../com_intel_vmf_FieldValue.h"

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__ (JNIEnv *, jclass)
{
    std::shared_ptr<FieldValue>* p = new std::shared_ptr<FieldValue>(new FieldValue ());
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__J (JNIEnv *env, jclass, jlong other)
{
    std::shared_ptr<FieldValue>* obj = (std::shared_ptr<FieldValue>*) other;
    std::shared_ptr<FieldValue>* p = new std::shared_ptr<FieldValue>(new FieldValue ((*(obj->get()))));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_FieldValue
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FieldValue_n_1FieldValue__Ljava_lang_String_2J (JNIEnv *env, jclass, jstring name, jlong variantAddr)
{
    std::shared_ptr<Variant>* obj = (std::shared_ptr<Variant>*) variantAddr;
    std::string sName (env->GetStringUTFChars (name, NULL));
    std::shared_ptr<FieldValue>* p = new std::shared_ptr<FieldValue>(new FieldValue (sName, (*(obj->get()))));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FieldValue_n_1getName (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FieldValue::n_1getName";
    
    try 
    {
        std::shared_ptr<FieldValue>* obj = (std::shared_ptr<FieldValue>*) self;
        const std::string str = (*obj)->getName ();
        return env->NewStringUTF (str.c_str());
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1setTo (JNIEnv *env, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldValue::n_1setTo";
    
    try 
    {
        std::shared_ptr<FieldValue>* self = (std::shared_ptr<FieldValue>*) selfAddr;
        std::shared_ptr<FieldValue>* other = (std::shared_ptr<FieldValue>*) otherAddr;
        *(*self) = *(*other);
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_FieldValue_n_1equals (JNIEnv *, jclass, jlong selfAddr, jlong otherAddr)
{
    static const char method_name[] = "FieldValue::n_1equals";
    
    try 
    {
        std::shared_ptr<FieldValue>* self = (std::shared_ptr<FieldValue>*) selfAddr;
        std::shared_ptr<FieldValue>* other = (std::shared_ptr<FieldValue>*) otherAddr;
        
        if (*(*self) == *(*other))
            return JNI_TRUE;
        else
            return JNI_FALSE;
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
 * Class:     com_intel_vmf_FieldValue
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FieldValue_n_1delete (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr<FieldValue>* p = (std::shared_ptr<FieldValue>*) self;
    delete p;
}
