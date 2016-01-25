package com.intel.vmf;

public class JSONReader implements IReader
{
    static
    {
        System.loadLibrary("vmf");
    }

    protected final long nativeObj;
	
    public JSONReader ()
    {
        nativeObj = n_JSONReader();
    }
	
    @Override
    public long getNativeAddr()
    {
        return nativeObj;
    }
    
    /*TO DO:
    @Override
    public boolean parseAll (final String text, long nextId, String filepath, String checksum, 
                             MetadataStream.VideoSegment[] segments, MetadataSchema[] schemas, MetadataInternal[] mdInt);*/
    
    //TO DO: PropertyMap parseProperties (String text);
    
    @Override
    public MetadataSchema[] parseSchemas (String text)
    {
        long nObjAddrs[] = n_parseSchemas (nativeObj, text); 
        MetadataSchema schemas[] = new MetadataSchema [nObjAddrs.length];
        
        for (int i = 0; i < schemas.length; i++)
        {
            schemas[i] = new MetadataSchema (nObjAddrs[i]);
        }
        
        return schemas;
    }

    @Override
    public MetadataInternal[] parseMetadata (String text)
    {
        long nObjAddrs[] = n_parseMetadata (nativeObj, text);
        MetadataInternal mdInt[] = new MetadataInternal [nObjAddrs.length];
        
        for (int i = 0; i < nObjAddrs.length; i++)
        {
            mdInt[i] = new MetadataInternal (nObjAddrs[i]);
        }
        
        return mdInt;
    }

    @Override
    public MetadataStream.VideoSegment[] parseVideoSegments (String text)
    {
        long nObjAddrs[] = n_parseVideoSegments (nativeObj, text);
        MetadataStream.VideoSegment videoSegments[] = new MetadataStream.VideoSegment[nObjAddrs.length];
        
        for (int i = 0; i < nObjAddrs.length; i++)
        {
            videoSegments[i] = new MetadataStream.VideoSegment(nObjAddrs[i]);
        }
        
        return videoSegments;
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }

    private static native long n_JSONReader();
    private static native long[] n_parseSchemas (long nativeObj, String text);
    private static native long[] n_parseMetadata (long nativeObj, String text);
    private static native long[] n_parseVideoSegments (long nativeObj, String text);
    private static native void n_delete (long nativeObj);
}
