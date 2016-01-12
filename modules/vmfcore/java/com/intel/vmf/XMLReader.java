package com.intel.vmf;

import java.io.DataInputStream;
import java.io.DataOutputStream;

public class XMLReader implements IReader
{
    static
    {
        System.loadLibrary("vmf");
    }

    protected final long nativeObj;
	
    public XMLReader ()
    {
        nativeObj = n_XMLReader();
    }
	
    public long getNativeAddr()
    {
        return nativeObj;
    }
    
    public boolean parseAll (final String text, long nextId, String filepath, String checksum, 
                             MetadataStream.VideoSegment[] segments, MetadataSchema[] schemas, MetadataInternal[] mdInt)
    {
        long segNativeAddrs[] = new long [segments.length];
        long schemaNativeAddrs[] = new long [schemas.length];
        long mdIntNativeAddrs[] = new long [mdInt.length];
        
        for (int i = 0; i < segments.length; i++)
        {
            segNativeAddrs[i] = segments[i].nativeObj;
        }
        
        for (int j = 0; j < schemas.length; j++)
        {
            schemaNativeAddrs[j] = schemas[j].nativeObj;
        }
        
        for (int k = 0; k < mdInt.length; k++)
        {
            mdIntNativeAddrs[k] = mdInt[k].nativeObj;
        }
        
        return n_parseAll (nativeObj, text, nextId, filepath, 
        		           checksum, segNativeAddrs, schemaNativeAddrs, 
        		           mdIntNativeAddrs);
    }
	
    public boolean parseSchemas (String text, MetadataSchema[] schemas)
    {
        long schemaNativeAddrs[] = new long [schemas.length];
        
        for (int j = 0; j < schemas.length; j++)
        {
            schemaNativeAddrs[j] = schemas[j].nativeObj;
        }
        
        return n_parseSchemas (nativeObj, text, schemaNativeAddrs);
    }
	
    public boolean parseMetadata (String text, MetadataSchema[] schemas, MetadataInternal[] mdInt)
    {
        long schemaNativeAddrs[] = new long [schemas.length];
        long mdIntNativeAddrs[] = new long [mdInt.length];
        
        for (int j = 0; j < schemas.length; j++)
        {
            schemaNativeAddrs[j] = schemas[j].nativeObj;
        }
        
        for (int k = 0; k < mdInt.length; k++)
        {
            mdIntNativeAddrs[k] = mdInt[k].nativeObj;
        }
        
        return n_parseMetadata (nativeObj, text, schemaNativeAddrs, mdIntNativeAddrs);
    }
	
    public boolean parseVideoSegments (String text, MetadataStream.VideoSegment[] segments)
    {
        long segNativeAddrs[] = new long [segments.length];
        
        for (int i = 0; i < segments.length; i++)
        {
            segNativeAddrs[i] = segments[i].nativeObj;
        }
        
        return n_parseVideoSegments (nativeObj, text, segNativeAddrs);
    }
	
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
	
	private static native long n_XMLReader();
	private static native boolean n_parseAll (long nativeObj, String text, long nextId,
                                              String filepath, String checksum,
                                              long[] segments, long[] schemas, long[] md);
	private static native boolean n_parseSchemas (long nativeObj, String text, long[] schemas);
	private static native boolean n_parseMetadata (long nativeObj, String text, long[] schemas, long[] metadata);
	private static native boolean n_parseVideoSegments (long nativeObj, String text, long[] segments);
	private static native void n_delete (long nativeObj);
}
