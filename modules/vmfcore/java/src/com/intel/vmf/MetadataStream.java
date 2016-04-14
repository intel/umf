package com.intel.vmf;

public class MetadataStream implements IQuery
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

    protected final long nativeObj;

    public static final int InMemory = 0;
    public static final int ReadOnly = 1;
    public static final int ReadWrite = 2;

    public static final long FRAME_COUNT_ALL = Long.MAX_VALUE;

    public static class VideoSegment
    {
        protected final long nativeObj;

        protected VideoSegment (long addr)
        {
            if (addr == 0)
                throw new java.lang.IllegalArgumentException ("Native object address is NULL");

            nativeObj = addr;
        }

        public VideoSegment ()
        {
            this (n_VideoSegment ());
        }

        public VideoSegment (String title, double fps, long startTime)
        {
            long duration = 0;
            int width = 0;
            int height = 0;

            long addr = n_VideoSegment (title, fps, startTime, duration, width, height);

            if (addr == 0)
                throw new java.lang.IllegalArgumentException ("Native object address is NULL");

            nativeObj = addr;
        }

        public VideoSegment (String title, double fps, long startTime, long duration)
        {
            int width = 0;
            int height = 0;

            long addr = n_VideoSegment (title, fps, startTime, duration, width, height);

            if (addr == 0)
                throw new java.lang.IllegalArgumentException ("Native object address is NULL");

            nativeObj = addr;
        }

        public VideoSegment (String title, double fps, long startTime, long duration, int width, int height)
        {
            this (n_VideoSegment (title, fps, startTime, duration, width, height));
        }

        public String getTitle ()
        {
            return n_getTitle (nativeObj);
        }

        public void setTitle (String title)
        {
            n_setTitle (nativeObj, title);
        }

        public double getFPS ()
        {
            return n_getFPS (nativeObj);
        }

        public void setFPS (double fps)
        {
            n_setFPS (nativeObj, fps);
        }

        public long getDuration ()
        {
            return n_getDuration (nativeObj);
        }

        public void setDuration (long duration)
        {
            n_setDuration (nativeObj, duration);
        }

        public long getTime ()
        {
            return n_getTime (nativeObj);
        }

        public void setTime (long startTime)
        {
            n_setTime (nativeObj, startTime);
        }

        public int getWidth ()
        {
            return n_getWidth (nativeObj);
        }

        public int getHeight ()
        {
            return n_getHeight (nativeObj);
        }

        public void setResolution (int width, int height)
        {
            n_setResolution (nativeObj, width, height);
        }

        @Override
        protected void finalize () throws Throwable
        {
            if (nativeObj != 0)
                n_delete (nativeObj);

            super.finalize();
        }

        private native static long n_VideoSegment ();
        private native static long n_VideoSegment (String title, double fps, long startTime, long duration, int width, int height);
        private native static String n_getTitle (long nativeObj);
        private native static void n_setTitle (long nativeObj, String title);
        private native static double n_getFPS (long nativeObj);
        private native static void n_setFPS (long nativeObj, double fps);
        private native static long n_getDuration (long nativeObj);
        private native static void n_setDuration (long nativeObj, long duration);
        private native static long n_getTime (long nativeObj);
        private native static void n_setTime (long nativeObj, long startTime);
        private native static int n_getWidth (long nativeObj);
        private native static int n_getHeight (long nativeObj);
        private native static void n_setResolution (long nativeObj, int width, int height);
        private native static void n_delete (long nativeObj);
    }

    protected MetadataStream (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException ("Native object address is NULL");

        nativeObj = addr;
    }

    public MetadataStream ()
    {
        this (n_MetadataStream ());
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
        long nativeAddr = n_getById (nativeObj, id);

        if (nativeAddr == 0)
            return null;

        return new Metadata (nativeAddr);
    }

    public long add (Metadata md)
    {
        return n_add (nativeObj, md.nativeObj);
    }

    public long add (MetadataInternal mdInternal)
    {
        return n_addInternal (nativeObj, mdInternal.nativeObj);
    }

    public boolean remove (long id)
    {
        return n_removeMdById (nativeObj, id);
    }

    public void remove (MetadataSet set)
    {
        n_removeSet (nativeObj, set.nativeObj);
    }

    public void remove (MetadataSchema schema)
    {
        n_removeSchema (nativeObj, schema.nativeObj);
    }

    public void remove()
    {
        n_removeAllMd (nativeObj);
    }

    public void addSchema (MetadataSchema schema)
    {
        n_addSchema (nativeObj, schema.nativeObj);
    }

    public MetadataSchema getSchema (String schemaName)
    {
        long nativeAddr = n_getSchema (nativeObj, schemaName);

        if (nativeAddr == 0)
            return null;

        return new MetadataSchema (nativeAddr);
    }

    public String[] getAllSchemaNames ()
    {
        return n_getAllSchemaNames (nativeObj);
    }

    public MetadataSet getAll()
    {
        long nativeAddr = n_getAll (nativeObj);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public boolean importSet (MetadataStream srcStream, MetadataSet srcSet,
                              long tarFrameIndex, long srcFrameIndex)
    {
        return n_importSet (nativeObj, srcStream.nativeObj, srcSet.nativeObj,
                            tarFrameIndex, srcFrameIndex, FRAME_COUNT_ALL, 0);
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

    /*public String serialize (IWriter formater)
    {
        return n_serialize (nativeObj, formater.getNativeAddr());
    }

    public void deserialize (String text, IReader formater)
    {
        n_deserialize (nativeObj, text, formater.getNativeAddr());
    }*/

    public String computeChecksum ()
    {
        return n_computeChecksum (nativeObj);
    }

    public String computeChecksum (long options[])
    {
        if (options.length != 2)
            throw new java.lang.IllegalArgumentException ("Argument must contain 2 elements");

        return n_computeChecksum (nativeObj, options);
    }

    public String getChecksum ()
    {
        return n_getChecksum (nativeObj);
    }

    public void setChecksum (String checksum)
    {
        n_setChecksum (nativeObj, checksum);
    }

    public void addVideoSegment (VideoSegment segment)
    {
        n_addVideoSegment (nativeObj, segment.nativeObj);
    }

    public VideoSegment[] getAllVideoSegments ()
    {
        long nObjs[] = n_getAllVideoSegments (nativeObj);

        if (nObjs == null)
            return null;

        VideoSegment objs[] = new VideoSegment[nObjs.length];

        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new VideoSegment (nObjs[i]);
        }

        return objs;
    }

    public void addStat(Stat s) {
        n_addStat(nativeObj, s.nativeObj);
    }

    public String[] getAllStatNames() {
        return n_getAllStatNames(nativeObj);
    }

    public Stat getStat(String name) {
        return new Stat( n_getStat(nativeObj, name) );
    }

    public void recalcStat() {
        n_recalcStat(nativeObj);
    }

    public long convertDurationToNumOfFrames (long timestamp, long duration)
    {
        return n_convertDurationToNumOfFrames (nativeObj, timestamp, duration);
    }

    public long convertTimestampToFrameIndex (long timestamp, long duration)
    {
        return n_convertTimestampToFrameIndex (nativeObj, timestamp, duration);
    }

    public long convertFrameIndexToTimestamp (long frameIndex, long numOfFrames)
    {
        return n_convertFrameIndexToTimestamp (nativeObj, frameIndex, numOfFrames);
    }

    public long convertNumOfFramesToDuration (long frameIndex, long numOfFrames)
    {
        return n_convertNumOfFramesToDuration (nativeObj, frameIndex, numOfFrames);
    }

    public MetadataSet queryByFrameIndex (long index)
    {
        long nativeAddr = n_queryByFrameIndex (nativeObj, index);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByTime (long startTime, long endTime)
    {
        long nativeAddr = n_queryByTime (nativeObj, startTime, endTime);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryBySchema (String schemaName)
    {
        long nativeAddr = n_queryBySchema (nativeObj, schemaName);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByName (String name)
    {
        long nativeAddr = n_queryByName (nativeObj, name);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByNameAndValue (String mdName, FieldValue value)
    {
        long nativeAddr = n_queryByNameAndValue (nativeObj, mdName, value.nativeObj);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByNameAndFields (String mdName, FieldValue fields[])
    {
        long nativeFieldValueObjs[] = new long [fields.length];
        for (int i = 0; i < fields.length; i++)
        {
            nativeFieldValueObjs[i] = fields[i].nativeObj;
        }

        long nativeAddr = n_queryByNameAndFields (nativeObj, mdName, nativeFieldValueObjs);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByReference (String refName)
    {
        long nativeAddr = n_queryByReference (nativeObj, refName);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByReference (String refName, FieldValue value)
    {
        long nativeAddr = n_queryByReference (nativeObj, refName, value.nativeObj);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    public MetadataSet queryByReference (String refName, FieldValue[] fields)
    {
        long nativeFieldValueObjs[] = new long [fields.length];
        for (int i = 0; i < fields.length; i++)
        {
            nativeFieldValueObjs[i] = fields[i].nativeObj;
        }

        long nativeAddr = n_queryByReference (nativeObj, refName, nativeFieldValueObjs);

        if (nativeAddr == 0)
            return null;

        return new MetadataSet (nativeAddr);
    }

    @Override
    protected void finalize () throws Throwable
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private native static long n_MetadataStream ();
    private native static boolean n_open (long nativeObj, String filePath, int mode);
    private native static boolean n_reopen (long nativeObj, int mode);
    private native static boolean n_load (long nativeObj, String schemaName);
    private native static boolean n_load (long nativeObj, String schemaName, String mdName);
    private native static boolean n_save (long nativeObj);
    private native static boolean n_saveTo (long nativeObj, String filePath);
    private native static void n_close (long nativeObj);
    private native static long n_getById (long nativeObj, long id);
    private native static long n_add (long nativeObj, long mdAddr);
    private native static long n_addInternal (long nativeObj, long mdIntAddr);
    private native static boolean n_removeMdById (long nativeObj, long id);
    private native static void n_removeSet (long nativeObj, long setAddr);
    private native static void n_removeSchema (long nativeObj, long schemaAddr);
    private native static void n_removeAllMd (long nativeObj);
    private native static void n_addSchema (long nativeObj, long schemaAddr);
    private native static long n_getSchema (long nativeObj, String schemaName);
    private native static String[] n_getAllSchemaNames (long nativeObj);
    private native static long n_getAll (long nativeObj);
    private native static boolean n_importSet (long nativeObj, long srcStreamAddr, long srcSetAddr, long tarFrameIndex,
                                               long srcFrameIndex, long numOfFrames, long setFailureAddr);
    private native static void n_clear (long nativeObjAddr);
    private native static String n_serialize (long nativeObjAddr, long formaterAddr);
    private native static void n_deserialize (long nativeObj, String text, long formaterAddr);
    private native static String n_computeChecksum (long nativeObjAddr);
    private native static String n_computeChecksum (long nativeObjAddr, long options[]);
    private native static String n_getChecksum (long nativeObjAddr);
    private native static void n_setChecksum (long nativeObj, String checksum);
    private native static void n_addVideoSegment (long nativeObj, long newSegmentAddr);
    private native static long[] n_getAllVideoSegments (long nativeObjAddr);
    private native static void n_addStat(long self, long stat);
    private native static String[] n_getAllStatNames(long self);
    private native static long n_getStat(long self, String name);
    private native static void n_recalcStat(long self);

    private native static long n_convertTimestampToFrameIndex (long nativeObj, long timestamp, long duration);
    private native static long n_convertDurationToNumOfFrames (long nativeObj, long timestamp, long duration);
    private native static long n_convertFrameIndexToTimestamp (long nativeObj, long frameIndex, long numOfFrames);
    private native static long n_convertNumOfFramesToDuration (long nativeObj, long frameIndex, long numOfFrames);

    private native static long n_queryByFrameIndex (long nativeObj, long index);
    private native static long n_queryByTime (long nativeObj, long startTime, long endTime);
    private native static long n_queryBySchema (long nativeObj, String schemaName);
    private native static long n_queryByName (long nativeObj, String name);
    private native static long n_queryByNameAndValue (long nativeObj, String mdName, long value);
    private native static long n_queryByNameAndFields (long nativeObj, String mdName, long nativeFieldValueObjs[]);
    private native static long n_queryByReference (long nativeObj, String refName);
    private native static long n_queryByReference (long nativeObj, String refName, long value);
    private native static long n_queryByReference (long nativeObj, String refName, long nativeFieldValueObjs[]);
    private native static void n_delete (long nativeObj);
}
