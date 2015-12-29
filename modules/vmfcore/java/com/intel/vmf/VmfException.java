package com.intel.vmf;

public class VmfException extends RuntimeException
{
    private static final long serialVersionUID = 1L;

    public VmfException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "CvException [" + super.toString() + "]";
    }
}