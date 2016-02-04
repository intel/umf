package com.intel.vmf;

public class JSONReader implements IReader
{
    static
    {
        try
        {
            System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError e)
        {
            System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
        }
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
        
        if (nObjAddrs == null)
        	return null;
        
        MetadataSchema schemas[] = new MetadataSchema [nObjAddrs.length];
        
        for (int i = 0; i < schemas.length; i++)
        {
            schemas[i] = new MetadataSchema (nObjAddrs[i]);
        }
        
        return schemas;
    }

    @Override
    public MetadataInternal[] parseMetadata (String text, MetadataSchema... schemas)
    {
    	long nSchemaAddrs[] = new long [schemas.length];
    	
    	for (int i = 0; i < schemas.length; i++)
        {
    		nSchemaAddrs[i] = schemas[i].nativeObj;
        }
    	
        long nMdIntAddrs[] = n_parseMetadata (nativeObj, text, nSchemaAddrs);
        
        if (nMdIntAddrs == null)
        	return null;
        
        MetadataInternal mdInt[] = new MetadataInternal [nMdIntAddrs.length];
        
        for (int j = 0; j < nMdIntAddrs.length; j++)
        {
            mdInt[j] = new MetadataInternal (nMdIntAddrs[j]);
        }
        
        return mdInt;
    }

    @Override
    public MetadataStream.VideoSegment[] parseVideoSegments (String text)
    {
        long nObjAddrs[] = n_parseVideoSegments (nativeObj, text);
        
        if (nObjAddrs == null)
        	return null;
        
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
    private static native long[] n_parseMetadata (long nativeObj, String text, long nSchemaAddrs[]);
    private static native long[] n_parseVideoSegments (long nativeObj, String text);
    private static native void n_delete (long nativeObj);
}
