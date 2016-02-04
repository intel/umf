package com.intel.vmf;

public class FieldDesc
{
    static
    {
    	try
    	{
    		System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
    	}
    	catch (UnsatisfiedLinkError e)
    	{
    		System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
    	}
    }
	
    protected final long nativeObj;
    
    protected FieldDesc (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public FieldDesc ()
    {
        this (n_FieldDesc());
    }
	
    public FieldDesc (String name, int type)
    {
        boolean isOptional = false;
        nativeObj = n_FieldDesc (name, type, isOptional);
        
        if (nativeObj == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
    }
    
    public FieldDesc (String name, int type, boolean isOptional)
    {
        this (n_FieldDesc (name, type, isOptional));
    }
	
    public boolean equals (FieldDesc other)
    {
        return n_equals (nativeObj, other.nativeObj);
    }
    
    public String getName ()
    {
        return n_getName (nativeObj);
    }
    
    public int getType ()
    {
        return n_getType (nativeObj);
    }
    
    public boolean isOptional ()
    {
        return n_isOptional (nativeObj);
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private static native long n_FieldDesc ();
    private static native long n_FieldDesc (String name, int type, boolean isOptional);
    private static native boolean n_equals (long nativeObjAddr, long otherAddr);
    private static native String n_getName (long nativeAddr);
    private static native int n_getType (long nativeAddr);
    private static native boolean n_isOptional (long nativeAddr);
    private static native void n_delete (long nativeAddr);
}
