package com.intel.vmf;

public class VmfIncorrectParamException extends VmfException
{
    public VmfIncorrectParamException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfIncorrectParamException [" + super.toString() + "]";
    }
}