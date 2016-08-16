package com.intel.umf;

public class Umf
{
    public static final String NATIVE_LIBRARY_NAME = "vmf";

    static
    {
        try
        {
            System.loadLibrary(NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError error1)
        {
        	error1.printStackTrace();
            try
            {
                System.loadLibrary(Umf.NATIVE_LIBRARY_NAME + "d");
            }
            catch (UnsatisfiedLinkError error2)
            {
                throw new java.lang.LinkageError("Native dynamic library is not found");
            }
        }
    }
    
    public static void initialize ()
    {
        n_initialize ();
    }
    
    public static void terminate ()
    {
        n_terminate ();
    }
    
	private static native void n_initialize ();
    private static native void n_terminate ();
    
    
    public static void main(String args[])
    {
    	initialize();
    	umf_vec3d u=new umf_vec3d();
    	System.out.println(u.getY());
    	terminate();
    }
    
  
}
