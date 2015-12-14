
public class Metadata
{
	static
    {
        System.loadLibrary("vmf");
    }
	
	protected long nativeObj;
	
	public static final long UNDEFINED_DURATION = 0;
	
	public static final long NUMOFFRAMES_DEFAULT = 1;
	
	protected Metadata (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
	
	public Metadata (MetadataDesc mdDescription)
	{
		nativeObj = n_Metadata (mdDescription.nativeObj);
	}
	
	public void copy (Metadata metadata)
	{
		if ((nativeObj == 0) || (metadata.nativeObj == 0))
			throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_copy (nativeObj, metadata.nativeObj);
	}
	
	public void clear ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		    
		n_clear (nativeObj);
	}
	
	public long getID ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getID (nativeObj);
	}
	
	public long getFrameIndex ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getFrameIndex (nativeObj);
	}
	
	public long getNumOfFrames ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getNumOfFrames (nativeObj);
	}
	
	public void setFrameIndex (long frameIndex)
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_setFrameIndex (nativeObj, frameIndex, NUMOFFRAMES_DEFAULT);
	}
	
	public void setFrameIndexAndNumOfFrames (long frameIndex, long numOfFrames)
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_setFrameIndex (nativeObj, frameIndex, numOfFrames);
	}
	
	public void setTimestamp (long time)
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_setTimestamp (nativeObj, UNDEFINED_DURATION);
	}
	
	public void setTimestampAndDuration (long time, long duration)
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_setTimestamp (nativeObj, duration);
	}
	
	public long getTimestamp ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getTimestamp (nativeObj);
	}
	
	public long getDuration ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getDuration (nativeObj);
	}
	
	public String getName ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getName (nativeObj);
	}
	
	public String getSchemaName ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return n_getSchemaName (nativeObj);
	}
	
	public MetadataDesc getDesc ()
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		return new MetadataDesc (n_getDesc (nativeObj));
	}
	
	public ArrayList<String> getFieldNames ()
	{
	    return n_getFieldNames (nativeObj);	
	}

	public Variant getFieldValue (String name)
	{
		return new Variant (n_getFieldValue(nativeObj));
	}
	
	public boolean hasField (String fieldName)
	{
	    return n_hasField (nativeObj);
	}
	
	/*TO DO:
	public FieldValue findField ( String fieldName ){}*/
	
	public boolean equals (Metadata other)
	{
	    return n_equals (nativeObj, other.nativeObj);
	}
	
	public boolean lessThan (Metadata other)
	{
	    return n_lessThan (nativeObject, other.nativeObj);
	}
	
    @Override
	protected void finalize () throws Throwable 
	{
        if (nativeObj != 0)
	    {
            n_delete (nativeObj);
            nativeObj = 0;
	    }
	    super.finalize();
	}
   
    private native long n_Metadata (long mdDescriptionAddr);
    private native static void n_copy (long nativeObj, long other);
    private native static void n_clear (long nativeObj);
    private native static void 

}
