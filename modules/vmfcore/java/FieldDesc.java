//package com.intel.vmf;

public class FieldDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
	
    protected final long nativeObj;
    
    public FieldDesc ()
    {
        nativeObj = n_FieldDesc ();
    }
	
    public FieldDesc (String newName, int newType, boolean isOptional)
    {
        nativeObj = n_FieldDesc (newName, newType, isOptional);
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
    
    private native long n_FieldDesc ();
    private native long n_FieldDesc (String newName, long newType, boolean isOptional);
    private static native boolean n_equals (long nativeObjAddr, long otherAddr);
    private static native String n_getName (long nativeAddr);
    private static native int n_getType (long nativeAddr);
    private static native boolean n_isOptional (long nativeAddr);
    private static native void n_delete (long nativeAddr);
}
