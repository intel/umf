//package com.intel.vmf;
 
public class ReferenceDesc
{
    public String name;
    public boolean isUnique;
    public boolean isCustom;

    public ReferenceDesc ()
	{
        name = "";
        isUnique = false;
        isCustom = false;
	}
    
    public ReferenceDesc(String refName, boolean unique, boolean custom) 
    {
    	name = refName;
    	isUnique = unique; 
    	isCustom = custom;
    }
}
