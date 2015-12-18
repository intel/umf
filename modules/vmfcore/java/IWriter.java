//package com.intel.vmf;

interface IWriter
{
    long getNativeAddr();
    
    String store (MetadataSchema[] schemas);

    String store (MetadataSet set);

    String store (long nextId,
                  String filepath,
                  String checksum,
                  VideoSegment[] segments,
                  MetadataSchema[] schemas,
                  MetadataSet set);

    String store (VideoSegment segment);	

    String store (VideoSegment[] segments);	
}
