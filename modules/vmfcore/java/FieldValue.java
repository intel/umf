//package com.intel.vmf;

public class FieldValue extends Variant
{
	static
    {
        System.loadLibrary ("vmf");
    }
	
	protected final long nativeObj;
	
	protected FieldValue (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
	
	public FieldValue ()
	{
		nativeObj = n_FieldValue ();
	}
	
	public FieldValue (FieldValue other)
	{
		if (other == null)
            throw new java.lang.UnsupportedOperationException("Object address is NULL");
		
		nativeObj = n_FieldValue (other.nativeObj);
	}
	
	public FieldValue (String newName, Variant variant)
	{
		if (variant == null)
            throw new java.lang.UnsupportedOperationException("Object address is NULL");
		
		
		nativeObj = n_FieldValue (newName, variant.nativeObj);
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
	    // TO DO implementation
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
	private static native long n_FieldValue (String name, long variant);
	private static native String n_getName (long nativeObj); 
	private static native boolean n_isEqual (long nativeObj, long other);
	private static native void n_equals (long nativeObj, long other);
	private static native void n_delete (long nativeObj);	
}
