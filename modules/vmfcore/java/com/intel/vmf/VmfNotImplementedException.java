package com.intel.vmf;

public class VmfNotImplementedException extends RuntimeException
{
    public VmfNotImplementedException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfNotImplementedException [" + super.toString() + "]";
    }
}