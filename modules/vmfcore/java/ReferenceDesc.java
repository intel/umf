//package com.intel.vmf;
 
public class ReferenceDesc
{
    public String name;
    public boolean isUnique;
    public boolean isCustom;

    static
    {
        System.loadLibrary("vmf");
    }
    
    protected final long nativeObj;
    
    protected ReferenceDesc (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public ReferenceDesc ()
	{
        name = "";
        isUnique = false;
        isCustom = false;
	}
    
    public ReferenceDesc (String refName, boolean unique, boolean custom) 
    {
    	name = refName;
    	isUnique = unique; 
    	isCustom = custom;
    }
}
