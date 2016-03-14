package com.intel.vmf;

public class MetadataInternal
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
    
    protected MetadataInternal (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataInternal (MetadataDesc mdDesc)
    {
        this (n_MetadataInternal (mdDesc.nativeObj));
    }
    
    /*
     * public MetadataInternal (MetadataInternal other)
     * {
     *  nativeObj = n_MetadataInternalCopy (other.nativeObj);
     * }
    */
    
    public MetadataInternal (Metadata md)
    {
        this (n_MetadataInternalByMD (md.nativeObj));
    }
    
    public void clear ()
    {
        throw new java.lang.UnsupportedOperationException("Not Yet Implemented");
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static long n_MetadataInternal (long mdDescAddr);
    //private native static long n_MetadataInternalCopy (long otherAddr);
    private native static long n_MetadataInternalByMD (long mdAddr);
    private native static void n_delete (long nativeObj);
}
