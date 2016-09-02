#include < iostream >
#include "stdafx.h"
#include < boost/python/module.hpp >
#include < boost/python/def.hpp >
#include "umf/vmdatasource.hpp"
#include<string>
#include<vector>
#include "umf/metadatastream.hpp"
#include "umf/umf.hpp"
#include <boost/python/class.hpp>
#include<boost/python.hpp>

using namespace std;
using namespace boost::python;
using namespace umf;


double umf_vec2d_getX(const umf::umf_vec2d& self)
{
	return self.x;
}


double umf_vec2d_getY(const umf::umf_vec2d& self)
{
	return self.y;
}

double umf_vec3d_getX(const umf::umf_vec3d& self)
{
	return self.x;
}

double umf_vec3d_getY(const umf::umf_vec3d& self)
{
	return self.y;
}

double umf_vec3d_getZ(const umf::umf_vec3d& self)
{
	return self.z;
}

double umf_vec4d_getX(const umf::umf_vec4d& self)
{
	return self.x;
}

double umf_vec4d_getY(const umf::umf_vec4d& self)
{
	return self.y;
}

double umf_vec4d_getZ(const umf::umf_vec4d& self)
{
	return self.z;
}

double umf_vec4d_getW(const umf::umf_vec4d& self)
{
	return self.w;
}


const int State_UpToDate = umf::Stat::State::Type::UpToDate;
const int State_NeedUpdate = umf::Stat::State::Type::NeedUpdate;
const int State_NeedRescan = umf::Stat::State::Type::NeedRescan;
const int Action_Add = umf::Stat::Action::Type::Add;
const int Action_Remove = umf::Stat::Action::Type::Remove;
const int UpdateMode_Disabled = umf::Stat::UpdateMode::Type::Disabled;
const int UpdateMode_Manual = umf::Stat::UpdateMode::Type::Manual;
const int UpdateMode_OnAdd = umf::Stat::UpdateMode::Type::OnAdd;
const int UpdateMode_OnTimer = umf::Stat::UpdateMode::Type::OnTimer;

static boost::shared_ptr<umf::FieldDesc> fieldDescConst1(std::string a, umf::Variant::Type b)
{
	return boost::shared_ptr < umf::FieldDesc>(new umf::FieldDesc(a, b, false));
}

std::string fieldDesc_getName(const umf::FieldDesc& self)
{
	return self.name;
}

umf::Variant::Type fieldDesc_getType(const umf::FieldDesc& self)
{
	return self.type;
}

bool fieldDesc_optional(const umf::FieldDesc& self)
{
	return  self.optional;
}


void fieldValue_setTo(umf::FieldValue& self, umf::FieldValue& other)
{
	self = other;

}

void fieldValue_clear(umf::FieldValue& self)
{
	FieldValue* newObj = new FieldValue();
	self = *newObj;
}

//Log.log varadic arguments handled
void logwrap( const umf::LogLevel level, const std::string& msg, const std::string& func, const std::string&file, int line)
{
	Log::log(level, msg, func, file, line);
}

const int LOG_INFOW = umf::LogLevel::LOG_INFO;
const int LOG_WARNINGW = umf::LogLevel::LOG_WARNING;
const int LOG_ERRORW = umf::LogLevel::LOG_ERROR;
const int LOG_NO_MESSAGEW = umf::LogLevel::LOG_NO_MESSAGE;


bool isReference1(umf::Metadata& self, const IdType& id, const std::string& refName)
{
	return self.isReference(id, refName);
}

bool isReference2(umf::Metadata& self, const std::shared_ptr<Metadata>& md, const std::string& refName)
{
	return self.isReference(md, refName);
}

void removeReference1(umf::Metadata& self, const umf::IdType& id, const std::string& refName)
{
	self.removeReference(id, refName);
}

void removeReference2(umf::Metadata& self, const std::shared_ptr<Metadata>& md, const std::string& refName)
{
	self.removeReference(md, refName);
}


bool MetadataDesc_getFieldDesc1(umf::MetadataDesc& self, umf::FieldDesc& field, const std::string& sFieldName)
{
	return self.getFieldDesc(field, sFieldName);
}

FieldDesc& MetadataDesc_getFieldDesc2(umf::MetadataDesc& self, const std::string& sFieldName)
{
	return self.getFieldDesc(sFieldName);
}

