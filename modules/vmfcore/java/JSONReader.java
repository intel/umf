//package com.intel.vmf;

//import java.util.String;
import java.io.DataInputStream;
import java.io.DataOutputStream;

public class JSONReader implements IReader
{
    static
    {
        System.loadLibrary("vmf");
        //System.loadLibrary("gnustl_shared");
    }

    protected final long nativeObj;
	
    public JSONReader ()
    {
        nativeObj = n_JSONReader();
    }
	
    public long getNativeAddr()
    {
        return nativeObj;
    }
    
    public boolean parseAll (final String text, long nextId,
                             String filepath, String checksum,
                             MetadataStream.VideoSegment[] segments,
                             MetadataSchema[] schemas,
                             MetadataInternal[] metadata)
    {
        return n_parseAll (nativeObj, text, nextId, filepath, 
        		           checksum, segments, schemas, metadata);
    }
	
    public boolean parseSchemas (final String text, MetadataSchema[] schemas)
    {
        return n_parseSchemas (nativeObj, text, schemas);
    }
	
    public boolean parseMetadata (final String text, final MetadataSchema[] schemas, MetadataInternal[] metadata)
    {
        return n_parseMetadata (nativeObj, text, schemas, metadata);
    }
	
    public boolean parseVideoSegments (final String text, MetadataStream.VideoSegment[] segments)
    {
        return n_parseVideoSegments (nativeObj, text, segments);
    }
	
    @Override
    protected void finalize () throws Throwable 
    {
        n_delete (nativeObj);
        super.finalize();
    }
	
	private native long n_JSONReader();
	
	private static native boolean n_parseAll ( long nativeObj, final String text, long nextId,
                                               String filepath, String checksum,
                                               MetadataStream.VideoSegment[] segments, 
                                               MetadataSchema[] schemas,
                                               MetadataInternal[] metadata );
	
	private static native boolean n_parseSchemas (long nativeObj, final String text, MetadataSchema[] schemas );
	
	private static native boolean n_parseMetadata (long nativeObj, final String text, final MetadataSchema[] schemas, MetadataInternal[] metadata );
	
	private static native boolean n_parseVideoSegments (long nativeObj, final String text, MetadataStream.VideoSegment[] segments );
	
	private static native void n_delete (long nativeObj);
}
