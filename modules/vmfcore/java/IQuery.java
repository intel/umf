package com.intel.vmf;

public interface IQuery
{	
	//final MetadataSet query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter );
	
	//public MetadataSet queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter );
	
	public MetadataSet queryByFrameIndex ( long index );
	
	public MetadataSet queryBySchema ( final String sSchemaName );
	
	public MetadataSet queryByName ( final String sName );
	
	public MetadataSet queryByNameAndValue ( final String sMetadataName, final FieldValue value );
	
	public MetadataSet queryByNameAndFields ( final String sMetadataName, final FieldValue[] vFields );
	
	public MetadataSet queryByReference( final String sReferenceName );
	
	public MetadataSet queryByReference( final String sReferenceName, final FieldValue value );
	
	public MetadataSet queryByReference( final String sReferenceName, final FieldValue[] vFields );
}