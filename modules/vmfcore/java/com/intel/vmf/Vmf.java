package com.intel.vmf;

public class Vmf
{
    static
    {
        System.loadLibrary("vmf");
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