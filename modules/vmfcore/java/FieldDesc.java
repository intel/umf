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
	
    public FieldDesc (String newName, long newType, boolean isOptional)
    {
        nativeObj = n_FieldDesc (newName, newType, isOptional);
    }
	
    public boolean equals (FieldDesc other)
    {
        return n_equals (nativeObj, other.nativeObj);
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
    private static native void n_delete (long nativeObj);
}
