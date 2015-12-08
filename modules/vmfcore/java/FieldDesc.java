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
    	nativeObj = n_FieldDesc ();
    }
	
    public FieldDesc ( final string sName, Variant.Type eType, boolean isOptional )
    {
        nativeObj = n_FieldDesc(sName, eType, isOptional);
    }
	
    public boolean isEqual ( FieldDesc other )
    {
        //return (name == other.name) && (type == other.type);
        return n_isEqual (nativeObj, obj.nativeObj);
    }
	
    @Override
    protected void finalize () throws Throwable 
    {
        n_delete (nativeObj);
        super.finalize();
    }
    
    private native long n_FieldDesc ();
    private native long n_FieldDesc ( sName, eType, isOptional );
    private native boolean n_isEqual ( long nativeObj, long other );
    private static native void n_delete ( long nativeObj );
}
