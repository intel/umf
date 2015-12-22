package com.intel.vmf;

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
        n_MetadataInternal (mdDesc.nativeObj);
    }
    
    public MetadataInternal (MetadataInternal other)
    {
        super (other);
        //n_MetadataInternal (Metadata.nativeObj, other.nativeObj);
    }
    
    private native static long n_MetadataInternal (long addr);
    private native static long n_MetadataInternal (long mdAddr, long otherObj);
}
