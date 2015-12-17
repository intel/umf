//package com.intel.vmf;

interface IWriter
{
    String store (MetadataSchema[] schemas);

    String store (MetadataSet set);

    String store (long nextId,
                  String filepath,
                  String checksum,
                  MetadataStream.VideoSegment[] segments,
                  MetadataSchema[] schemas,
                  MetadataSet set);

    String store (MetadataStream.VideoSegment segment);	

    String store (MetadataStream.VideoSegment[] segments);	
}
