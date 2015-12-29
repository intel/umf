package com.intel.vmf;

public class VmfInit
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    public static void vmfInitialize ()
    {
        n_vmfInitialize ();
    }
    
    public static void vmfTerminate ()
    {
        n_vmfTerminate ();
    }
    
    private static native void n_vmfInitialize ();
    private static native void n_vmfTerminate ();
}