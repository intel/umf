package com.intel.vmf;

interface IReader
{
    boolean parseAll ( final String text, Long nextId,
                       String filepath, String checksum,
                       MetadataStream.VideoSegment[] segments, 
                       MetadataSchema[] schemas,
                       MetadataInternal[] metadata);
				   
    boolean parseSchemas (final String text, MetadataSchema[] schemas);
	
    boolean parseMetadata (final String text, final MetadataSchema[] schemas, MetadataInternal[] metadata);
	
    boolean parseVideoSegments(final String text, MetadataStream.VideoSegment[] segments);
}
