package com.intel.vmf;

public class FieldDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
	
    protected final long nativeObj;
	
	public boolean optional;
	
	public String name;
	
	public Variant.Type type;
	
	public FieldDesc ()
	{
        nativeObj = n_FieldDesc();
	}
	
	public boolean isEquals(long obj)
	{
        return n_isEquals(nativeObj, obj);
	}
	
	@Override
    protected void finalize () throws Throwable 
    {
        n_delete (nativeObj);
        super.finalize();
    }
	
	private native long n_FieldDesc ();
	
	private native boolean n_isEquals(long nativeObj);
	
	private static native void n_delete (long nativeObj);
}