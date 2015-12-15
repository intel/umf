
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
	    return n_lessThan (nativeObj, other.nativeObj);
	}
	
	public Metadata getFirstReference (String mdName)
	{
	    return new Metadata (n_getFirstReference (nativeObj, mdName));
	}
	
	public MetadataSet getReferencesByMetadata (String mdName)
	{
	    return new MetadataSet (n_getReferencesByMetadata (nativeObj, mdName));
	}
	
	public MetadataSet getReferencesByName (String refName)
	{
	    return new MetadataSet (n_getReferencesByName (nativeObj, refName));
	}
	
	public ArrayList<Reference> getAllReferences ()
	{
	    return n_getAllReferences (nativeObj);
	}
	
	public boolean isReference (long id)
	{
	    String refName = "";
	    return n_isReference (nativeObj, id, refName);
	}
	
	public boolean isReference (long id, String refName)
	{
	    return n_isReference (nativeObj, id, refName);
	}
	
	public boolean isReference (Metadata md)
	{
	    String refName = "";
	    return n_isReference (nativeObj, md.nativeObj, refName);
	}
	
	public boolean isReference (Metadata md, String refName)
    {
        return n_isReference (nativeObj, md.nativeObj, refName);
    }
	
	public void addReference (Metadata md)
	{
	    String refName = "";
	    n_addReference (nativeObj, md.nativeObj, refName);
	}
	
	public void addReference (Metadata md, String refName)
	{
	    n_addReference (nativeObj, md.nativeObj, refName);
	}
	
	public void removeReference (long id)
	{
	    String refName = "";
	    n_removeReference (nativeObj, id, refName);
	}
	
	public void removeReference (long id, String refName)
	{
	    n_removeReference (nativeObj, id, refName);
	}
	
	public void removeReference (Metadata md)
    {
	    String refName = "";
	    n_removeReference (nativeObj, md.nativeObj, refName);
    }
	
	public void removeReference (Metadata md, String refName)
	{
	    n_removeReference (nativeObj, md.nativeObj, refName);
	}
	
	public void setFieldValue (String fieldName, Variant value)
	{
	    n_setFieldValue (nativeObj, fieldName, value.nativeObj);
	}
	
	public void addValue (Variant value)
	{
	    n_addValue (nativeObj, value.nativeObj);
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
