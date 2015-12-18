//package com.intel.vmf;

import java.nio.ByteBuffer;

 public class Variant
 {
    static
    {
        System.loadLibrary("vmf");
    }
	
    public static final int type_unknown = 0;
    public static final int type_char = 1;
    public static final int type_integer = 2;
    public static final int type_real = 3;
    public static final int type_string = 4;
    public static final int type_vec2d = 5;
    public static final int type_vec3d = 6;
    public static final int type_vec4d = 7;
    public static final int type_rawbuffer = 8;
    public static final int type_char_vector = 9;
    public static final int type_integer_vector = 10;
    public static final int type_real_vector = 11;
    public static final int type_string_vector = 12;
    public static final int type_vec2d_vector = 13;
    public static final int type_vec3d_vector = 14;
    public static final int type_vec4d_vector = 15;
    
    protected final long nativeObj;
	
    protected Variant (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public Variant ()
    {
        nativeObj = n_Variant ();
    }
	
    public Variant (int value)
    {
	    nativeObj = n_Variant ();
	    n_setTo (nativeObj, value);
    }
	
    public Variant (float value)
    {
        nativeObj = n_Variant ();
        n_setTo (nativeObj, value);
	}
	
	public Variant (int[] array)
	{
		nativeObj = n_Variant ();
		n_setTo (nativeObj, array);
	}
	
	public Variant (float[] array)
	{
		nativeObj = n_Variant ();
		n_setTo (nativeObj, array);
	}
	
	public Variant (String str)
	{
		nativeObj = n_Variant ();
		n_setTo (nativeObj, str);
	}
	
	public void copy (Variant other)
	{
	    //if (nativeObj == 0)
          //  throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_copy (nativeObj, other.nativeObj);
	}
	
	//setTo is analog for operator= 
	public void setTo (int value)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, value);
	}
	
	//setTo is analog for operator=
	public void setTo (float value)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, value);
	}
	
	public void setTo (int[] array)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, array);
	}
	
	public void setTo (float[] array)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, array);
	}
	
	public void setTo (String str)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, str);
	}
	
	public void setTo (Variant other)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, other.nativeObj);
	}
	
	public boolean equals (Variant other)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		return n_equals (nativeObj, other.nativeObj);
	}
	
	public String toString ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		return n_toString (nativeObj);
	}
	
	public void fromString (int type, String str)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_fromString (nativeObj, type, str);
	}
	
	public int getType ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		return n_getType (nativeObj);
	}
	
	public boolean isEmpty ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		return n_isEmpty (nativeObj);
	}
	
	public String getTypeName ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		return n_getTypeName (nativeObj);
	}
	
	public void clear ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_setTo (nativeObj, n_Variant ());
	}
	
	public void convertTo (int type)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
	    
		n_convertTo (nativeObj, type);
	}
	
	public static String typeToString (int type)
	{   
		return n_typeToString (type);
	}

	public static int typeFromString (String fieldType)
	{
		return n_typeFromString (fieldType);
	}
	
	public static boolean isConvertible (int srcType, int tarType)
	{   
		return n_isConvertible (srcType, tarType);
	}
	
	public static String base64Encode (vmfRawBuffer buf)
	{   
		return n_base64Encode (buf.byteBuf, buf.size);
	}
	
	public static vmfRawBuffer base64Decode (String base64Str)
	{
		vmfRawBuffer buf = new vmfRawBuffer ();
		int size[] = new int[1];
		buf.byteBuf = n_base64Decode (base64Str, size);
		buf.size = size[0]; 
		return buf;
	}
	
    @Override
    protected void finalize () throws Throwable 
    {
    	if (nativeObj != 0)
            n_delete (nativeObj);
    	
        super.finalize();
    }
	
	private native long n_Variant ();
	private native static void n_copy (long nativeObj, long other);
	private native static void n_setTo (long nativeObj, int value);
	private native static void n_setTo (long nativeObj, float value);
	private native static void n_setTo (long nativeObj, int array[]);
	private native static void n_setTo (long nativeObj, float array[]);
	private native static void n_setTo (long nativeObj, String str);
	private native static void n_setTo (long nativeObj, long other);
	private native static boolean n_equals (long nativeObj, long other);
	private native static String n_toString (long nativeObj);
	private native static void n_fromString (long nativeObj, int type, String str);
	private native static int n_getType (long nativeObj);
	private native static boolean n_isEmpty (long nativeObj);
	private native static String n_getTypeName (long nativeObj);
	private native static void n_convertTo (long nativeObj, int type);
	private native static String n_typeToString (int type);
	private native static int n_typeFromString (String fieldType);
	private native static boolean n_isConvertible (int srcType, int tarType);
	private native static String n_base64Encode (ByteBuffer buf, long size);
	private native static ByteBuffer n_base64Decode (String str, int size[]);
	private native static void n_delete (long nativeObj);
 }
