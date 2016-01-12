package com.intel.vmf;

public class VmfTypeCastException extends VmfException
{
    public VmfTypeCastException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfTypeCastException [" + super.toString() + "]";
    }
}