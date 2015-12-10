 package com.intel.vmf;
 
 public class Variant
 {
    static
    {
        System.loadLibrary("vmf");
    }
	
    public enum Type
    {
    	type_unknown,
        type_char,
        type_integer,
        type_real,
        type_string,
        type_vec2d,
        type_vec3d,
        type_vec4d,
        type_rawbuffer,
        type_char_vector,
        type_integer_vector,
        type_real_vector,
        type_string_vector,
        type_vec2d_vector,
        type_vec3d_vector,
        type_vec4d_vector
    }
    
    protected final long nativeObj;
	
    private Variant (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        nativeObj = addr;
    }
    
	public Variant ()
	{
	    nativeObj = n_Variant ();
	}
	
	public Variant ( final int value )
	{
		nativeObj = n_Variant ( value );
	}
	
	public Variant ( final float value )
	{
		nativeObj = n_Variant ( value );
	}
	
	public Variant ( int[] array )
	{
		nativeObj = n_Variant (array);
	}
	
	public Variant ( float[] array )
	{
		nativeObj = n_Variant (array);
	}
	
	public Variant ( String pszString )
	{
		nativeObj = n_Variant (pszString);
	}
	
	public void copyTo (final Variant other)
	{
		n_copyTo (nativeObj, other.nativeObj);
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
	
	public boolean equalTo (Variant other)
	{
		return n_equalTo (nativeObj, other.nativeObj);
	}
	
	public boolean unequalTo (Variant other)
	{
		return n_unequalTo (nativeObj, other.nativeObj);
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
	
	public void dispose ()
	{
		n_delete (nativeObj);
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
		return n_isConverible (srcType, tarType);
	}
	
	public static String base64Encode (vmf_rawbuffer struct)
	{
		return n_base64Encode (struct.data, struct.size);
	}
	
	static vmf_rawbuffer base64Decode (String base64Str)
	{
		vmf_rawbuffer buffer = new vmf_rawbuffer();
		
		int[] size;
		n_base64Decode (base64Str, buffer.data, size);
		buffer.size = size[0]; 
		return buffer;
	}
	
    @Override
    protected void finalize () throws Throwable 
    {
        n_delete (nativeObj);
        super.finalize();
    }
	
	private native long n_Variant ();
	private native long n_Variant (final int value);
	private native long n_Variant (final float value);
	private native long n_Variant (int[] array);
	private native long n_Variant (float[] array);
	private native long n_Variant (String pszString);
	private native static void n_copyTo (long nativeObj, long other);
	private native static long n_setTo (long nativeObj, int value);
	private native static long n_setTo (long nativeObj, float value);
	private native static long n_setTo (long nativeObj, int[] array);
	private native static long n_setTo (long nativeObj, float[] array);
	private native static long n_setTo (long nativeObj, String str);
	private native static long n_setTo (long nativeObj, long other);
	private native static boolean n_equalTo (long nativeObj, long other);
	private native static boolean n_unequalTo (long nativeObj, long other);
	private native static String n_toString (long nativeObj);
	private native static void n_fromString (long nativeObj, int type, String str);
	private native static int n_getType (long nativeObj);
	private native static boolean n_isEmpty (long nativeObj);
	private native static String n_getTypeName(long nativeObj);
	private native static void n_convertTo (long nativeObj, int type);
	private native static String n_typeToString (int type);
	private native static int n_typeFromString (String fieldType);
	private native static boolean n_isConverible (int srcType, int tarType);
	private native static String n_base64Encode (ByteBuffer buffer, long size);
	//????private native static vmf_rawbuffer n_base64Decode (String buffer);
	private native static void n_base64Decode (String str, ByteBuffer buffer, int[] size);
	private native static void n_delete (long nativeObj);
 }
 