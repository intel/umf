package com.intel.vmf;

public class MetadataSchema
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    public static final int STD_DST = 0;
    
    protected MetadataSchema (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataSchema (String name)
    {
        String author = "";
        nativeObj = n_MetadataSchema (name, author);
    }
    
    public MetadataSchema (String name, String author)
    {
        nativeObj = n_MetadataSchema (name, author);
    }
    
    public String getName ()
    {
        return n_getName (nativeObj);
    }
    
    public String getAuthor()
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
        return new MetadataDesc (n_findMetadataDesc (nativeObj, mdName));
    }
    
    public MetadataDesc[] getAll()
    {
        long nObjs[] = n_getAll (nativeObj);
        MetadataDesc objs[] = new MetadataDesc [nObjs.length];
        
        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new MetadataDesc (nObjs[i]);
        }
        
        return objs;
    }
    
    public String getStdSchemaName ()
    {
        return n_getStdSchemaName (nativeObj, STD_DST);
    }
    
    public MetadataSchema getStdSchema ()
    {
        return new MetadataSchema (n_getStdSchema (nativeObj, STD_DST));
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native long n_MetadataSchema (String name, String author);
    private native static String n_getName (long nativeObj);
    private native static String n_getAuthor (long nativeObj);
    private native static long n_size (long nativeObj);
    private native static void n_add (long nativeObj, long mdDescAddr);
    private native static long n_findMetadataDesc (long nativeObj, String mdName);
    private native static long[] n_getAll (long nativeObj);
    private native static String n_getStdSchemaName (long nativeObj, int kind);
    private native static long n_getStdSchema (long nativeObj, int kind);
    private native static void n_delete (long nativeObj);
}
