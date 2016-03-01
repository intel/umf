package com.intel.vmf;

public class JSONWriter implements IWriter
{
    static
    {
        try
        {
            System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError error1)
        {
            try
            {
                System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
            }
            catch (UnsatisfiedLinkError error2)
            {
                System.out.println("Native dynamic library is not found");
            }
        }
    }

    protected final long nativeObj;
    
    public JSONWriter ()
    {
        nativeObj = n_JSONWriter();
    }
    
    @Override
    public long getNativeAddr()
    {
        return nativeObj;
    }
    
    @Override
    public String store (MetadataSchema... schemas)
    {
        long schemaNativeAddrs[] = new long [schemas.length];
        
        for (int j = 0; j < schemas.length; j++)
        {
            schemaNativeAddrs[j] = schemas[j].nativeObj;
        }
        
        return n_storeSchemas (nativeObj, schemaNativeAddrs);
    }

    @Override
    public String store (MetadataSet set)
    {
        return n_storeMetadataSet (nativeObj, set.nativeObj);
    }

    @Override
    public String store (long nextId,
                        String filepath,
                        String checksum,
                        MetadataStream.VideoSegment[] segments,
                        MetadataSchema[] schemas,
                        MetadataSet set)
    {
        long segNativeAddrs[] = new long [segments.length];
        long schemaNativeAddrs[] = new long [schemas.length];
        
        for (int i = 0; i < segments.length; i++)
        {
            segNativeAddrs[i] = segments[i].nativeObj;
        }
        
        for (int j = 0; j < schemas.length; j++)
        {
            schemaNativeAddrs[j] = schemas[j].nativeObj;
        }
        
        return n_storeAll (nativeObj, nextId, filepath, checksum, segNativeAddrs, schemaNativeAddrs, set.nativeObj);
    }

    @Override
    public String store (MetadataStream.VideoSegment... segments)
    {
        long segNativeAddrs[] = new long [segments.length];
        
        for (int i = 0; i < segments.length; i++)
        {
            segNativeAddrs[i] = segments[i].nativeObj;
        }
        
        return n_storeSegments (nativeObj, segNativeAddrs);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private static native long n_JSONWriter();
    private static native String n_storeSchemas (long nativeObj, long schemaNativeAddrs[]);
    private static native String n_storeMetadataSet (long nativeObj, long setAddr);
    private static native String n_storeAll (long nativeObj, long nextId, String filepath, String checksum, long segNativeAddrs[], long schemaNativeAddrs[], long setNativeAddr);
    private static native String n_storeSegments (long nativeObj, long segNativeAddrs[]);
    private static native void n_delete (long nativeObj);
}
