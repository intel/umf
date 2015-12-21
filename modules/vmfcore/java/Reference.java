//package com.intel.vmf;

public class Reference
{
	static
    {
        System.loadLibrary("vmf");
    }
	
	protected final long nativeObj;
	
	protected Reference (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
	
	public Reference ()
	{
		nativeObj = n_Reference();
	}
	
	public Reference (ReferenceDesc desc, Metadata md)
	{	
		if (md.nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		nativeObj = n_Reference (desc.nativeObj, md.nativeObj);
	}
	
	public void clear ()
	{
		//TO DO: implementation
	}
	
	public Metadata getMetadata ()
	{
		return new Metadata (n_getMetadata (nativeObj));
	}
	
	public ReferenceDesc getReferenceDescription ()
	{
		return new ReferenceDesc (n_getReferenceDescription (nativeObj));
	}
	
	public void setMetadata (Metadata md)
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
	
	private native long n_Reference();
	private native long n_Reference (long refDescAddr, long mdAddr);
	private native static long n_getMetadata (long nativeObj);
	private native static void n_setMetadata (long nativeObj, long md);
	private native static long n_getReferenceDescription (long nativeObj);
	private native static void n_delete (long nativeObj);
}
