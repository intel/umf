package com.intel.vmf;

public class MetadataDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    protected MetadataDesc (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public MetadataDesc ()
    {
        this (n_MetadataDesc ());
    }
    
    public MetadataDesc (String mdName, FieldDesc...fields)
    {
        long nObjs[] = new long [fields.length];
        
        for (int i = 0; i < nObjs.length; i++)
        {
            nObjs[i] = fields[i].nativeObj;
        }
        nativeObj = n_MetadataDesc (mdName, nObjs);
    }
    
    public MetadataDesc (String mdName, FieldDesc[] fields, ReferenceDesc[] refs)
    {
        long fdNativeAddr[] = new long [fields.length];
        long rdNativeAddr[] = new long [refs.length];
        
        for (int i = 0; i < fields.length; i++)
        {
            fdNativeAddr[i] = fields[i].nativeObj;
        }
        
        for (int j = 0; j < refs.length; j++)
        {
            rdNativeAddr[j] = refs[j].nativeObj;
        }
        
        nativeObj = n_MetadataDesc (mdName, fdNativeAddr, rdNativeAddr);
    }
    
    public MetadataDesc (String mdName, long type)
    {
        this (n_MetadataDesc (mdName, type));
    }
    
    public String getMetadataName ()
    {
        return n_getMetadataName (nativeObj);
    }
    
    public String getSchemaName ()
    {
        return n_getSchemaName (nativeObj);
    }
    
    public FieldDesc[] getFields ()
    {
        long nObjAddrs[] = n_getFields (nativeObj);
        FieldDesc objs[] = new FieldDesc [nObjAddrs.length];
        
        for (int i = 0; i < nObjAddrs.length; i++)
        {
            objs[i] = new FieldDesc (nObjAddrs[i]);
        }
        
        return objs;
    }
    
    public ReferenceDesc[] getAllReferenceDescs ()
    {
        long nObjAddrs[] = n_getAllReferenceDescs (nativeObj);
        ReferenceDesc objs[] = new ReferenceDesc [nObjAddrs.length];
        
        for (int i = 0; i < nObjAddrs.length; i++)
        {
            objs[i] = new ReferenceDesc (nObjAddrs[i]);
        }
        
        return objs;
    }
    
    public void declareCustomReference (String refName)
    {
        boolean isUnique = false;
        n_declareCustomReference (nativeObj, refName, isUnique);
    }
    
    public void declareCustomReference (String refName, boolean isUnique)
    {
        n_declareCustomReference (nativeObj, refName, isUnique);
    }
    
    public ReferenceDesc getReferenceDesc (String refName)
    {
        return new ReferenceDesc (n_getReferenceDesc (nativeObj, refName));
    }
    
    public boolean getFieldDesc (FieldDesc field)
    {
        String fieldName = "";
        return n_getFieldDesc (nativeObj, field.nativeObj, fieldName);
    }
    
    public boolean getFieldDesc (FieldDesc field, String fieldName)
    {
        return n_getFieldDesc (nativeObj, field.nativeObj, fieldName);
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);
        
        super.finalize();
    }
    
    private native static long n_MetadataDesc ();
    private native static long n_MetadataDesc (String mdName, long type);
    private native static long n_MetadataDesc (String mdName, long[] fieldDescAddr);
    private native static long n_MetadataDesc (String mdName, long[] fieldDescAddr, long[] refDescAddr);
    private native static String n_getMetadataName (long nativeObj);
    private native static String n_getSchemaName (long nativeObj);
    private native static long[] n_getFields (long nativeObj);
    private native static long[] n_getAllReferenceDescs (long nativeObj);
    private native static void n_declareCustomReference (long nativeObj, String refName, boolean isUnique);
    private native static long n_getReferenceDesc (long nativeObj, String refName);
    private native static boolean n_getFieldDesc (long nativeObj, long fieldAddr, String fieldName);
    private native static void n_delete (long nativeObj);
} 
