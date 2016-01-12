package com.intel.vmf;

public class vmf_vec4d
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    public final long nativeObj;
    
    protected vmf_vec4d (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public vmf_vec4d ()
    {
        nativeObj = n_vmf_vec4d ();
    }
    
    public vmf_vec4d (double x, double y, double z, double w)
    {
        nativeObj = n_vmf_vec4d (x, y, z, w);
    }
    
    public void setTo (vmf_vec4d other)
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
    
    private native static long n_vmf_vec4d ();
    private native static long n_vmf_vec4d (double x, double y, double z, double w);
    private native static void n_setTo (long nativeAddr, long otherAddr);
    private native static double[] n_get (long nativeAddr);
    private native static void n_delete (long nativeAddr);
}