bool MetadataStream_load1(umf::MetadataStream& self, const std::string & sSchemaName)
{
	return self.load(sSchemaName);
}

bool MetadataStream_load2(umf::MetadataStream& self, const std::string& sSchemaName, const std::string& sMetadataName)
{
	return self.load(sSchemaName, sMetadataName);
}


void MetadataStream_add1(umf::MetadataStream& self, std::shared_ptr<Stat>& stat)
{
	self.add(stat);
}

void MetadataStream_add2(umf::MetadataStream& self, std::shared_ptr<MetadataSchema> spSchema)
{
	self.add(spSchema);
}


void MetadataStream_add3(umf::MetadataStream& self, std::shared_ptr<umf::MetadataStream::VideoSegment> newSegment)
{
	self.add(newSegment);
}


bool  MetadataStream_remove1(umf::MetadataStream& self, const umf::IdType& id)
{
	return self.remove(id);
}


void MetadataStream_remove2(umf::MetadataStream& self, const MetadataSet& set) 
{
	self.remove(set);
}

void MetadataStream_remove3(umf::MetadataStream& self, std::shared_ptr<MetadataSchema> spSchema)
{
	self.remove(spSchema);
}

void MetadataStream_remove4(umf::MetadataStream& self)
{
	self.remove();
}

bool MetadataStream_import1(umf::MetadataStream& self, MetadataStream& srcStream, MetadataSet& srcSet, long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames, MetadataSet* pSetFailure)
{
	return self.import(srcStream, srcSet, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames, pSetFailure);
}


std::shared_ptr<Metadata> MetadataStream_import2(umf::MetadataStream& self, MetadataStream& srcStream, std::shared_ptr< Metadata >& spMetadata, std::map< IdType, IdType >& mapIds, long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames)
{
	cout << "Unable to access method";
	return nullptr;
	//return self.import(srcStream, spMetadata, mapIds, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames);
}

std::string MetadataStream_computeChecksum1(umf::MetadataStream& self)
{
	return self.computeChecksum();
}

std::string MetadataStream_computeChecksum2(umf::MetadataStream &self, long long &XMPPacketSize, long long &XMPPacketOffset)
{
	return self.computeChecksum(XMPPacketSize, XMPPacketOffset);
}


umf::MetadataSet MetadataStream_queryByReference(umf::MetadataStream &self, const std::string &sReferenceName)
{
	return self.queryByReference(sReferenceName);
}


umf::MetadataSet MetadataStream_queryByReference1(umf::MetadataStream &self, const std::string &sReferenceName, const umf::FieldValue &value)
{
	return self.queryByReference(sReferenceName, value);
}


umf::MetadataSet MetadataStream_queryByReference2(umf::MetadataStream &self, const std::string& sReferenceName, const std::vector< umf::FieldValue>& vFields)
{
	return self.queryByReference(sReferenceName, vFields);
}


std::string ReferenceDesc_getName(const umf::ReferenceDesc &self)
{
	return self.name;
}

bool ReferenceDesc_isUnique(const umf::ReferenceDesc &self)
{
	return self.isUnique;

}

bool ReferenceDesc_isCustom(const umf::ReferenceDesc &self)
{
	return self.isCustom;

}

