package com.intel.vmf;

public class vmf_vec3d
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    public final long nativeObj;
    
    protected vmf_vec3d (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public vmf_vec3d ()
    {
        nativeObj = n_vmf_vec3d ();
    }
    
    public vmf_vec3d (double x, double y, double z)
    {
        nativeObj = n_vmf_vec3d (x, y, z);
    }
    
    public void setTo (vmf_vec3d other)
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
    
    private native static long n_vmf_vec3d ();
    private native static long n_vmf_vec3d (double x, double y, double z);
    private native static void n_setTo (long nativeAddr, long otherAddr);
    private native static double[] n_get (long nativeAddr);
    private native static void n_delete (long nativeAddr);
}
