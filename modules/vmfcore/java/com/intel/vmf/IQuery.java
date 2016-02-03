package com.intel.vmf;

interface IQuery
{	
	public MetadataSet queryByFrameIndex (long index);
	
	public MetadataSet queryBySchema (String schemaName);
	
	public MetadataSet queryByName (String name);
	
	public MetadataSet queryByNameAndValue (String mdName, FieldValue value);
	
	public MetadataSet queryByNameAndFields (String mdName, FieldValue[] fields );
	
	public MetadataSet queryByReference (String referenceName);
	
	public MetadataSet queryByReference (String referenceName, FieldValue value);
	
	public MetadataSet queryByReference (String referenceName, FieldValue[] fields);
}
