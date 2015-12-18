//package com.intel.vmf;
 
public class ReferenceDesc
{
    public String name;
    public boolean isUnique;
    public boolean isCustom;

    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    protected ReferenceDesc (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public ReferenceDesc ()
	{
        nativeObj = ReferenceDesc ();
	}
    
    public ReferenceDesc (String refName, boolean unique, boolean custom) 
    {
        nativeObj = ReferenceDesc (refName, unique, custom);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native long ReferenceDesc ();
    private native long ReferenceDesc (String refName, boolean unique, boolean custom);
    private static native void n_delete (long nativeAddr);
}
