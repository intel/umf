package com.intel.vmf;

public class VmfInternalErrorException extends VmfException
{   
    public VmfInternalErrorException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfInternalErrorException [" + super.toString() + "]";
    }
}
