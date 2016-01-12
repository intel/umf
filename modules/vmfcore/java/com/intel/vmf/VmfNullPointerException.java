package com.intel.vmf;

public class VmfNullPointerException extends VmfException
{
    public VmfNullPointerException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfNullPointerException [" + super.toString() + "]";
    }
}
