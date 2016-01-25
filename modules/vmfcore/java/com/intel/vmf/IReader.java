package com.intel.vmf;

interface IReader
{
    long getNativeAddr();
     
    /*boolean parseAll (String text, long nextId,
                      String filepath, String checksum,
                      MetadataStream.VideoSegment[] segments, 
                      MetadataSchema[] schemas,
                      MetadataInternal[] metadata);
	
	PropertyMap parseProperties (String text);
	*/
    
    MetadataSchema[] parseSchemas (String text);
	
    MetadataInternal[] parseMetadata (String text);
	
    MetadataStream.VideoSegment[] parseVideoSegments (String text);
}
