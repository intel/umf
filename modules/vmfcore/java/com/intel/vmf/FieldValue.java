package com.intel.vmf;

public class FieldValue extends Variant
{
	static
    {
        System.loadLibrary ("vmf");
    }
	
	protected FieldValue (long addr)
    {
	    super (addr);
    }
	
	public FieldValue ()
	{
	    super ();
		n_FieldValue ();
	}
	
	public FieldValue (String newName, Variant variant)
    { 
        super (variant.nativeObj);
        n_FieldValue (newName, variant.nativeObj);
    }
	
	public FieldValue (FieldValue other)
	{
		n_FieldValue (other.nativeObj);
	}
	
	public String getName ()
	{
		return n_getName (nativeObj); 
	}
	
	public boolean isEqual (FieldValue other)
	{
		return n_isEqual (nativeObj, other.nativeObj);
	}
	
	public void equals (FieldValue other)
	{
		n_equals (nativeObj, other.nativeObj);
	}
	
	public void clear ()
	{
	    // TO DO: implementation
	}
	
	@Override
	protected void finalize () throws Throwable 
	{
		if (nativeObj != 0)
	        n_delete (nativeObj);
	    
        super.finalize();
	}
	
	private static native long n_FieldValue ();
	private static native void n_FieldValue (long other);
	private native long n_FieldValue (String name, long variant);
	private static native String n_getName (long nativeObj); 
	private static native boolean n_isEqual (long nativeObj, long other);
	private static native void n_equals (long nativeObj, long other);
	private static native void n_delete (long nativeObj);	
}
