package com.intel.vmf;

public abstract class Compressor {
	public static final String BUILTIN_ZLIB = "com.intel.vmf.compressor.zlib";
	public abstract Variant compress(String input);
	public abstract String decompress(Variant data);
	public abstract Compressor createNewInstance(); // == clone()
	public abstract String getId();
	
    public final long nativeObj;
    
    protected Compressor(long addr) {
        if (addr == 0) throw new java.lang.IllegalArgumentException("Native object address is NULL");
        nativeObj = addr;
    }
	
	// factory methods
    // TODO: NYI
    /*
	public static void registerNew(Compressor compressor) {
		n_registerNew(compressor.nativeObj);
	}
	
	public static void unregister(String id) {
		n_unregister(id);
	}
	
	public static Compressor create(String id) {
		return n_create(id);
	}
	
	public static String[] getRegisteredIds() {
		return n_getRegisteredIds();
	}
	
	// native
	
	private static native void n_registerNew(long addr);
	private static native void n_unregister(String id);
	private static native Compressor n_create(String id);
	private static native String[] n_getRegisteredIds();
	*/
    // JNI lib loading

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
}
