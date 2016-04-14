package com.intel.vmf;

public class Metadata
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

    public final long nativeObj;

    public static final long UNDEFINED_DURATION = 0;

    public static final long NUMOFFRAMES_DEFAULT = 1;

    protected Metadata (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }

    public Metadata (MetadataDesc mdDescription)
    {
        this (n_Metadata (mdDescription.nativeObj));
    }

    /*
     * public Metadata (Metadata md)
     * {
     *     nativeObj = n_MetadataCopy (md.nativeObj);
     * }
    */

    public void clear ()
    {
        throw new java.lang.UnsupportedOperationException("Not Yet Implemented");
    }

    public long getID ()
    {
        return n_getID (nativeObj);
    }

    public long getFrameIndex ()
    {
        return n_getFrameIndex (nativeObj);
    }

    public long getNumOfFrames ()
    {
        return n_getNumOfFrames (nativeObj);
    }

    public void setFrameIndex (long frameIndex)
    {
        n_setFrameIndex (nativeObj, frameIndex, NUMOFFRAMES_DEFAULT);
    }

    public void setFrameIndex (long frameIndex, long numOfFrames)
    {
        n_setFrameIndex (nativeObj, frameIndex, numOfFrames);
    }

    public void setTimestamp (long time)
    {
        n_setTimestamp (nativeObj, time, UNDEFINED_DURATION);
    }

    public void setTimestamp (long time, long duration)
    {
        n_setTimestamp (nativeObj, time, duration);
    }

    public long getTime ()
    {
        return n_getTime (nativeObj);
    }

    public long getDuration ()
    {
        return n_getDuration (nativeObj);
    }

    public String getName ()
    {
        return n_getName (nativeObj);
    }

    public String getSchemaName ()
    {
        return n_getSchemaName (nativeObj);
    }

    public MetadataDesc getDesc ()
    {
    	long nativeAddr = n_getDesc (nativeObj);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new MetadataDesc (nativeAddr);
    }

    public String[] getFieldNames ()
    {
        return n_getFieldNames (nativeObj);	
    }

    public Variant getFieldValue (String name)
    {
    	long nativeAddr = n_getFieldValue (nativeObj, name);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new Variant (nativeAddr);
    }

    public boolean hasField (String fieldName)
    {
        return n_hasField (nativeObj, fieldName);
    }

    /*
     *public FieldValue findField (String fieldName)
     *{
     *   return new FieldValue (n_findField (nativeObj, fieldName));
     *}
    */
    
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
    	long nativeAddr = n_getFirstReference (nativeObj, mdName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new Metadata (nativeAddr);
    }

    public MetadataSet getReferencesByMetadata (String mdName)
    {
    	long nativeAddr = n_getReferencesByMetadata (nativeObj, mdName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new MetadataSet (nativeAddr);
    }

    public MetadataSet getReferencesByName (String refName)
    {
    	long nativeAddr = n_getReferencesByName (nativeObj, refName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new MetadataSet (nativeAddr);
    }

    public Reference[] getAllReferences ()
    {
        long nObjs[] = n_getAllReferences (nativeObj);
        
        if (nObjs == null)
        	return null;
        
        Reference objs[] = new Reference [nObjs.length];

        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new Reference (nObjs[i]);
        }

        return objs;
    }

    public boolean isReference (long id)
    {
        String refName = "";
        return n_isReferenceById (nativeObj, id, refName);
    }

    public boolean isReference (long id, String refName)
    {
        return n_isReferenceById (nativeObj, id, refName);
    }

    public boolean isReference (Metadata md)
    {
        String refName = "";
        return n_isReferenceByMd (nativeObj, md.nativeObj, refName);
    }

    public boolean isReference (Metadata md, String refName)
    {
        return n_isReferenceByMd (nativeObj, md.nativeObj, refName);
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
        n_removeReferenceById (nativeObj, id, refName);
    }

    public void removeReference (long id, String refName)
    {
        n_removeReferenceById (nativeObj, id, refName);
    }

    public void removeReference (Metadata md)
    {
        String refName = "";
        n_removeReferenceByMd (nativeObj, md.nativeObj, refName);
    }

    public void removeReference (Metadata md, String refName)
    {
        n_removeReferenceByMd (nativeObj, md.nativeObj, refName);
    }

    public void setFieldValue (String fieldName, Variant value)
    {
        n_setFieldValue (nativeObj, fieldName, value.nativeObj);
    }

    public void addValue (Variant value)
    {
        n_addValue (nativeObj, value.nativeObj);
    }

    public void validate ()
    {
        n_validate (nativeObj);
    }

    public boolean isValid ()
    {
        return n_isValid (nativeObj);
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }

    private native static long n_Metadata (long mdDescriptionAddr);
    private native static long n_getID (long nativeObj);
    private native static long n_getFrameIndex (long nativeObj);
    private native static long n_getNumOfFrames (long nativeObj);
    private native static void n_setFrameIndex (long nativeObj, long frameIndex, long numOfFrames);
    private native static void n_setTimestamp (long nativeObj, long time, long duration);
    private native static long n_getTime (long nativeObj);
    private native static long n_getDuration (long nativeObj);
    private native static String n_getName (long nativeObj);
    private native static String n_getSchemaName (long nativeObj);
    private native static long n_getDesc (long nativeObj);
    private native static String[] n_getFieldNames (long nativeObj);
    private native static long n_getFieldValue (long nativeObj, String name);
    private native static boolean n_hasField (long nativeObj, String name);
    private native static boolean n_equals (long nativeObj, long other);
    //???private native static long n_findField (long nativeObj, String fieldName);
    private native static boolean n_lessThan (long nativeObj, long other);
    private native static long n_getFirstReference (long nativeObj, String name);
    private native static long n_getReferencesByMetadata (long nativeObj, String name);
    private native static long n_getReferencesByName (long nativeObj, String name);
    private native static long[] n_getAllReferences (long nativeObj);
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
