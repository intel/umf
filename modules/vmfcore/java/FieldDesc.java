//package com.intel.vmf;

public class FieldDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
	
    public boolean optional;
	
    public String name;
	
    public int type;
    
    public FieldDesc ()
    {
    	optional = false;
    	name = "";
    	type = Variant.type_string;
    }
	
    public FieldDesc (String newName, int newType, boolean isOptional )
    {
    	optional = isOptional;
    	name = newName;
    	type = newType;
    }
	
    public boolean isEqual (FieldDesc other )
    {
        return (name == other.name) && (type == other.type);
    }
}
