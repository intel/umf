package com.intel.vmf;

public class VmfAssociationException extends VmfException
{
    public VmfAssociationException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfAssociationException [" + super.toString() + "]";
    }
}
