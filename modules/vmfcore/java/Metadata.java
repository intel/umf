
public class Metadata
{
	static
    {
        System.loadLibrary("vmf");
    }
	
	protected long nativeObj;
	
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
	
	public void copy (Metadata Metadata)
	{
        n_copy (nativeObj, Metadata.nativeObj);
	}
	
	/*??public void clear ()
	{
		if (nativeObj != 0)
		    n_delete (nativeObj);
		
		nativeObj = 0;
	}*/
	
	public long getId ()
	{
		return n_getID (nativeObj);
	}
	
	public long getFrameIndex ()
	{
		return;
	}
	
	public long getNumOfFrames ()
	{
		return;
	}
	
	public void setFrameIndex ()
	{
		
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
    
    private native long n_Metadata (mdDescription.nativeObj);
    private native static void n_copy (long nativeObj, long other);
}