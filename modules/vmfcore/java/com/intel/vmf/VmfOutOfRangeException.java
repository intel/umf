package com.intel.vmf;

public class VmfOutOfRangeException extends VmfException
{
    public VmfOutOfRangeException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfOutOfRangeException [" + super.toString() + "]";
    }
}
