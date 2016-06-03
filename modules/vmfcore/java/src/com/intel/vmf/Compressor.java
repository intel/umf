package com.intel.vmf;

public class Compressor {
	
	public static final String BUILTIN_ZLIB = "com.intel.vmf.compressor.zlib";

    public final long nativeObj;

    protected Compressor(long addr) {
        if (addr == 0) throw new java.lang.IllegalArgumentException("Native object address is NULL");
        nativeObj = addr;
    }

    public Variant compress(String input) {
		if(input == null) return null;
    	long addr = n_compress(nativeObj, input);
		if(addr == 0) return null;
		else return new Variant(addr);
	}

	public String decompress(Variant data) {
		if(data == null) return null;
		return n_decompress(nativeObj, data.nativeObj);
	}

	public Compressor createNewInstance() {
		long addr = n_createNewInstance(nativeObj);
		if(addr == 0) return null;
		else return new Compressor(addr);
	}

	public String getId() {
		return n_getId(nativeObj);
	}
	
	public static Compressor create(String id) {
		long addr = n_create(id);
		if(addr == 0) return null;
		else return new Compressor(addr);
	}
	
	public static String[] getRegisteredIds() {
		//return n_getRegisteredIds();
		return new String[] {BUILTIN_ZLIB};
	}
	
	public static String builtinId() {
		//return n_builtinId();
		return BUILTIN_ZLIB;
	}

	// user-provided Compressors support factory methods
    // TODO: NYI
    /*
	public static void registerNew(Compressor compressor) {
		n_registerNew(compressor.nativeObj);
	}
	
	public static void unregister(String id) {
		n_unregister(id);
	}
	
	// native
	
	private static native void n_registerNew(long addr);
	private static native void n_unregister(String id);
	private static native String[] n_getRegisteredIds();
	*/
    // JNI lib loading

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
	private static native long n_compress(long self, String str);
	private static native String n_decompress(long self, long variantAddr);
	private static native long n_createNewInstance(long self);
	private static native String n_getId(long self);
	private static native long n_create(String str);

}
