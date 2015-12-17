//package com.intel.vmf;

public class MetadataInternal extends Metadata
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
    
    private native long n_MetadataInternal (long addr);
}