package com.intel.vmf;

public class MetadataSchema
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
                throw new java.lang.LinkageError("Native dynamic library is not found");
            }
        }
    }

    protected final long nativeObj;
    
    public static final int STD_DST = 0;
    
    protected MetadataSchema (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataSchema (String name)
    {
        String author = "";
        nativeObj = n_MetadataSchema (name, author);
    }
    
    public MetadataSchema (String name, String author)
    {
        this (n_MetadataSchema (name, author));
    }
    
    public String getName ()
    {
        return n_getName (nativeObj);
    }
    
    public String getAuthor ()
    {
        return n_getAuthor (nativeObj);
    }
    
    public long size ()
    {
        return n_size (nativeObj);
    }
    
    public void add (MetadataDesc mdDesc)
    {
        n_add (nativeObj, mdDesc.nativeObj);
    }
    
    public MetadataDesc findMetadataDesc (String mdName)
    {
    	long nativeAddr = n_findMetadataDesc (nativeObj, mdName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new MetadataDesc (nativeAddr);
    }
    
    public MetadataDesc[] getAll()
    {
        long nObjs[] = n_getAll (nativeObj);
        
        if (nObjs == null)
        	return null;
        
        MetadataDesc objs[] = new MetadataDesc [nObjs.length];
        
        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new MetadataDesc (nObjs[i]);
        }
        
        return objs;
    }
    
    public static String getStdSchemaName ()
    {
        return n_getStdSchemaName (STD_DST);
    }
    
    public static MetadataSchema getStdSchema ()
    {
    	long nativeAddr = n_getStdSchema (STD_DST);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new MetadataSchema (nativeAddr);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static long n_MetadataSchema (String name, String author);
    private native static String n_getName (long nativeObj);
    private native static String n_getAuthor (long nativeObj);
    private native static long n_size (long nativeObj);
    private native static void n_add (long nativeObj, long mdDescAddr);
    private native static long n_findMetadataDesc (long nativeObj, String mdName);
    private native static long[] n_getAll (long nativeObj);
    private native static String n_getStdSchemaName (int kind);
    private native static long n_getStdSchema (int kind);
    private native static void n_delete (long nativeObj);
}