BOOST_PYTHON_MODULE(umflib) {
	
	//UMF initialize and terminate functions
	def("initialize", &umf::initialize);
	def("terminate", &umf::terminate);


	//umf_vec2d type
	class_<umf::umf_vec2d>("umf_vec2d")
		.def(init<double, double>())
		.def("getX", &umf_vec2d_getX)
		.def("getY", &umf_vec2d_getY)
		.def("equals", &umf::umf_vec2d::operator==)
		;


	//umf_vec3d type
	class_<umf::umf_vec3d>("umf_vec3d")
		.def(init<double, double, double>())
		.def("getX", &umf_vec3d_getX)
		.def("getY", &umf_vec3d_getY)
		.def("getZ", &umf_vec3d_getZ)
		.def("equals", &umf::umf_vec3d::operator==)
		;
	
	//umf_vec4d type
	class_<umf::umf_vec4d>("umf_vec4d")
		.def(init<double, double, double, double>())
		.def("getX", &umf_vec4d_getX)
		.def("getY", &umf_vec4d_getY)
		.def("getZ", &umf_vec4d_getZ)
		.def("getW", &umf_vec4d_getW)
		.def("equals", &umf::umf_vec4d::operator==)
		;

	//statfield class
	class_<umf::StatField>("StatField")
		.def(init < std::string, std::string, std::string, std::string, std::string>())
		.def("getName", &umf::StatField::getName)
		.def("getSchemaName", &umf::StatField::getSchemaName)
		.def("getMetaDataName", &umf::StatField::getMetadataName)
		.def("getFieldName", &umf::StatField::getFieldName)
		.def("getOpName", &umf::StatField::getOpName)
		.def("getValue", &umf::StatField::getValue)
		.def("equals", &umf::StatField::operator==)
		;

	
	//stat class

	//function pointer for update
	void    (umf::Stat::*u1)(bool) = &umf::Stat::update;
	class_<umf::Stat>("Stat", init<std::string, const std::vector< StatField >&>())
		.def("getName", &umf::Stat::getName)
		.def("getState", &umf::Stat::getState)
		.def("setUpdateMode", &umf::Stat::setUpdateMode)
		.def("getUpdateMode", &umf::Stat::getUpdateMode)
		.def("getUpdateTimeout", &umf::Stat::getUpdateTimeout)
		.def("update", u1)
		.def("clear", &umf::Stat::clear)
		.def("getAllFieldNames", &umf::Stat::getAllFieldNames)
		.add_property("getField", boost::python::make_function(&umf::Stat::getField, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def_readonly("State_UpToDate", State_UpToDate)
		.def_readonly("State_NeedUpdate", State_NeedUpdate)
		.def_readonly("State_NeedRescan", State_NeedRescan)
		.def_readonly("Action_Add", Action_Add)
		.def_readonly("Action_Remove", Action_Remove)
		.def_readonly("UpdateMode_Disabled", UpdateMode_Disabled)
		.def_readonly("UpdateMode_Manual", UpdateMode_Manual)
		.def_readonly("UpdateMode_OnAdd", UpdateMode_OnAdd)
		.def_readonly("UpdateMode_OnTimer", UpdateMode_OnTimer)
		;


	//Variant class
	//FP's for fromString
	void	(umf::Variant::*fromString1)(umf::Variant::Type, const std::string&) = &umf::Variant::fromString;
	void	(umf::Variant::*fromString2)(const std::string&) = &umf::Variant::fromString;


	class_<umf::Variant>("Variant")
		.def(init<const int&>())
		.def(init<const float&>())
		.def(init<const unsigned int&>())
		.def(init<const char*>())
		.def(init<const umf_vec2d&>())
		.def(init<const umf_vec3d&>())
		.def(init<const umf_vec4d&>())
		.def(init<const std::vector<int>&>())
		.def(init<const std::vector<float>&>())
		.def(init<const std::vector<umf::umf_vec2d>&>())
		.def(init<const std::vector<umf::umf_vec3d>&>())
		.def(init<const std::vector<umf::umf_vec4d>&>())
		.def("convertTo", &umf::Variant::convertTo)
		.def("base64decode", &umf::Variant::base64decode)
		.def("base64encode", &umf::Variant::base64encode)
		.def("fromString", fromString1)
		.def("fromString", fromString2)
		.def("getType", &umf::Variant::getType)
		.def("getTypeName", &umf::Variant::getTypeName)
		.def("get_integer", boost::python::make_function(&umf::Variant::get_integer, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_integer_vector", boost::python::make_function(&umf::Variant::get_integer_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_rawbuffer", boost::python::make_function(&umf::Variant::get_rawbuffer, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_real", boost::python::make_function(&umf::Variant::get_real, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_real_vector", boost::python::make_function(&umf::Variant::get_real_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_string", boost::python::make_function(&umf::Variant::get_string, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_string_vector", boost::python::make_function(&umf::Variant::get_string_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_vec2d", boost::python::make_function(&umf::Variant::get_vec2d, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_2d_vector", boost::python::make_function(&umf::Variant::get_vec2d_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_vec3d", boost::python::make_function(&umf::Variant::get_vec3d, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_vec3d_vector", boost::python::make_function(&umf::Variant::get_vec3d_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_vec4d", boost::python::make_function(&umf::Variant::get_vec4d, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("get_vec4d_vector", boost::python::make_function(&umf::Variant::get_vec4d_vector, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("isConvertible", &umf::Variant::isConvertible)
		.def("isEmpty", &umf::Variant::isEmpty)
		.def("toString", &umf::Variant::toString)
		.def("typeFromString", &umf::Variant::typeFromString)
		.def("typeToString", &umf::Variant::typeToString)
		.def("notEquals", &umf::Variant::operator!=)
		.def("equals", &umf::Variant::operator==)
		;

	
	//Compressor class
	class_<umf::Compressor,boost::noncopyable>("Compressor",no_init)
		.def("create", &umf::Compressor::create)
		.def("createNewInstance", &umf::Compressor::createNewInstance)
		.def("compress", &umf::Compressor::compress)
		.def("builtinId", &umf::Compressor::builtinId)
		.def("decompress", &umf::Compressor::decompress)
		.def("getId", &umf::Compressor::getId)
		.def("getRegisteredIds", &umf::Compressor::getRegisteredIds)
		.def("registerNew", &umf::Compressor::registerNew)
		.def("unregister", &umf::Compressor::unregister)
		;


	//Encryptor class
	class_<umf::Encryptor, boost::noncopyable >("Encryptor", no_init)
		.def("encrypt", &umf::Encryptor::encrypt)
		.def("decrypt",&umf::Encryptor::decrypt)
		.def("getHint",&umf::Encryptor::getHint)
		;
	

	//EncryptorDefault class
	class_<umf::EncryptorDefault>("EncryptorDefault", init<std::string>())
		.def("encrypt",&umf::EncryptorDefault::encrypt)
		.def("decrypt",&umf::EncryptorDefault::decrypt)
		.def("getHint",&umf::EncryptorDefault::getHint)
		;


	//Exception class
	class_<umf::Exception>("Exception", init<std::string>())
		.def("getMessage", &umf::Exception::getMessage)
		.def("what", &umf::Exception::what)
		;

	//Exception Association class
	class_<umf::AssociationException>("ExceptionAssociation")
		.def(init<std::string>())
		.def("getMessage", &umf::AssociationException::getMessage)
		.def("what", &umf::AssociationException::what)
		;


	//ExceptionDataStorage class
	class_<umf::DataStorageException>("ExceptionDataStorage")
		.def(init<std::string>())
		.def("getMessage", &umf::DataStorageException::getMessage)
		.def("what", &umf::DataStorageException::what)
		;


	//ExceptionIncorrectParam class
	class_<umf::IncorrectParamException>("ExceptionIncorrectParam")
		.def(init < std::string>())
		.def("getMessage", &umf::IncorrectParamException::getMessage)
		.def("what", &umf::IncorrectParamException::what)
		;


	//ExceptionInternalError class
	class_<umf::InternalErrorException>("ExceptionInternalError")
		.def(init<std::string>())
		.def("getMessage", &umf::InternalErrorException::getMessage)
		.def("what", &umf::InternalErrorException::what)
		;

	//ExceptionNotFound class
	class_<umf::NotFoundException>("ExceptionNotFound")
		.def(init<std::string>())
		.def("getMessage", &umf::NotFoundException::getMessage)
		.def("what", &umf::NotFoundException::what)
		;

	//ExceptionNotImplemented class
	class_<umf::NotImplementedException>("ExceptionNotImplemented")
		.def(init<std::string>())
		.def("getMessage", &umf::NotImplementedException::getMessage)
		.def("what", &umf::NotImplementedException::what)
		;

	//ExceptionNotInitialized class
	class_<umf::NotInitializedException>("ExceptionNotInitialized")
		.def(init<std::string>())
		.def("getMessage", &umf::NotInitializedException::getMessage)
		.def("what", &umf::NotInitializedException::what)
		;

	//ExceptionNullPointer class
	class_<umf::NullPointerException>("ExceptionNullPointer")
		.def(init<std::string>())
		.def("getMessage", &umf::NullPointerException::getMessage)
		.def("what", &umf::NullPointerException::what)
		;

	//ExceptionObjectExpired class

	class_<umf::ObjectExpiredException>("ExceptionObjectExpired")
		.def(init<std::string>())
		.def("getMessage", &umf::ObjectExpiredException::getMessage)
		.def("what", &umf::ObjectExpiredException::what)
		;


	//ExceptionOutOfRange class
	class_<umf::OutOfRangeException>("ExceptionOutOfRange")
		.def(init<std::string>())
		.def("getMessage", &umf::OutOfRangeException::getMessage)
		.def("what", &umf::OutOfRangeException::what)
		;


	//ExceptionTypeCast class
	class_<umf::TypeCastException>("ExceptionTypeCast")
		.def(init<std::string>())
		.def("getMessage", &umf::TypeCastException::getMessage)
		.def("what", &umf::TypeCastException::what)
		;


	//ExceptionValidate class
	class_<umf::ValidateException>("ExceptionValidate")
		.def(init<std::string>())
		.def("getMessage", &umf::ValidateException::getMessage)
		.def("what", &umf::ValidateException::what)
		;


	//FieldDesc class
	class_<umf::FieldDesc>("FieldDesc")
		.def("__init__",make_constructor(fieldDescConst1))
		.def(init<std::string,umf::Variant::Type,bool>())
		.def("equals", &umf::FieldDesc::operator==)
		.def("getName", fieldDesc_getName)
		.def("getType", fieldDesc_getType)
		.def("isOptional", fieldDesc_optional)
		;


	//FieldValue class
	class_<umf::FieldValue>("FieldValue")
		.def(init<std::string, umf::Variant>())
		.def("getName", boost::python::make_function(&umf::FieldValue::getName, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("equals",&umf::FieldValue::operator==)
		.def("getType",&umf::FieldValue::getType)
		.def("clear",fieldValue_clear)
		.def("setTo",fieldValue_setTo)
		.def("convertTo", boost::python::make_function(&umf::FieldValue::convertTo, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("isEmpty",&umf::FieldValue::isEmpty)
		;

	//Format class
	class_<umf::Format, boost::noncopyable>("Format",no_init)
		.def("parse", &umf::Format::parse)
		.def("store", &umf::Format::parse)
		.def("getBackendFormat", &umf::Format::getBackendFormat)
		;


	//FormatCompressed class
	class_<umf::FormatCompressed>("FormatCompressed", init<std::shared_ptr<Format>, std::string>())
		.def("parse", &umf::FormatCompressed::parse)
		.def("store", &umf::FormatCompressed::store)
		;


	//FormatEncrypted class
	class_<umf::FormatEncrypted>("FormatEncrypted", init<std::shared_ptr<Format>, std::shared_ptr<Encryptor>>())
		.def("parse", &umf::FormatEncrypted::parse)
		.def("store", &umf::FormatEncrypted::store)
		;


	//FormatJSON class
	class_<umf::FormatJSON>("FormatJSON")
		.def("parse", &umf::FormatJSON::parse)
		.def("store", &umf::FormatJSON::store)
		;


	//FormatXML class
	class_<umf::FormatXML>("FormatXML")
		.def("parse", &umf::FormatXML::parse)
		.def("store", &umf::FormatXML::store)
		;


	//ToDo: Sabarish to add Interface IQuery

	boost::python::enum_<umf::LogLevel>("LogLevel")
		.value("LOG_INFO", umf::LogLevel::LOG_INFO)
		.value("LOG_WARNING", umf::LogLevel::LOG_WARNING)
		.value("LOG_ERROR", umf::LogLevel::LOG_ERROR)
		.value("LOG_NO_MESSAGE", umf::LogLevel::LOG_NO_MESSAGE)
		.export_values()
		;

	//Log class
	class_<umf::Log>("Log")
		.def("log", logwrap)
		.staticmethod("log")
		.def("setVerbosityLevel", &umf::Log::setVerbosityLevel)
		.def("getVerbosityLevel", &umf::Log::getVerbosityLevel)
		.def("setLogToConsole", &umf::Log::logToConsole)
		.def("setLogToFile", &umf::Log::logToFile)
		;

	//MetaData class
	class_<umf::Metadata>("Metadata", init<std::shared_ptr< MetadataDesc >&, bool>())
		.def("getId", &umf::Metadata::getId)
		.def("getFrameIndex", &umf::Metadata::getFrameIndex)
		.def("getNumOfFrames", &umf::Metadata::getNumOfFrames)
		.def("setFrameIndex", &umf::Metadata::setFrameIndex)
		.def("setTimestamp", &umf::Metadata::setTimestamp)
		.def("getTime", &umf::Metadata::getTime)
		.def("getDuration", &umf::Metadata::getDuration)
		.def("getName", &umf::Metadata::getName)
		.def("getSchemaName", &umf::Metadata::getSchemaName)
		.def("getDesc", &umf::Metadata::getDesc)
		.def("getFieldNames", &umf::Metadata::getFieldNames)
		.def("getFieldValue", &umf::Metadata::getFieldValue)
		.def("hasField", &umf::Metadata::hasField)
		.def("equals", &umf::Metadata::operator==)
		.def("lessthan", &umf::Metadata::operator<)
		.def("getFirstReference", &umf::Metadata::getFirstReference)
		.def("getReferencesByMetadata", &umf::Metadata::getReferencesByMetadata)
		.def("getReferencesByName", &umf::Metadata::getReferencesByName)
		.def("getAllReferences", boost::python::make_function(&umf::Metadata::getAllReferences, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("isReference", isReference1)
		.def("isReference",isReference2)
		.def("addReference", &umf::Metadata::addReference)
		.def("removeReference", removeReference1)
		.def("removeReference", removeReference2)
		.def("setFieldValue", &umf::Metadata::setFieldValue)
		.def("addValue", &umf::Metadata::addValue)
		.def("validate", &umf::Metadata::validate)
		.def("isValid", &umf::Metadata::isValid)
		;
	

	// MetadataDesc
	class_<umf::MetadataDesc>("MetadataDesc")
		.def(init<std::string, umf::Variant::Type>())
		.def(init<std::string, std::vector< FieldDesc >&>())
		.def(init<std::string, std::vector< FieldDesc >&, std::vector<std::shared_ptr<ReferenceDesc>>&>())
		.def("getMetadataName", &umf::MetadataDesc::getMetadataName)
		.def("getSchemaName", &umf::MetadataDesc::getSchemaName)
		.def("getFields", &umf::MetadataDesc::getFields)
		.def("getAllReferenceDescs", boost::python::make_function(&umf::MetadataDesc::getAllReferenceDescs, boost::python::return_value_policy<boost::python::copy_const_reference>()))
		.def("declarCustomReference", &umf::MetadataDesc::declareCustomReference)
		.def("getReferenceDesc", &umf::MetadataDesc::getReferenceDesc)
		.def("getFieldDesc", MetadataDesc_getFieldDesc1)
		.def("getFieldDesc", boost::python::make_function(MetadataDesc_getFieldDesc2,boost::python::return_value_policy<boost::python::copy_non_const_reference>()));
		;


	//MetadataInternal class
	class_<umf::MetadataInternal>("MetadataInternal", init<MetadataInternal&>())
		;

	//MetadataSchema class
	class_<umf::MetadataSchema>("MetadataSchema", init<std::string>())
		.def(init<std::string, std::string>())
		.def("getName", &umf::MetadataSchema::getName)
		.def("getAuthor", &umf::MetadataSchema::getAuthor)
		.def("size", &umf::MetadataSchema::size)
		.def("add", &umf::MetadataSchema::add)
		.def("findMetadataDesc", &umf::MetadataSchema::findMetadataDesc)
		.def("getAll", &umf::MetadataSchema::getAll)
		.def("getStdSchemaName", &umf::MetadataSchema::getStdSchemaName)
		.def("getStdSchema", &umf::MetadataSchema::getStdSchema)
		;


	//VideoSegment class
	class_<umf::MetadataStream::VideoSegment>("VideoSegment")
		.def(init<std::string, double, long long>())
		.def(init<std::string, double, long long, long long>())
		.def(init<std::string, double, long long, long long, int, int>())
		.def("getTitle", &umf::MetadataStream::VideoSegment::getTitle)
		.def("setTitle", &umf::MetadataStream::VideoSegment::setTitle)
		.def("getFPS", &umf::MetadataStream::VideoSegment::getFPS)
		.def("setFPS", &umf::MetadataStream::VideoSegment::setFPS)
		.def("getDuration", &umf::MetadataStream::VideoSegment::getDuration)
		.def("setDuration", &umf::MetadataStream::VideoSegment::setDuration)
		.def("getTime", &umf::MetadataStream::VideoSegment::getTime)
		.def("setTime", &umf::MetadataStream::VideoSegment::setTime)
		.def("getResolution", &umf::MetadataStream::VideoSegment::getResolution)
		.def("setResolution", &umf::MetadataStream::VideoSegment::setResolution)
		;

	//MetaDataStream class
	class_<umf::MetadataStream>("MetadataStream")
		.def("open", &umf::MetadataStream::open)
		.def("reopen", &umf::MetadataStream::reopen)
		.def("load", MetadataStream_load1)
		.def("load", MetadataStream_load2)
		.def("save", &umf::MetadataStream::save)
		.def("saveTo", &umf::MetadataStream::saveTo)
		.def("close", &umf::MetadataStream::close)
		.def("getById", &umf::MetadataStream::getById)
		.def("add", MetadataStream_add1)
		.def("add", MetadataStream_add2)
		.def("add", MetadataStream_add3)
		.def("remove", MetadataStream_remove1)
		.def("remove", MetadataStream_remove2)
		.def("remove", MetadataStream_remove3)
		.def("remove", MetadataStream_remove4)
		.def("addSchema", &umf::MetadataStream::addSchema)
		.def("getSchema", &umf::MetadataStream::getSchema)
		.def("getAllSchemaNames", &umf::MetadataStream::getAllSchemaNames)
		.def("getAll", &umf::MetadataStream::getAll)
		.def("importSet", MetadataStream_import1)
		.def("importSet", MetadataStream_import2)
		.def("clear", &umf::MetadataStream::clear)
		.def("computeChecksum", MetadataStream_computeChecksum1)
		.def("computeChecksum", MetadataStream_computeChecksum2)
		.def("getChecksum", &umf::MetadataStream::getChecksum)
		.def("setChecksum", &umf::MetadataStream::setChecksum)
		.def("getAllVideoSegments", boost::python::make_function(&umf::MetadataStream::getAllVideoSegments, boost::python::return_value_policy<boost::python::copy_non_const_reference>()))
		.def("addStat", &umf::MetadataStream::addStat)
		.def("getAllStatNames", &umf::MetadataStream::getAllStatNames)
		.def("getStat", &umf::MetadataStream::getStat)
		.def("recalcStat", &umf::MetadataStream::recalcStat)
		.def("convertDurationToNumOfFrames", &umf::MetadataStream::convertTimestampToFrameIndex)
		.def("convertTimestampToFrameIndex", &umf::MetadataStream::convertTimestampToFrameIndex)
		.def("convertFrameIndexToTimestamp", &umf::MetadataStream::convertFrameIndexToTimestamp)
		.def("convertNumOfFramesToDuration", &umf::MetadataStream::convertFrameIndexToTimestamp)
		.def("queryByFrameIndex", &umf::MetadataStream::queryByFrameIndex)
		.def("queryByTime", &umf::MetadataStream::queryByTime)
		.def("queryByName", &umf::MetadataStream::queryByName)
		.def("queryByNameAndValue", &umf::MetadataStream::queryByNameAndValue)
		.def("queryByNameAndFields", &umf::MetadataStream::queryByNameAndFields)
		.def("queryByReference", MetadataStream_queryByReference1) 
		.def("queryByReference", MetadataStream_queryByReference2)
		;


	// Reference class
		class_<umf::Reference>("Reference")
		.def(init<std::shared_ptr<ReferenceDesc>&, std::weak_ptr<Metadata>&>())
		.def(init<std::shared_ptr<ReferenceDesc>&, const std::shared_ptr<Metadata>&>())
		.def("getReferenceMetadata", &umf::Reference::getReferenceMetadata)
		.def("getReferenceDescription", &umf::Reference::getReferenceDescription)
		.def("setReferenceMetadata", &umf::Reference::setReferenceMetadata)
		;

	//ReferenceDesc class
	class_<umf::ReferenceDesc>("ReferenceDesc")
		.def(init<std::string>())
		.def(init<std::string, bool, bool>())
		.def("getName", ReferenceDesc_getName)
		.def("isUnique", ReferenceDesc_isUnique)
		.def("isCustom", ReferenceDesc_isCustom)
		;

}