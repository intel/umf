#include<string>
#include<vector>
#include<numeric>
#include "vmf/format_json.hpp"
#include "throwJavaException.hpp"

extern "C" {

using namespace vmf;

/*
 * Class:     com_intel_vmf_FormatJSON
 * Method:    n_FormatJSON
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_vmf_FormatJSON_n_1FormatJSON(JNIEnv *env, jclass);


JNIEXPORT jlong JNICALL Java_com_intel_vmf_FormatJSON_n_1FormatJSON(JNIEnv *env, jclass)
{
    static const char method_name[] = "FormatJSON::n_1FormatJSON";

    try
    {
        std::shared_ptr<FormatJSON>* obj = new std::shared_ptr<FormatJSON>(new FormatJSON());
        return (jlong)obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


/*
 * Class:     com_intel_vmf_FormatJSON
 * Method:    n_store
 * Signature: (long nativeObj, long set, long[] schemas, long[] segments, long[] stats, String[] attribNames, String[] attribVals);
 */
JNIEXPORT jstring JNICALL Java_com_intel_vmf_FormatJSON_n_1store(JNIEnv *env, jclass, jlong self, jlong setAddr, jlongArray schemaAddrs, jlongArray segAddrs, jlongArray statsAddrs, jobjectArray attribNames, jobjectArray attribVals);


JNIEXPORT jstring JNICALL Java_com_intel_vmf_FormatJSON_n_1store(JNIEnv *env, jclass, jlong self, jlong setAddr, jlongArray schemaAddrs, jlongArray segAddrs, jlongArray statsAddrs, jobjectArray attribNames, jobjectArray attribVals)
{
    static const char method_name[] = "FormatJSON::n_1store";

    try
    {
        std::shared_ptr<FormatJSON>* obj = (std::shared_ptr<FormatJSON>*) self;
        if (!obj || !*obj)
            VMF_EXCEPTION(NullPointerException, "'self' is null.");

        std::shared_ptr<MetadataSet>* set = (std::shared_ptr<MetadataSet>*) setAddr;

        std::vector <std::shared_ptr<MetadataSchema>> vecSchemas;
        jsize lenSchemas;
        jlong* schemasArray;
        if (schemaAddrs)
        {
            lenSchemas = env->GetArrayLength(schemaAddrs);
            schemasArray = env->GetLongArrayElements(schemaAddrs, 0);
            if (schemasArray)
            {
                for (int j = 0; j < lenSchemas; j++)
                {
                    std::shared_ptr<MetadataSchema>* pSpSchema = (std::shared_ptr<MetadataSchema>*) schemasArray[j];
                    vecSchemas.push_back(*pSpSchema);
                }
                env->ReleaseLongArrayElements(schemaAddrs, schemasArray, 0);
            }
        }

        std::vector <std::shared_ptr<MetadataStream::VideoSegment>> vecSegments;
        jsize lenSegments;
        jlong* segmentsArray;
        if (segAddrs)
        {
            lenSegments = env->GetArrayLength(segAddrs);
            segmentsArray = env->GetLongArrayElements(segAddrs, 0);
            if (segmentsArray)
            {
                for (int i = 0; i < lenSegments; i++)
                {
                    std::shared_ptr<MetadataStream::VideoSegment>* pSpSegment = (std::shared_ptr<MetadataStream::VideoSegment>*) segmentsArray[i];
                    vecSegments.push_back(*pSpSegment);
                }
                env->ReleaseLongArrayElements(segAddrs, segmentsArray, 0);
            }
        }

        std::vector <std::shared_ptr<Stat>> vecStats;
        jsize lenStats;
        jlong* statsArray;
        if (statsAddrs)
        {
            lenStats = env->GetArrayLength(statsAddrs);
            statsArray = env->GetLongArrayElements(statsAddrs, 0);
            if (statsArray)
            {
                for (int i = 0; i < lenStats; i++)
                {
                    std::shared_ptr<Stat>* stat = (std::shared_ptr<Stat>*)statsArray[i];
                    vecStats.push_back( *stat );
                }
                env->ReleaseLongArrayElements(statsAddrs, statsArray, 0);
            }
        }

        Format::AttribMap attribs;
        if (attribNames && attribVals)
        {
            int sizeN = env->GetArrayLength(attribNames);
            int sizeV = env->GetArrayLength(attribVals);
            if (sizeN > 0 && sizeN == sizeV)
            {
                for (int i = 0; i < sizeN; ++i)
                {
                    jstring name = (jstring)env->GetObjectArrayElement(attribNames, i);
                    jstring val = (jstring)env->GetObjectArrayElement(attribVals, i);
                    const char* nameStr = env->GetStringUTFChars(name, 0);
                    const char* valStr  = env->GetStringUTFChars(val, 0);
                    attribs[nameStr] = valStr;
                    env->ReleaseStringUTFChars(name, nameStr);
                    env->ReleaseStringUTFChars(val, valStr);
                    env->DeleteLocalRef(name);
                    env->DeleteLocalRef(val);
                }
            }
        }


        std::string xml = (*obj)->store(
            (set && *set ? **set : MetadataSet()),
            vecSchemas,
            vecSegments,
            vecStats,
            attribs
            );
        return env->NewStringUTF(xml.c_str());
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}


/*
* Class:     com_intel_vmf_FormatJSON
* Method:    n_parse
* Signature: long[] n_parse(long nativeObj, String text, String[] attribNames, String[] attribVals)
*/
JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_FormatJSON_n_1parse(JNIEnv *env, jclass, jlong self, jstring text, jobjectArray attribNames, jobjectArray attribVals);

JNIEXPORT jlongArray JNICALL Java_com_intel_vmf_FormatJSON_n_1parse(JNIEnv *env, jclass, jlong self, jstring text, jobjectArray attribNames, jobjectArray attribVals)
{
    static const char method_name[] = "FormatJSON::n_1parse";
    try
    {
        std::shared_ptr<FormatJSON>* obj = (std::shared_ptr<FormatJSON>*) self;
        if (!obj || !*obj)
            VMF_EXCEPTION(NullPointerException, "'self' is null");

        if (!text)
            VMF_EXCEPTION(NullPointerException, "Input text is null");

        const char* textStr = env->GetStringUTFChars(text, 0);
        if (!textStr)
            VMF_EXCEPTION(InternalErrorException, "Error accessing input text");

        std::vector<MetadataInternal> metadata;
        std::vector<std::shared_ptr<MetadataSchema>> schemas;
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
        std::vector<std::shared_ptr<Stat>> stats;
        Format::AttribMap attribs;
        Format::ParseCounters counters = (*obj)->parse(textStr, metadata, schemas, segments, stats, attribs);

        env->ReleaseStringUTFChars(text, textStr);

        jsize addressesArraySize = counters.metadata + 1 + counters.schemas + 1 + counters.segments + 1 + counters.stats;

        jlongArray objs  = env->NewLongArray(addressesArraySize);
        jlong* objsAddrs = env->GetLongArrayElements(objs, 0);
        if (objsAddrs)
        {
            int cnt = 0;
            //metadata
            for (int i = 0; i < counters.metadata; i++) objsAddrs[i + cnt] = (jlong) new MetadataInternal(metadata[i]);
            objsAddrs[counters.metadata + cnt] = 0;
            cnt += counters.metadata + 1;
            //schemas
            for (int i = 0; i < counters.schemas; i++) objsAddrs[i + cnt] = (jlong) new std::shared_ptr<MetadataSchema>(schemas[i]);
            objsAddrs[counters.schemas + cnt] = 0;
            cnt += counters.schemas + 1;
            //segments
            for (int i = 0; i < counters.segments; i++) objsAddrs[i + cnt] = (jlong) new std::shared_ptr<MetadataStream::VideoSegment>(segments[i]);
            objsAddrs[counters.segments + cnt] = 0;
            cnt += counters.segments + 1;
            //stats
            for (int i = 0; i < counters.stats; i++) objsAddrs[i + cnt] = (jlong) new std::shared_ptr<Stat>(stats[i]);
            env->ReleaseLongArrayElements(objs, objsAddrs, 0);
        }

        if (attribNames && attribVals)
        {
            int sizeN = env->GetArrayLength(attribNames);
            int sizeV = env->GetArrayLength(attribVals);
            if (sizeN > 0 && sizeN == sizeV)
            {
                int cnt = 0;
                for (const auto& a : attribs)
                {
                    env->SetObjectArrayElement(attribNames, cnt, env->NewStringUTF(a.first.c_str()));
                    env->SetObjectArrayElement(attribVals,  cnt, env->NewStringUTF(a.second.c_str()));
                    if (++cnt >= sizeN) break;
                }
            }
        }

        return objs;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
    return 0;
}

/*
 * Class:     com_intel_vmf_FormatJSON
 * Method:    n_delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_vmf_FormatJSON_n_1delete(JNIEnv *env, jclass, jlong self);

JNIEXPORT void JNICALL Java_com_intel_vmf_FormatJSON_n_1delete(JNIEnv *env, jclass, jlong self)
{
    static const char method_name[] = "FormatJSON::n_1delete";

    try
    {
        std::shared_ptr<FormatJSON>* obj = (std::shared_ptr<FormatJSON>*) self;
        if (!obj || !*obj) VMF_EXCEPTION(NullPointerException, "'this' is null.");
        delete obj;
    }
    catch (const std::exception &e) { throwJavaException(env, &e, method_name); }
    catch (...) { throwJavaException(env, 0, method_name); }
}


}
