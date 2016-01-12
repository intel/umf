package com.intel.vmf;

public class VmfObjectExpiredException extends VmfException
{
    public VmfObjectExpiredException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfObjectExpiredException [" + super.toString() + "]";
    }
}