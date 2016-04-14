package com.intel.vmf;

public class vmf_vec3d
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
    
    public final long nativeObj;
    
    protected vmf_vec3d (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public vmf_vec3d ()
    {
        this (n_vmf_vec3d ());
    }
    
    public vmf_vec3d (double x, double y, double z)
    {
        this (n_vmf_vec3d (x, y, z));
    }
    
    /*
     * public void setTo (vmf_vec3d other)
     *{
     *    n_setTo (nativeObj, other.nativeObj);
     *}
    */
    
    public double getX()
    {
        return n_getX(nativeObj);
    }
    
    public double getY()
    {
        return n_getY(nativeObj);
    }
    
    public double getZ()
    {
        return n_getZ(nativeObj);
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
    //private native static void n_setTo (long nativeAddr, long otherAddr);
    private native static double n_getX (long nativeAddr);
    private native static double n_getY (long nativeAddr);
    private native static double n_getZ (long nativeAddr);
    private native static void n_delete (long nativeAddr);
}
