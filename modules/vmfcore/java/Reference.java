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
		
		nativeObj = n_Reference (desc.name, desc.isUnique, desc.isCustom, md.nativeObj);
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
		boolean flags[] = new boolean[2];
		String name = new String();
		n_getReferenceDescription (nativeObj, flags, name);
		return new ReferenceDesc (name, flags[0], flags[1]);
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
	private native long n_Reference (String name, boolean unique, boolean custom, long md);
	private native static long n_getMetadata (long nativeObj);
	private native static void n_setMetadata (long nativeObj, long md);
	private native static void n_getReferenceDescription (long nativeObj, boolean flags[], String name);
	private native static void n_delete (long nativeObj);
}
