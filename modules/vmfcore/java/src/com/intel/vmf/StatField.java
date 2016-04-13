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

    public static final String BuiltinOp_Min     = "com.intel.statistics.builtin_operation.Min";
    public static final String BuiltinOp_Max     = "com.intel.statistics.builtin_operation.Max";
    public static final String BuiltinOp_Average = "com.intel.statistics.builtin_operation.Average";
    public static final String BuiltinOp_Count   = "com.intel.statistics.builtin_operation.Count";
    public static final String BuiltinOp_Sum     = "com.intel.statistics.builtin_operation.Sum";
    public static final String BuiltinOp_Last    = "com.intel.statistics.builtin_operation.Last";

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

    public String getFieldName() {
        return n_getFieldName(nativeObj);
    }

    public String getOpName() {
        return n_getOpName(nativeObj);
    }

    public Variant getValue() {
        long addr = n_getValue(nativeObj);
        if(addr == 0) return null;
        else return new Variant( addr );
    }
    
    public boolean equals (StatField other)
    {
        return n_equals (nativeObj, other.nativeObj);
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
    private native static String n_getFieldName(long nativeObj);
    private native static String n_getOpName(long nativeObj);
    private native static long n_getValue(long nativeObj);
    private native static boolean n_equals(long self, long other);
}
