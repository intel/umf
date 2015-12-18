//package com.intel.vmf;

public class MetadataInternal extends Metadata
{
    
    static
    {
        System.loadLibrary("vmf");
    }
    
    
    protected MetadataInternal (long addr)
    {
        super (addr);
    }
    
    public MetadataInternal (MetadataDesc mdDesc)
    {
        super (mdDesc);
        nativeObj = n_MetadataInternal (mdDesc.nativeObj);
    }
    
    private native long n_MetadataInternal (long addr);
}