//package com.intel.vmf;

public class Reference
{
	private ReferenceDesc refDesc;
	private Metadata  metaData;
	
	protected long nativeObj;
	
	public Reference ()
	{
		refDesc = null;
		metaData = null;
		nativeObj = n_Reference();
	}
	
	public Reference (ReferenceDesc desc, Metadata md)
	{
		refDesc = desc;
		metaData = md;
		nativeObj = n_Reference (desc.name, desc.isUnique, desc.isCustom, md.description);
	}
	
	public void clear ()
	{
		if (nativeObj != 0)
		    n_delete (nativeObj);
		
		nativeObj = n_Reference (nativeObj);
	}
	
	public Metadata getMetadata ()
	{
		return n_getMetadata (nativeObj, );
		
		//return metaData;
	}
	
	public ReferenceDesc getReferenceDescription ()
	{
		boolean flags = new boolean[2];
		String name = new String();
		n_getReferenceDescription (nativeObj, flags, name);
		return new ReferenceDesc (name, flags[0], flags[1]);
		
		//return refDesc;
	}
	
	public void setMetadata (Metadata md)
	{
		metaData = md;
		n_setMetadata (md);
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
}