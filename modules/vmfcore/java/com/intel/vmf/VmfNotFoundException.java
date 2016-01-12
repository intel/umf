package com.intel.vmf;

public class VmfNotFoundException extends VmfException
{   
    public VmfNotFoundException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfNotFoundException [" + super.toString() + "]";
    }
}