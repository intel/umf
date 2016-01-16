package com.intel.vmf;

public class vmf_vec2d
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    public final long nativeObj;
    
    protected vmf_vec2d (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public vmf_vec2d ()
    {
        this (n_vmf_vec2d ());
    }
    
    public vmf_vec2d (double x, double y)
    {
        this (n_vmf_vec2d (x, y));
    }
    
    public void setTo (vmf_vec2d other)
    {
        n_setTo (nativeObj, other.nativeObj);
    }
    
    public double[] get()
    {
        return n_get (nativeObj);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static long n_vmf_vec2d ();
    private native static long n_vmf_vec2d (double x, double y);
    private native static void n_setTo (long nativeAddr, long otherAddr);
    private native static double[] n_get (long nativeAddr);
    private native static void n_delete (long nativeAddr);
}
