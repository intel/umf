package com.intel.vmf;

public class FieldDesc
{
    static
    {
        System.loadLibrary("vmf");
    }
	
    public boolean optional;
	
    public String name;
	
    public Variant.Type type;
    
    public FieldDesc ()
    {
    	optional = false;
    	name = "";
    	type = Variant.STRING;
    }
	
    public FieldDesc ( final string sName, Variant.Type eType, boolean isOptional )
    {
    	optional = isOptional;
    	name = sName;
    	type = eType;
    }
	
    public boolean isEqual ( FieldDesc other )
    {
        return (name == other.name) && (type == other.type);
    }
}
