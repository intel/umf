package com.intel.vmf;

public class StatField {

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

    protected final long nativeObj;

    protected StatField (long addr) {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }

    public StatField(String name, String schemaName, String metadataName, String fieldName, String opName ) {
    	this( n_StatField(name, schemaName, metadataName, fieldName, opName) );
    }

    public String getName() {
    	return n_getName(nativeObj);
    }
    
    public String getSchemaName() {
    	return n_getSchemaName(nativeObj);
    }
    
    public String getMetadataName() {
    	return n_getMetadataName(nativeObj);
    }
    
    public MetadataDesc getMetadataDesc() {
		return new MetadataDesc( n_getMetadataDesc(nativeObj) );
	}
    
    public String getFieldName() {
    	return n_getFieldName(nativeObj);
    }
    
    public FieldDesc getFieldDesc() {
		return new FieldDesc( n_getFieldDesc(nativeObj) );
	}
    
    public String getOpName() {
    	return n_getOpName(nativeObj);
    }
    
    public Variant getValue() {
    	return new Variant( n_getValue(nativeObj) );
    }
	
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static void n_delete(long nativeObj);
    private native static long n_StatField(String name, String schemaName, String metadataName, String fieldName, String opName);
    private native static String n_getName(long nativeObj);
    private native static String n_getSchemaName(long nativeObj);
    private native static String n_getMetadataName(long nativeObj);
    private native static long n_getMetadataDesc(long nativeObj);
    private native static String n_getFieldName(long nativeObj);
    private native static long n_getFieldDesc(long nativeObj);
    private native static String n_getOpName(long nativeObj);
    private native static long n_getValue(long nativeObj);
}
