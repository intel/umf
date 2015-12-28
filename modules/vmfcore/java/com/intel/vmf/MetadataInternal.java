package com.intel.vmf;

public class MetadataInternal// extends Metadata
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    protected MetadataInternal (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataInternal (MetadataDesc mdDesc)
    {
        nativeObj = n_MetadataInternal (mdDesc.nativeObj);
    }
    
    public MetadataInternal (MetadataInternal other)
    {
        nativeObj = n_MetadataInternalCopy (other.nativeObj);
    }
    
    public MetadataInternal (Metadata md)
    {
        nativeObj = n_MetadataInternalByMD (md.nativeObj);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static long n_MetadataInternal (long mdDescAddr);
    private native static long n_MetadataInternalCopy (long otherAddr);
    private native static long n_MetadataInternalByMD (long mdAddr);
    private native static void n_delete (long nativeObj);
}
