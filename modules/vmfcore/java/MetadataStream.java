//package com.intel.vmf;

public class MetadataStream extends IQuery
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    public static final int InMemory = 0;
    public static final int ReadOnly = 1;
    public static final int ReadWrite = 2;
    
    public static final int FRAME_COUNT_ALL = long.MAX_VALUE;
    
    /*public class VideoSegment
    {
        public VideoSegment()
        {
            
        }
    }*/
    
    protected MetadataStream (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataStream ()
    {
        nativeObj = n_MetadataStream ();
    }
    
    public boolean open (String filePath, int mode)
    {
        return n_open (nativeObj, filePath, mode);
    }
    
    public boolean reopen (int mode)
    {
        return n_reopen (nativeObj, mode);
    }
    
    public boolean load (String schemaName)
    {
        return n_load (nativeObj, schemaName);
    }
    
    public boolean load (String schemaName, String mdName)
    {
        return n_load (nativeObj, schemaName, mdName);
    }
    
    public boolean save ()
    {
        return n_save (nativeObj);
    }
    
    public boolean saveTo (String filePath)
    {
        return n_saveTo (nativeObj, filePath);
    }
    
    public void close()
    {
        n_close (nativeObj);
    }
    
    public Metadata getById (long id)
    {
        return new Metadata (n_getById (nativeObj, id));
    }
    
    public long add (Metadata md)
    {
        return n_add (nativeObj, md.nativeObj);
    }
    
    public boolean removeMdById (long id)
    {
        return n_removeMdById (nativeObj, id);
    }
    
    public void removeSet (MetadataSet set)
    {
        n_removeSet (nativeObj, set.nativeObj);
    }
    
    public void removeSchema (MetadataSchema schema)
    {
        n_removeSchema (nativeObj, schema.nativeObj);
    }
    
    public void n_removeAllMd()
    {
        n_removeAllMd (nativeObj);
    }
    
    public void addSchema (MetadataSchema schema)
    {
        n_addSchema (nativeObj, schema.nativeObj);
    }
    
    public MetadataSchema getSchema (String schemaName)
    {
        return new MetadataSchema (n_getSchema (nativeObj, schemaName));
    }
    
    public String[] getAllSchemaNames ()
    {
        return n_getAllSchemaNames (nativeObj);
    }
    
    public MetadataSet getAll()
    {
        return new MetadataSet (n_getAll (nativeObj));
    }
    
    public boolean importSet (MetadataStream srcStream, MetadataSet srcSet,
                              long tarFrameIndex, long srcFrameIndex)
    {
        return n_importSet (nativeObj, srcStream.nativeObj, srcSet.nativeObj,
                            tarFrameIndex, srcFrameIndex, FRAME_COUNT_ALL, null);
    }
    
    public boolean importSet (MetadataStream srcStream, MetadataSet srcSet, long tarFrameIndex, 
                              long srcFrameIndex, long numOfFrames, MetadataSet setFailure)
    {
        return n_importSet (nativeObj, srcStream.nativeObj, srcSet.nativeObj,
                            tarFrameIndex, srcFrameIndex, numOfFrames, setFailure.nativeObj);
    }
    
    public void clear ()
    {
        n_clear (nativeObj);
    }
    
    public void sortMdSetById ()
    {
        n_sortMdSetById (nativeObj);
    }
    
    public String serialize (IWriter formater)
    {
        return n_serialize (nativeObj, formater.nativeObj);
    }
    
    public void deserialize (String text, IReader formater)
    {
        n_deserialize (nativeObj, text, formater.nativeObj);
    }
    
    public String computeChecksum ()
    {
        return n_computeChecksum (nativeObj);
    }
    
    public String computeChecksum (long XMPPacketSize, long XMPPacketOffset)
    {
        return n_computeChecksum (nativeObj, XMPPacketSize, XMPPacketOffset);
    }
    
    public String getChecksum ()
    {
        return n_getChecksum (nativeObj);
    }
    
    public void setChecksum (String checksum)
    {
        n_setChecksum (nativeObj, checksum);
    }
    
    public void addVideoSegment (VideoSegment newSegment)
    {
        n_addVideoSegment (nativeObj, newSegment.nativeObj);
    }
    
    public VideoSegment[] getAllVideoSegments ()
    {
        long nObjs[] = n_getAllVideoSegment (nativeObj);
        VideoSegment objs[] = new VideoSegment[nObjs.length];
        
        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new VideoSegment (nObjs[i]);
        }
        
        return objs;
    }
    
    public void convertTimestampToFrameIndex (long timeStamp, long duration,
                                              long frameIndex, long numOfFrames)
    {
        n_convertTimestampToFrameIndex (nativeObj, timestamp, duration,
                                        frameIndex, numOfFrames);
    }
    
    public void convertFrameIndexToTimestamp (long frameIndex, long numOfFrames, 
                                              long timestamp, long duration)
    {
        n_convertFrameIndexToTimestamp (nativeObj, frameIndex, numOfFrames,
                                        timestamp, duration);
    }
    
    //TO DO??: public long add(MetadataInternal mdInternal)
    
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
        
        int nativeFieldValueObjs[] = new int [fields.length];
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
    
    public MetadataSet queryByReference (String refName, FieldValue value);
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new MetadataSet (n_queryByReference (nativeObj, refName, value.nativeObj));
    }
    
    public MetadataSet queryByReference (String refName, FieldValue[] fields)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        int nativeFieldValueObjs[] = new int [fields.length];
        for (int i = 0; i < fields.length; i++)
        {
            nativeFieldValueObjs[i] = fields[i].nativeObj;
        }
        
        return new MetadataSet (n_queryByReference (nativeObj, refName, nativeFieldValueObjs));
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native long n_MetadataStream ();
    private native static boolean n_open (long nativeObj, String filePath, int mode);
    private native static boolean n_reopen (long nativeObj, int mode);
    private native static boolean n_load (long nativeObj, String schemaName);
    private native static boolean n_load (long nativeObj, String schemaName, String mdName);
    private native static boolean n_save (long nativeObj);
    private native static boolean n_saveTo (long nativeObj, String filePath);
    private native static void n_close (long nativeObj);
    private native static long n_getById (long nativeObj, long id);
    private native static long n_add (long nativeObj, long mdAddr);
    private native static boolean n_removeMdById (long nativeObj, long id);
    private native static void n_removeSet (long nativeObj, long setAddr);
    private native static void n_removeSchema (long nativeObj, long schemaAddr);
    private native static void n_removeAllMd (long nativeObj);
    private native static void n_addSchema (long nativeObj, long nativeObj);
    private native static long n_getSchema (long nativeObj, long schemaName);
    private native static String[] n_getAllSchemaNames (long nativeObj);
    private native static long n_getAll (long nativeObj);
    private native static boolean n_importSet (long nativeObj, long srcStreamAddr, long srcSetAddr, long tarFrameIndex,
                                               long srcFrameIndex, long numOfFrames, long setFailureAddr);
    private native static void n_clear (long nativeObjAddr);
    private native static void n_sortMdSetById (long nativeObjAddr);
    private native static String n_serialize (long nativeObjAddr, long formaterAddr);
    private native static void n_deserialize (long nativeObj, String text, long nativeObj);
    private native static String n_computeChecksum (long nativeObjAddr);
    private native static String n_computeChecksum (long nativeObjAddr, long XMPPacketSize, long XMPPacketOffset);
    private native static String n_getChecksum (long nativeObjAddr);
    private native static void n_setChecksum (long nativeObj, String checksum);
    private native static void n_addVideoSegment (long nativeObj, long newSegmentAddr);
    private native static long[] n_getAllVideoSegment (long nativeObjAddr);
    private native static void n_convertTimestampToFrameIndex (long nativeObj, long timestamp, long duration,
                                                               long frameIndex, long numOfFrames);
    private native static void n_convertFrameIndexToTimestamp (long nativeObj, long frameIndex, long numOfFrames,
                                                               long timestamp, long duration);
    private native static MetadataSet n_queryByFrameIndex (long nativeObj, long index);
    private native static MetadataSet n_queryByTime (long nativeObj, long startTime, long endTime);
    private native static MetadataSet n_queryBySchema (long nativeObj, String schemaName);
    private native static MetadataSet n_queryByName (long nativeObj, String name);
    private native static MetadataSet n_queryByNameAndValue (long nativeObj, String mdName, long value);
    private native static MetadataSet n_queryByNameAndFields (long nativeObj, String mdName, long nativeFieldValueObjs[]);
    private native static MetadataSet n_queryByReference (long nativeObj, String refName);
    private native static MetadataSet n_queryByReference (long nativeObj, String refName, long value);
    private native static MetadataSet n_queryByReference (long nativeObj, String refName, long nativeFieldValueObjs[]);
    //TO DO??: public long add(MetadataInternal mdInternal)
    private native static void n_delete (long nativeObj);
}
