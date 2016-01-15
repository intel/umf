package com.intel.vmf;

public class FieldValue
{
    static
    {
        System.loadLibrary ("vmf");
    }

    protected final long nativeObj;
    
    protected FieldValue (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }

    public FieldValue ()
    {
        this (n_FieldValue ());
    }

    public FieldValue (String newName, Variant variant)
    { 
        this (n_FieldValue (newName, variant.nativeObj));
    }

    /*
     * public FieldValue (FieldValue other)
     * {
     * nativeObj = n_FieldValue (other.nativeObj);
     * }
     */

    public String getName ()
    {
        return n_getName (nativeObj); 
    }

    public boolean equals (FieldValue other)
    {
        return n_equals (nativeObj, other.nativeObj);
    }

    public void setTo (Variant value)
    {
        n_setTo (nativeObj, value.nativeObj);
    }

    public void clear ()
    {
        setTo (new Variant());
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private static native long n_FieldValue ();
    //private static native long n_FieldValue (long other);
    private static native long n_FieldValue (String name, long variant);
    private static native String n_getName (long nativeObj); 
    private static native void n_setTo (long nativeObj, long other);
    private static native boolean n_equals (long nativeObj, long other);
    private static native void n_delete (long nativeObj);	
}
