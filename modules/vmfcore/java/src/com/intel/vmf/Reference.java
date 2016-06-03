package com.intel.vmf;

public class Reference
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

    protected Reference (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");

        nativeObj = addr;
    }

    public Reference ()
    {
        this (n_Reference());
    }

    public Reference (ReferenceDesc desc, Metadata md)
    {
        this (n_Reference (desc.nativeObj, md.nativeObj));
    }

    public void clear ()
    {
        throw new java.lang.UnsupportedOperationException("Not Yet Implemented");
    }

    public Metadata getReferenceMetadata ()
    {
    	long nativeAddr = n_getMetadata (nativeObj);

    	if (nativeAddr == 0)
			return null;

        return new Metadata (nativeAddr);
    }

    public ReferenceDesc getReferenceDescription ()
    {
    	long nativeAddr = n_getReferenceDescription (nativeObj);

    	if (nativeAddr == 0)
			return null;

        return new ReferenceDesc (nativeAddr);
    }

    public void setReferenceMetadata (Metadata md)
    {
        n_setMetadata (nativeObj, md.nativeObj);
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private native static long n_Reference();
    private native static long n_Reference (long refDescAddr, long mdAddr);
    private native static long n_getMetadata (long nativeObj);
    private native static void n_setMetadata (long nativeObj, long mdAddr);
    private native static long n_getReferenceDescription (long nativeObj);
    private native static void n_delete (long nativeObj);
}
