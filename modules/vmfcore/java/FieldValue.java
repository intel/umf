//package com.intel.vmf;

public class FieldValue extends Variant
{
	static
    {
        System.loadLibrary ("vmf");
    }
	
	private String name;
	
	protected final long nativeObj;
	
	public FieldValue ()
	{
		nativeObj = n_FieldValue ();
	}
	
	public FieldValue (FieldValue other)
	{
		nativeObj = n_FieldValue (other.nativeObj);
	}
	
	public FieldValue (String newName, Variant variant )
	{
		name = newName;
		nativeObj = n_FieldValue (name, variant);
	}
	
	public String getName ()
	{
		//return m_name;
		return n_getName (nativeObj); 
	}
	
	public boolean isEqual (FieldValue other)
	{
		return n_isEqual (nativeObj, other.nativeObj);
	}
	
	public void equals (FieldValue other)
	{
		n_equals( nativeObj, other.nativeObj );
		return;
	}
	
	@Override
	protected void finalize () throws Throwable 
	{
		if (nativeObj != 0)
	        n_delete (nativeObj);
	    
        super.finalize();
	}
	
	private static native long n_FieldValue ();
	private static native long n_FieldValue (long other);
	private static native long n_FieldValue (String name, Variant variant);
	private static native String n_getName (long nativeObj); 
	private static native boolean n_isEqual (long nativeObj, long other);
	private static native void n_equals (long nativeObj, long other);
	private static native void n_delete (long nativeObj);	
}
