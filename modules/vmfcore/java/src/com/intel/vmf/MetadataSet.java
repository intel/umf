package com.intel.vmf;

public class MetadataSet implements IQuery
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

    public static final long FRAME_COUNT_ALL = Long.MAX_VALUE;

    protected MetadataSet (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException ("Native object address is NULL");

        nativeObj = addr;
    }

    public MetadataSet ()
    {
        this (n_MetadataSet ());
    }

    public Metadata getElement (long number)
    {
    	if (number >= 0)
    	{
    		long nativeAddr = n_getElement (nativeObj, number);
    		if (nativeAddr == 0) return null;
    		else return new Metadata (nativeAddr);
    	}
    	else return null;
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

    public long shift (long nTarFrameIndex, long nSrcFrameIndex)
    {
        return n_shift (nativeObj, nTarFrameIndex, nSrcFrameIndex, FRAME_COUNT_ALL, 0);
    }

    public long shift (long nTarFrameIndex, long nSrcFrameIndex, long nNumOfFrames, MetadataSet setFailure)
    {
    	if (setFailure == null)
    		return n_shift (nativeObj, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames, 0);
    	else
    		return n_shift (nativeObj, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames, setFailure.nativeObj);
    }

    public long getSize ()
    {
        return n_getSize (nativeObj);
    }

    public void clear ()
    {
    	n_clear (nativeObj);
    }

    public void push_back (Metadata m)
    {
    	n_push_back (nativeObj, m.nativeObj);
    }

    public void erase (long idx)
    {
    	n_erase (nativeObj, idx);
    }

    @Override
    protected void finalize () throws Throwable
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private native static long n_MetadataSet ();
    private native static long n_getElement (long nativeObj, long number);
    private native static long n_queryByFrameIndex (long nativeObj, long index);
    private native static long n_queryByTime (long nativeObj, long startTime, long endTime);
    private native static long n_queryBySchema (long nativeObj, String schemaName);
    private native static long n_queryByName (long nativeObj, String name);
    private native static long n_queryByNameAndValue (long nativeObj, String mdName, long value);
    private native static long n_queryByNameAndFields (long nativeObj, String mdName, long nativeFieldValueObjs[]);
    private native static long n_queryByReference (long nativeObj, String refName);
    private native static long n_queryByReference (long nativeObj, String refName, long value);
    private native static long n_queryByReference (long nativeObj, String refName, long nativeFieldValueObjs[]);
    private native static long n_shift (long nativeObjAddr, long dstFrameIndex, long srcFrameIndex, long numOfFrames, long setFailure);
    private native static long n_getSize (long nativeObj);
    private native static long n_clear (long nativeObj);
    private native static long n_push_back (long nativeObj, long metadataObj);
    private native static long n_erase (long nativeObj, long idx);
    private native static void n_delete (long nativeObj);
}
