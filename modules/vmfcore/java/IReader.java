//package com.intel.vmf;

interface IReader
{
    boolean parseAll (String text, long nextId,
                      String filepath, String checksum,
                      VideoSegment[] segments, 
                      MetadataSchema[] schemas,
                      MetadataInternal[] metadata);
				   
    boolean parseSchemas (String text, MetadataSchema[] schemas);
	
    boolean parseMetadata (String text, MetadataSchema[] schemas, MetadataInternal[] metadata);
	
    boolean parseVideoSegments (String text, VideoSegment[] segments);
}
