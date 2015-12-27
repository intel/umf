package com.intel.vmf;

public class FieldDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
	
    protected final long nativeObj;
    
    protected FieldDesc (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
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
    
    public void setName (String name)
    {
        n_setName (nativeObj, name);
    }
    
    public void setType (int type)
    {
        n_setType (nativeObj, type);
    }
    
    public void setOptional (boolean flag)
    {
        n_setOptional (nativeObj, flag);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private static native long n_FieldDesc ();
    private static native long n_FieldDesc (String newName, int newType, boolean isOptional);
    private static native boolean n_equals (long nativeObjAddr, long otherAddr);
    private static native String n_getName (long nativeAddr);
    private static native int n_getType (long nativeAddr);
    private static native boolean n_isOptional (long nativeAddr);
    private static native void n_setName (long nativeAddr, String name);
    private static native void n_setType (long nativeObj, int type);
    private static native void n_setOptional (long nativeObj, boolean flag);
    private static native void n_delete (long nativeAddr);
}
