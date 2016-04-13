package com.intel.vmf;

import com.intel.vmf.Vmf;

 public class Variant
 {
     static
     {
         try
         {
             System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
         }
         catch (UnsatisfiedLinkError error1)
         {
             try
             {
                 System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
             }
             catch (UnsatisfiedLinkError error2)
             {
                 throw new java.lang.LinkageError("Native dynamic library is not found");
             }
         }
     }

    public static final int type_empty = 0;
    public static final int type_integer = 1;
    public static final int type_real = 2;
    public static final int type_string = 3;
    public static final int type_vec2d = 4;
    public static final int type_vec3d = 5;
    public static final int type_vec4d = 6;
    public static final int type_rawbuffer = 7;
    public static final int type_integer_vector = 8;
    public static final int type_real_vector = 9;
    public static final int type_string_vector = 10;
    public static final int type_vec2d_vector = 11;
    public static final int type_vec3d_vector = 12;
    public static final int type_vec4d_vector = 13;
    
    protected final long nativeObj;

    protected Variant (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public Variant ()
    {
        this (n_Variant ());
    }

    public Variant (int value)
    {
        this (n_Variant ());
        n_setTo (nativeObj, value);
    }

    public Variant (float value)
    {
        this (n_Variant ());
        n_setTo (nativeObj, value);
    }
    
    public Variant (double value)
    {
        this (n_Variant ());
        n_setTo (nativeObj, value);
    }
    
    public Variant (String str)
    {
        this (n_Variant ());
        n_setTo (nativeObj, str);
    }

    public Variant (vmf_vec2d vec2d)
    {
        this (n_Variant ());
        n_setToVec2d (nativeObj, vec2d.nativeObj);
    }
    
    public Variant (vmf_vec3d vec3d)
    {
        this (n_Variant ());
        n_setToVec3d (nativeObj, vec3d.nativeObj);
    }
    
    public Variant (vmf_vec4d vec4d)
    {
        this (n_Variant ());
        n_setToVec4d (nativeObj, vec4d.nativeObj);
    }
    
    public Variant (int array[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, array);
    }
    
    public Variant (long array[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, array);
    }
    
    public Variant (double array[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, array);
    }
    
    public Variant (float array[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, array);
    }
    
    public Variant (String array[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, array);
    }
    
    public Variant (byte buffer[])
    {
        this (n_Variant ());
        n_setTo (nativeObj, buffer);
    }
    
    public Variant (vmf_vec2d vec2d[])
    {
        this (n_Variant ());
        
        long nObjs[] = new long [vec2d.length];
        
        for (int i = 0; i < vec2d.length; i++)
        {
            nObjs[i] = vec2d[i].nativeObj;
        }
        
        n_setToVec2dArray (nativeObj, nObjs);
    }
    
    public Variant (vmf_vec3d vec3d[])
    {
        this (n_Variant ());
        
        long nObjs[] = new long [vec3d.length];
        
        for (int i = 0; i < vec3d.length; i++)
        {
            nObjs[i] = vec3d[i].nativeObj;
        }
        
        n_setToVec3dArray (nativeObj, nObjs);
    }
    
    public Variant (vmf_vec4d vec4d[])
    {
        this (n_Variant ());
        
        long nObjs[] = new long [vec4d.length];
        
        for (int i = 0; i < vec4d.length; i++)
        {
            nObjs[i] = vec4d[i].nativeObj;
        }
        
        n_setToVec4dArray (nativeObj, nObjs);
    }
    
   /*
    * public Variant (Variant other)
    *{
    *    this (n_Variant (other.nativeObj));
    *}
   */
    
    public void setTo (int value)
    {
        n_setTo (nativeObj, value);
    }
    
    public void setTo (float value)
    {
        n_setTo (nativeObj, value);
    }
    
    public void setTo (double value)
    {
        n_setTo (nativeObj, value);
    }
    
    public void setTo (long value)
    {
        n_setTo (nativeObj, value);
    }
    
    public void setTo (int array[])
    {
        n_setTo (nativeObj, array);
    }

    public void setTo (float array[])
    {
        n_setTo (nativeObj, array);
    }
    
    public void setTo (long array[])
    {
        n_setTo (nativeObj, array);
    }
    
    public void setTo (double array[])
    {
        n_setTo (nativeObj, array);
    }
    
    public void setTo (byte buffer[])
    {
        n_setTo (nativeObj, buffer);
    }
    
    public void setTo (String str)
    {
        n_setTo (nativeObj, str);
    }
    
    public void setTo (String str[])
    {
        n_setTo (nativeObj, str);
    }
    
    public void setTo (vmf_vec2d vec)
    {
    	n_setToVec2d (nativeObj, vec.nativeObj);
    }
    
    public void setTo (vmf_vec3d vec)
    {
    	n_setToVec3d (nativeObj, vec.nativeObj);
    }
    
    public void setTo (vmf_vec4d vec)
    {
    	n_setToVec4d (nativeObj, vec.nativeObj);
    }
    
    public void setTo (vmf_vec2d vec[])
    {
        long nObjs[] = new long [vec.length];
        
        for (int i = 0; i < vec.length; i++)
        {
            nObjs[i] = vec[i].nativeObj;
        }
        
        n_setToVec2dArray (nativeObj, nObjs);
    }
    
    public void setTo (vmf_vec3d vec[])
    {
        long nObjs[] = new long [vec.length];
        
        for (int i = 0; i < vec.length; i++)
        {
            nObjs[i] = vec[i].nativeObj;
        }
        
        n_setToVec3dArray (nativeObj, nObjs);
    }
    
    public void setTo (vmf_vec4d vec[])
    {
        long nObjs[] = new long [vec.length];
        
        for (int i = 0; i < vec.length; i++)
        {
            nObjs[i] = vec[i].nativeObj;
        }
        
        n_setToVec4dArray (nativeObj, nObjs);
    }
    
    public void setTo (Variant other)
    {
        n_set (nativeObj, other.nativeObj);
    }

    public boolean equals (Variant other)
    {
        return n_equals (nativeObj, other.nativeObj);
    }

    public String toString (boolean withType)
    {
        return n_toString (nativeObj, withType);
    }

    public String toString ()
    {
        return toString (false);
    }

    public void fromString (int type, String str)
    {
        n_fromString (nativeObj, type, str);
    }
    
    public void fromString (String str)
    {
        n_fromString2 (nativeObj, str);
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
        return n_getTypeName (nativeObj);
    }

    public void clear ()
    {
        n_set (nativeObj, (new Variant()).nativeObj);
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
    
    public static String base64Encode (byte buf[])
    {   
        return n_base64Encode (buf, buf.length);
    }
    
    public static byte[] base64Decode (String base64Str)
    {
        byte buf[] = n_base64Decode (base64Str); 
        return buf;
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }
    
    private native static long n_Variant ();
    //private native static long n_Variant (long other);
    
    private native static void n_setTo (long nativeAddr, int value);
    private native static void n_setTo (long nativeAddr, long value);
    private native static void n_setTo (long nativeAddr, float value);
    private native static void n_setTo (long nativeAddr, double value);
    private native static void n_setTo (long nativeAddr, String str);
    
    private native static void n_setTo (long nativeAddr, int array[]);
    private native static void n_setTo (long nativeAddr, long array[]);
    private native static void n_setTo (long nativeAddr, float array[]);
    private native static void n_setTo (long nativeAddr, double array[]);
    private native static void n_setTo (long nativeAddr, byte buffer[]);
    private native static void n_setTo (long nativeAddr, String str[]);
    
    private native static void n_setToVec2d (long nativeAddr, long vec2dAddr);
    private native static void n_setToVec3d (long nativeAddr, long vec3dAddr);
    private native static void n_setToVec4d (long nativeAddr, long vec4dAddr);
    private native static void n_setToVec2dArray (long nativeObj, long vec2dAddr[]);
    private native static void n_setToVec3dArray (long nativeObj, long vec3dAddr[]);
    private native static void n_setToVec4dArray (long nativeObj, long vec4dAddr[]);
    
    private native static void n_set (long nativeAddr, long otherAddr);
    private native static boolean n_equals (long nativeObj, long other);
    private native static String n_toString (long nativeObj, boolean withType);
    private native static void n_fromString (long nativeObj, int type, String str);
    private native static void n_fromString2 (long nativeObj, String str);
    private native static int n_getType (long nativeObj);
    private native static boolean n_isEmpty (long nativeObj);
    private native static String n_getTypeName (long nativeObj);
    private native static void n_convertTo (long nativeObj, int type);
    private native static String n_typeToString (int type);
    private native static int n_typeFromString (String fieldType);
    private native static boolean n_isConvertible (int srcType, int tarType);
    private native static String n_base64Encode (byte buf[], int length);
    private native static byte[] n_base64Decode (String str);
    private native static void n_delete (long nativeObj);
 }
