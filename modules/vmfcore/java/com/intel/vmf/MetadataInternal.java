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
        n_MetadataInternal (this.nativeObj, mdDesc.nativeObj);
    }
    
    public MetadataInternal (MetadataInternal other)
    {
        super (other);
        n_MetadataInternal (this.nativeObj, other.nativeObj);
    }
    
    private native static long n_MetadataInternal (long nativeAddr, long addr);
    private native static long n_MetadataInternalCopy (long nativeAddr, long otherObj);
}
