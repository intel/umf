//package com.intel.vmf;

public class MetadataSet implements IQuery
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    public static final long FRAME_COUNT_ALL = Long.MAX_VALUE;
    
    protected MetadataSet (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataSet ()
    {
        nativeObj = n_MetadataSet ();
    }
    
    public void copy (MetadataSet other)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        n_copy (nativeObj, other.nativeObj);
    }
    
    public void setTo (MetadataSet other)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        n_setTo (nativeObj, other.nativeObj);
    }
    
    /*TO DO:
    public MetadataSet query (filter){}
    public MetadataSet queryByReference (Metadata md, filter){}
    */
    
    public MetadataSet queryByFrameIndex (long index)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByFrameIndex (nativeObj, index));
    }
    
    public MetadataSet queryByTime (long startTime, long endTime)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByTime (nativeObj, startTime, endTime));
    }
    
    public MetadataSet queryBySchema (String schemaName)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryBySchema (nativeObj, schemaName));
    }
    
    public MetadataSet queryByName (String name)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByName (nativeObj, name));
    }
    
    public MetadataSet queryByNameAndValue (String mdName, FieldValue value)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByNameAndValue (nativeObj, mdName, value.nativeObj));
    }
    
    public MetadataSet queryByNameAndFields (String mdName, FieldValue fields[])
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        long nativeFieldValueObjs[] = new long [fields.length];
        for (int i = 0; i < fields.length; i++)
        {
            nativeFieldValueObjs[i] = fields[i].nativeObj;
        }
        
        return new MetadataSet (n_queryByNameAndFields (nativeObj, mdName, nativeFieldValueObjs));
    }
    
    public MetadataSet queryByReference (String refName)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByReference (nativeObj, refName));
    }
    
    public MetadataSet queryByReference (String refName, FieldValue value)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByReference (nativeObj, refName, value.nativeObj));
    }
    
    public MetadataSet queryByReference (String refName, FieldValue[] fields)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        long nativeFieldValueObjs[] = new long [fields.length];
        for (int i = 0; i < fields.length; i++)
        {
            nativeFieldValueObjs[i] = fields[i].nativeObj;
        }
        
        return new MetadataSet (n_queryByReference (nativeObj, refName, nativeFieldValueObjs));
    }
    
    public long shift (long nTarFrameIndex, long nSrcFrameIndex)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_shift (nativeObj, nTarFrameIndex, nSrcFrameIndex, FRAME_COUNT_ALL, 0);
    }
    
    public long shift (long nTarFrameIndex, long nSrcFrameIndex, long nNumOfFrames, MetadataSet setFailure)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_shift (nativeObj, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames, setFailure.nativeObj);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native long n_MetadataSet ();
    private native static void n_copy (long nativeObj, long other);
    private native static void n_setTo (long nativeObj, long other);
    private native static long n_queryByFrameIndex (long nativeObj, long index);
    private native static long n_queryByTime (long nativeObj, long startTime, long endTime);
    private native static long n_queryBySchema (long nativeObj, String schemaName);
    private native static long n_queryByName (long nativeObj, String name);
    private native static long n_queryByNameAndValue (long nativeObj, String mdName, long value);
    private native static long n_queryByNameAndFields (long nativeObj, String mdName, long nativeFieldValueObjs[]);
    private native static long n_queryByReference (long nativeObj, String refName);
    private native static long n_queryByReference (long nativeObj, String refName, long value);
    private native static long n_queryByReference (long nativeObj, String refName, long nativeFieldValueObjs[]);
    private native static long n_shift (long nativeObjAddr, long nTarFrameIndex, long nSrcFrameIndex, long nNumOfFrames, long setFailure);
    private native static void n_delete (long nativeObj);
}
