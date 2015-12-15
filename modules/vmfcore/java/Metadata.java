
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
		
		n_setTimestamp (nativeObj, time, UNDEFINED_DURATION);
	}
	
	public void setTimestampAndDuration (long time, long duration)
	{
		if (nativeObj == 0)
		    throw new java.lang.UnsupportedOperationException("Native object address is NULL");
		
		n_setTimestamp (nativeObj, time, duration);
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
	
	public String[] getFieldNames ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_getFieldNames (nativeObj);	
	}

	public Variant getFieldValue (String name)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
		return new Variant (n_getFieldValue (nativeObj, name));
	}
	
	public boolean hasField (String fieldName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_hasField (nativeObj, fieldName);
	}
	
	/*TO DO:
	public FieldValue findField ( String fieldName ){}*/
	
	public boolean equals (Metadata other)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_equals (nativeObj, other.nativeObj);
	}
	
	public boolean lessThan (Metadata other)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_lessThan (nativeObj, other.nativeObj);
	}
	
	public Metadata getFirstReference (String mdName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return new Metadata (n_getFirstReference (nativeObj, mdName));
	}
	
	public MetadataSet getReferencesByMetadata (String mdName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return new MetadataSet (n_getReferencesByMetadata (nativeObj, mdName));
	}
	
	public MetadataSet getReferencesByName (String refName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return new MetadataSet (n_getReferencesByName (nativeObj, refName));
	}
	
	public Reference[] getAllReferences ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_getAllReferences (nativeObj);
	}
	
	public boolean isReference (long id)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    String refName = "";
	    return n_isReferenceById (nativeObj, id, refName);
	}
	
	public boolean isReference (long id, String refName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_isReferenceById (nativeObj, id, refName);
	}
	
	public boolean isReference (Metadata md)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    String refName = "";
	    return n_isReferenceByMd (nativeObj, md.nativeObj, refName);
	}
	
	public boolean isReference (Metadata md, String refName)
    {
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_isReferenceByMd (nativeObj, md.nativeObj, refName);
    }
	
	public void addReference (Metadata md)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    String refName = "";
	    n_addReference (nativeObj, md.nativeObj, refName);
	}
	
	public void addReference (Metadata md, String refName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_addReference (nativeObj, md.nativeObj, refName);
	}
	
	public void removeReference (long id)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    String refName = "";
	    n_removeReferenceById (nativeObj, id, refName);
	}
	
	public void removeReference (long id, String refName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_removeReferenceById (nativeObj, id, refName);
	}
	
	public void removeReference (Metadata md)
    {
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    String refName = "";
	    n_removeReferenceByMd (nativeObj, md.nativeObj, refName);
    }
	
	public void removeReference (Metadata md, String refName)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_removeReferenceByMd (nativeObj, md.nativeObj, refName);
	}
	
	public void setFieldValue (String fieldName, Variant value)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_setFieldValue (nativeObj, fieldName, value.nativeObj);
	}
	
	public void addValue (Variant value)
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_addValue (nativeObj, value.nativeObj);
	}
	
	public void validate ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    n_validate (nativeObj);
	}
	
	public boolean isValid ()
	{
	    if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
	    return n_isValid (nativeObj);
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
    private native static long n_getID (long nativeObj);
    private native static long n_getFrameIndex (long nativeObj);
    private native static long n_getNumOfFrames (long nativeObj);
    private native static void n_setFrameIndex (long nativeObj, long frameIndex, long numOfFrames);
    private native static void n_setTimestamp (long nativeObj, long time, long duration);
    private native static long n_getTimestamp (long nativeObj);
    private native static long n_getDuration (long nativeObj);
    private native static String n_getName (long nativeObj);
    private native static String n_getSchemaName (long nativeObj);
    private native static long n_getDesc (long nativeObj);
    private native static String[] n_getFieldNames (long nativeObj);
    private native static long n_getFieldValue (long nativeObj, String name);
    private native static boolean n_hasField (long nativeObj, String name);
    private native static boolean n_equals (long nativeObj, long other);
    //TO DO: private native static long n_findField (long nativeObj, String fieldName);
    private native static boolean n_lessThan (long nativeObj, long other);
    private native static long n_getFirstReference (long nativeObj, String name);
    private native static long n_getReferencesByMetadata (long nativeObj, String name);
    private native static long n_getReferencesByName (long nativeObj, String name);
    private native static Reference[] n_getAllReferences (long nativeObj);
    private native static boolean n_isReferenceById (long nativeObj, long id, String refName);
    private native static boolean n_isReferenceByMd (long nativeObj, long md, String refName);
    private native static void n_addReference (long nativeObj, long md, String refName);
    private native static void n_removeReferenceById (long nativeObj, long id, String refName);
    private native static void n_removeReferenceByMd (long nativeObj, long md, String refName);
    private native static void n_setFieldValue (long nativeObj, String fieldName, long variant);
    private native static void n_addValue (long nativeObj, long variant);
    private native static void n_validate (long nativeObj);
    private native static boolean n_isValid (long nativeObj);
    private native static void n_delete (long nativeObj);
}
