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
    
    
    protected long nativeObj;
	
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
		nativeObj = n_setTo (nativeObj, value);
		//nativeObj = n_Variant (value);
	}
	
	public Variant (float value)
	{
		nativeObj = n_Variant ();
		nativeObj = n_setTo (nativeObj, value);
		//nativeObj = n_Variant ( value );
	}
	
	public Variant (int[] array)
	{
		nativeObj = n_Variant ();
		nativeObj = n_setTo (nativeObj, array);
		//nativeObj = n_Variant (array);
	}
	
	public Variant (float[] array)
	{
		nativeObj = n_Variant ();
		nativeObj = n_setTo (nativeObj, array);
		//nativeObj = n_Variant (array);
	}
	
	public Variant (String str)
	{
		nativeObj = n_Variant ();
		nativeObj = n_setTo (nativeObj, str);
		//nativeObj = n_Variant (str);
	}
	
	public void copy (final Variant other)
	{
		n_copy (nativeObj, other.nativeObj);
	}
	
	public Variant setTo (int value)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, value));
		return retVal;
	}
	
	public Variant setTo (float value)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, value));
		return retVal;
	}
	
	public Variant setTo (int[] array)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, array));
		return retVal;
	}
	
	public Variant setTo (float[] array)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, array));
		return retVal;
	}
	
	public Variant setTo (String str)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, str));
		return retVal;
	}
	
	public Variant setTo (Variant other)
	{
		Variant retVal = new Variant (n_setTo (nativeObj, other.nativeObj));
		return retVal;
	}
	
	public boolean equals (Variant other)
	{
		return n_equals (nativeObj, other.nativeObj);
	}
	
	public boolean unequals (Variant other)
	{
		return n_unequals (nativeObj, other.nativeObj);
	}
	
	public String toString ()
	{
		return n_toString (nativeObj);
	}
	
	public void fromString (int type, String str)
	{
		n_fromString (nativeObj, type, str);
	}
	
	public int getType ()
	{
		return n_getType (nativeObj);
	}
	
	public boolean isEmpty ()
	{
		return n_isEmpty (nativeObj);
	}
	
	public String getTypeName ()
	{
		return n_getTypeName(nativeObj);
	}
	
	public void clear ()
	{
		n_delete (nativeObj);
		nativeObj = n_Variant ();
	}
	
	public void convertTo (int type)
	{
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
	
	static vmfRawBuffer base64Decode (String base64Str)
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
	/*private native long n_Variant (final int value);
	private native long n_Variant (final float value);
	private native long n_Variant (int[] array);
	private native long n_Variant (float[] array);
	private native long n_Variant (String str);*/
	private native static void n_copy (long nativeObj, long other);
	private native static long n_setTo (long nativeObj, int value);
	private native static long n_setTo (long nativeObj, float value);
	private native static long n_setTo (long nativeObj, int array[]);
	private native static long n_setTo (long nativeObj, float array[]);
	private native static long n_setTo (long nativeObj, String str);
	private native static long n_setTo (long nativeObj, long other);
	private native static boolean n_equals (long nativeObj, long other);
	private native static boolean n_unequals (long nativeObj, long other);
	private native static String n_toString (long nativeObj);
	private native static void n_fromString (long nativeObj, int type, String str);
	private native static int n_getType (long nativeObj);
	private native static boolean n_isEmpty (long nativeObj);
	private native static String n_getTypeName(long nativeObj);
	private native static void n_convertTo (long nativeObj, int type);
	private native static String n_typeToString (int type);
	private native static int n_typeFromString (String fieldType);
	private native static boolean n_isConvertible (int srcType, int tarType);
	private native static String n_base64Encode (ByteBuffer buf, long size);
	//????private native static vmf_rawbuffer n_base64Decode (String buffer);
	private native static ByteBuffer n_base64Decode (String str, int size[]);
	private native static void n_delete (long nativeObj);
 }
 