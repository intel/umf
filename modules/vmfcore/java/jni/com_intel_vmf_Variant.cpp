#include<string>
#include<vector>
#include "vmf/metadatastream.hpp"
#include "../com_intel_vmf_Variant.h"

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
 * Class:     com_intel_vmf_Variant
 * Method:    n_Variant
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant__ (JNIEnv *, jclass)
{
    std::shared_ptr <Variant>* p = new std::shared_ptr <Variant>(new Variant());
    return (jlong) p;
} 

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_Variant
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_Variant_n_1Variant__J (JNIEnv *, jclass, jlong otherAddr)
{
    std::shared_ptr <Variant>* other = (std::shared_ptr <Variant>*)otherAddr;
    std::shared_ptr <Variant>* p = new std::shared_ptr <Variant>(new Variant(**other));
    return (jlong) p;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JI (JNIEnv *env, jclass, jlong self, jint value)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    (**obj) = (int) value;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JF)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JF (JNIEnv *env, jclass, jlong self, jfloat value)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    (**obj) = (float) value;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (J[I)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3I (JNIEnv *env, jclass, jlong self, jintArray values)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::vector <int> vecValues;
    jint *cArray = env->GetIntArrayElements (values, 0);
    jsize len = env->GetArrayLength (values);
    
    for (int i = 0; i < len; i++)
    {
        vecValues.push_back((int)cArray[i]);
    }
    
    env->ReleaseIntArrayElements (values, cArray, 0);
    (**obj) = vecValues;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (J[F)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__J_3F (JNIEnv *env, jclass, jlong self, jfloatArray values)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::vector <float> vecValues;
    jfloat* array = env->GetFloatArrayElements (values, 0);
    jsize len = env->GetArrayLength (values);
    
    for (int i = 0; i < len; i++)
    {
        float elem = (float) array[i];
        vecValues.push_back (elem);
    }
    
    env->ReleaseFloatArrayElements (values, array, 0);
    (**obj) = vecValues;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JLjava_lang_String_2 (JNIEnv *env, jclass, jlong self, jstring str)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    (**obj) = env->GetStringUTFChars (str, NULL);
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_setTo
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1setTo__JJ (JNIEnv *env, jclass, jlong self, jlong other)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::shared_ptr <Variant>* otherObj = (std::shared_ptr <Variant>*)other;
    (**obj) = (**otherObj);
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1equals (JNIEnv *env, jclass, jlong self, jlong other)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::shared_ptr <Variant>* otherObj = (std::shared_ptr <Variant>*)other;
    
    if ((**obj) == (**otherObj))
        return JNI_TRUE;
    else
        return JNI_FALSE;
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_toString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1toString (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::string str = (*obj)->toString ();
    return env->NewStringUTF (str.c_str());
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_fromString
 * Signature: (JILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1fromString (JNIEnv *env, jclass, jlong self, jint type, jstring value)
{
    static const char method_name[] = "Variant::n_1fromString";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        std::string str (env->GetStringUTFChars (value, NULL));
        Variant::Type Type = (Variant::Type) type;
        if (Type >= Variant::Type::type_unknown && Type <= Variant::Type::type_vec4d_vector)
            (*obj)->fromString (Type, str);
        else
            throwJavaException(env, 0, method_name);
    } 
    catch(const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    } 
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1getType (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1getType";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        return (jint) (*obj)->getType ();
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_isEmpty
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isEmpty (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1isEmpty";
    
    try 
    {
        std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
        return (jboolean) (*obj)->isEmpty ();
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_getTypeName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1getTypeName (JNIEnv *env, jclass, jlong self)
{
    std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
    std::string str = (*obj)->getTypeName ();
    return env->NewStringUTF (str.c_str());
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_convertTo
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1convertTo (JNIEnv *env, jclass, jlong self, jint type)
{
    static const char method_name[] = "Variant::n_1convertTo";
    
    try 
    {
        Variant::Type Type = (Variant::Type) type;
        if ((Type >= Variant::Type::type_unknown) && (Type <= Variant::Type::type_vec4d_vector))
        {
            std::shared_ptr <Variant>* obj = (std::shared_ptr <Variant>*)self;
            (*obj)->convertTo (Type);
        }
        else
            throwJavaException(env, 0, method_name);
    }
    catch(const std::exception &e)
    {
        throwJavaException(env, &e, method_name);
    } 
    catch (...)
    {
        throwJavaException(env, 0, method_name);
    }
}

/*
 * Class:     com_intel_vmf_Variant
 * Method:    n_typeToString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1typeToString (JNIEnv *env, jclass, jint type)
{
    static const char method_name[] = "Variant::n_1typeToString";
    
    try 
    {
        Variant::Type Type = (Variant::Type) type;
        if (Type >= Variant::Type::type_unknown && Type <= Variant::Type::type_vec4d_vector)
        {
            std::string str = Variant::typeToString (Type);
            return env->NewStringUTF (str.c_str());
        }
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_typeFromString
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_intel_vmf_Variant_n_1typeFromString (JNIEnv *env, jclass, jstring type)
{
    static const char method_name[] = "Variant::n_1typeFromString";
     
    try 
    {
        std::string sType (env->GetStringUTFChars (type, NULL));
        return (jint) Variant::typeFromString (sType);
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_isConvertible
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_vmf_Variant_n_1isConvertible (JNIEnv *env, jclass, jint srcType, jint dstType)
{
    static const char method_name[] = "Variant::n_1isConvertible";
    
    try 
    {
        Variant::Type src = (Variant::Type) srcType;
        Variant::Type dst = (Variant::Type) dstType;
        return (jboolean) Variant::isConvertible (src, dst);
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_base64Encode
 * Signature: ([BI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_Variant_n_1base64Encode (JNIEnv *env, jclass, jbyteArray buf, jint length)
{
    static const char method_name[] = "Variant::n_1base64Encode";
    try 
    {
        char* array = (char*) env->GetByteArrayElements (buf, 0);
        vmf_rawbuffer rb(array, (size_t)length);

        std::string str = Variant::base64encode(rb);

        env->ReleaseByteArrayElements (buf, (jbyte*)array, 0);
        return env->NewStringUTF(str.c_str());
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_base64Decode
 * Signature: (Ljava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_intel_vmf_Variant_n_1base64Decode (JNIEnv *env, jclass, jstring src)
{
    static const char method_name[] = "Variant::n_1base64Decode";
    try 
    {
        std::string str (env->GetStringUTFChars (src, NULL));
        vmf_rawbuffer buf = Variant::base64decode (str);
        jbyteArray byteArray = env->NewByteArray ((jsize)buf.size);
        jbyte* array = env->GetByteArrayElements (byteArray, 0);
        
        for (std::size_t i = 0; i < buf.size; i++)
        {
            array[i] = buf.data[i];
        }
        
        env->ReleaseByteArrayElements (byteArray, array, 0);
        return byteArray;
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
 * Class:     com_intel_vmf_Variant
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_Variant_n_1delete (JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "Variant::n_1delete";

    try
    {
        std::shared_ptr<Variant>* p = (std::shared_ptr<Variant>*) self;
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
