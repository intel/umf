//package com.intel.vmf;

public class MetadataDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    public MetadataDesc ()
    {
        nativeObj = n_MetadataDesc ();
    }
    
    /* TO DO: public MetadataDesc (String mdName, FieldDesc[] fields)
    {
        nativeObj = n_MetadataDesc (mdName, fields);
    }
    
    public MetadataDesc (String mdName, FieldDesc[] fields, ReferenceDesc[] refs)
    {
        nativeObj = n_MetadataDesc (mdName, fields, refs);
    }*/
    
    public MetadataDesc (String mdName, long type)
    {
        nativeObj = n_MetadataDesc (mdName, type);
    }
    
    public String getMetadataName ()
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_getMetadataName (nativeObj);
    }
    
    public String getSchemaName ()
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_getSchemaName (nativeObj);
    }
    
    public FieldDesc[] getFields ()
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_getFields (nativeObj);
    }
    
    public ReferenceDesc[] getAllReferenceDescs ()
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_getAllReferenceDescs (nativeObj);
    }
    
    public void declareCustomReference (String refName)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        boolean isUnique = false;
        n_declareCustomReference (nativeObj, refName, isUnique);
    }
    
    public void declareCustomReference (String refName, boolean isUnique)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        n_declareCustomReference (nativeObj, refName, isUnique);
    }
    
    public ReferenceDesc getReferenceDesc (String refName)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return new ReferenceDesc (n_getReferenceDesc (nativeObj, refName));
    }
    
    public boolean getFieldDesc (FieldDesc field)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        String fieldName = "";
        return n_getFieldDesc (nativeObj, field, fieldName);
    }
    
    public boolean getFieldDesc (FieldDesc field, String fieldName)
    {
        if (nativeObj == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        return n_getFieldDesc (nativeObj, field, fieldName);
    }
        
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native long n_MetadataDesc ();
    private native long n_MetadataDesc (String mdName, long type);
    /* TO DO: 
     * private native long n_MetadataDesc (String mdName, FieldDesc[] fields);
     * private native long n_MetadataDesc (String mdName, FieldDesc[] fields, ReferenceDesc[] refs);
     */
    private native static String n_getMetadataName (long nativeObj);
    private native static String n_getSchemaName (long nativeObj);
    private native static FieldDesc[] n_getFields (long nativeObj);
    private native static ReferenceDesc[] n_getAllReferenceDescs (long nativeObj);
    private native static void n_declareCustomReference (long nativeObj, String refName, boolean isUnique);
    private native static long n_getReferenceDesc (long nativeObj, String refName);
    private native static boolean n_getFieldDesc (long nativeObj, FieldDesc field, String fieldName);
    private native static void n_delete (long nativeObj);
} 
