
interface IWriter
{
    String store(final MetadataSchema[] schemas);

    String store(final MetadataSet set);

    String store(final Long nextId,
                 final String filepath,
                 final String checksum,
                 final MetadataStream.VideoSegment[] segments,
                 final MetadataSchema[] schemas,
                 final MetadataSet set );

    String store(final MetadataStream.VideoSegment spSegment);	

    String store(final MetadataStream.VideoSegment[] segments);	
}
