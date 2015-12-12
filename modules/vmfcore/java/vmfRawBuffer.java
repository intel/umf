//package com.intel.vmf;

import java.nio.ByteBuffer;

public class vmfRawBuffer
{
	static
    {
        System.loadLibrary("vmf");
    }
	
    protected ByteBuffer byteBuf;
    protected int size;
    
    protected vmfRawBuffer (long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        
        nativeObj = addr;
    }
    
    public vmfRawBuffer ()
    {
    	byteBuf = null;
    	size = 0;
    }
    
    public vmfRawBuffer (ByteBuffer newBuffer)
    {
    	byteBuf = newBuffer;
    	size = newBuffer.capacity();
    }
    
    public void clear ()
    {
    	if (byteBuf != null)
    	{
    	    n_delete (byteBuf);
    	    byteBuf = null;
    	    size = 0;
    	}
    }
    
    @Override
    protected void finalize () throws Throwable 
    {
    	if (byteBuf != null)
    	{
            n_delete (byteBuf);
            byteBuf = null;
            size = 0;
    	}
    	
        super.finalize();
    }
    
    private native static void n_delete (ByteBuffer byteBuf);
}
