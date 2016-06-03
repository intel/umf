package com.intel.vmf;

import com.intel.vmf.Vmf;

public class MetadataDesc
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
        long nObjs[] = n_getFields (nativeObj);
        
        if (nObjs == null)
        	return null;
        
        FieldDesc objs[] = new FieldDesc [nObjs.length];
        
        for (int i = 0; i < nObjs.length; i++)
        {
            objs[i] = new FieldDesc (nObjs[i]);
        }
        
        return objs;
    }
    
    public ReferenceDesc[] getAllReferenceDescs ()
    {
        long nObjAddrs[] = n_getAllReferenceDescs (nativeObj);
        
        if (nObjAddrs == null)
        	return null;
        
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
    	long nativeAddr = n_getReferenceDesc (nativeObj, refName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
        return new ReferenceDesc (nativeAddr);
    }
    
    public FieldDesc getFieldDesc ()
    {
        String fieldName = "";
        long nativeAddr = n_getFieldDesc (nativeObj, fieldName);
        
        if (nativeAddr == 0)
    		return null;
        
        return new FieldDesc (nativeAddr);
    }
    
    public FieldDesc getFieldDesc (String fieldName)
    {
    	long nativeAddr = n_getFieldDesc (nativeObj, fieldName);
    	
    	if (nativeAddr == 0)
    		return null;
    	
    	return new FieldDesc (nativeAddr);
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
    private native static long n_getFieldDesc (long nativeObj, String fieldName);
    private native static void n_delete (long nativeObj);
} 
