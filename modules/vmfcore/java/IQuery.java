//package com.intel.vmf;

interface IQuery
{	
	//final MetadataSet query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter );
	
	//public MetadataSet queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter );
	
	public MetadataSet queryByFrameIndex (long index);
	
	public MetadataSet queryBySchema (String schemaName);
	
	public MetadataSet queryByName (String name);
	
	public MetadataSet queryByNameAndValue (String mdName, FieldValue value);
	
	public MetadataSet queryByNameAndFields (String mdName, FieldValue[] fields );
	
	public MetadataSet queryByReference (String referenceName);
	
	public MetadataSet queryByReference (String referenceName, FieldValue value);
	
	public MetadataSet queryByReference (String referenceName, FieldValue[] fields);
}
