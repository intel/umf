package com.intel.vmf;

public class Stat {

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

    // State
    public static final int State_UpToDate   = 1;
    public static final int State_NeedUpdate = 2;
    public static final int State_NeedRescan = 3;

    // Action
    public static final int Action_Add    = 1;
    public static final int Action_Remove = 2;

    //UpdateMode
    public static final int UpdateMode_Disabled = 1;
    public static final int UpdateMode_Manual   = 2;
    public static final int UpdateMode_OnAdd    = 3;
    public static final int UpdateMode_OnTimer  = 4;

    protected Stat (long addr) {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");

        nativeObj = addr;
    }

    private static long[] fields2Addr(StatField[] fields) {
        long[] fieldsAddr = null;
        if(fields != null && fields.length > 0) {
            fieldsAddr = new long[fields.length];
            int cnt = 0;
            for(StatField sf : fields)
                fieldsAddr[cnt++] = sf.nativeObj;
        }
        return fieldsAddr;
    }

    public Stat(String name, StatField...fields) {
        this( n_Stat(name, Stat.fields2Addr(fields)) );
    }

    public String getName() {
        return n_getName(nativeObj);
    }

    public int getState() {
        return n_getState(nativeObj);
    }

    public void setUpdateMode( int updateMode ) {
        n_setUpdateMode(nativeObj, updateMode);
    }

    public int getUpdateMode() {
        return n_getUpdateMode(nativeObj);
    }

    public void setUpdateTimeout( int ms ) {
        n_setUpdateTimeout(nativeObj, ms);
    }

    public int getUpdateTimeout() {
        return n_getUpdateTimeout(nativeObj);
    }

    public void update( boolean doWait ) {
        n_update(nativeObj, doWait);
    }

    public void update() {
        update(false);
    }

    public void clear() {
        n_clear(nativeObj);
    }

    public String[] getAllFieldNames() {
        return n_getAllFieldNames(nativeObj);
    }

    public StatField getFieldCopy( String name ) {
        return new StatField( n_getField(nativeObj, name) );
    }

    public Variant getValue( String name ) {
        return new Variant( n_getValue(nativeObj, name) );
    }

    @Override
    protected void finalize () throws Throwable
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private native static void n_delete(long nativeObj);
    private native static long n_Stat(String name, long[] fields2Addr);
    private native static String n_getName(long nativeObj);
    private native static int n_getState(long nativeObj);
    private native static void n_setUpdateMode(long nativeObj, int updateMode);
    private native static int n_getUpdateMode(long nativeObj);
    private native static void n_setUpdateTimeout(long nativeObj, int ms);
    private native static int n_getUpdateTimeout(long nativeObj);
    private native static void n_update(long nativeObj, boolean doWait);
    private native static void n_clear(long nativeObj);
    private native static String[] n_getAllFieldNames(long nativeObj);
    private native static long n_getField(long nativeObj, String name);
    private native static long n_getValue(long nativeObj, String name);
}
