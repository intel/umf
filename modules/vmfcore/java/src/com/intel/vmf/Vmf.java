package com.intel.vmf;

public class Vmf
{
    public static final String NATIVE_LIBRARY_NAME = "vmf";

    static
    {
        try
        {
            System.loadLibrary(NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError e)
        {
            System.loadLibrary(NATIVE_LIBRARY_NAME + "d");
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
}
