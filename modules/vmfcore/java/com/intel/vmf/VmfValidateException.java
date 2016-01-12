package com.intel.vmf;

public class VmfValidateException extends VmfException
{
    public VmfValidateException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfValidateException [" + super.toString() + "]";
    }
}
