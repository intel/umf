package com.intel.vmf;

public class VmfNotInitializedException extends VmfException
{
    public VmfNotInitializedException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfNotInitializedException [" + super.toString() + "]";
    }
}