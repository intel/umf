package com.intel.vmf;

public abstract class Encryptor {

    public final long nativeObj;
    
    protected Encryptor(long addr) {
        if (addr == 0) throw new java.lang.IllegalArgumentException("Native object address is NULL");
        nativeObj = addr;
    }

    public abstract Variant encrypt(String text);
    public abstract String decrypt(Variant data);
    public abstract String getHint();
    
    @Override
    protected void finalize () throws Throwable
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

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

    private native static void n_delete(long nativeObj);
}
