package com.intel.vmf;

public class VmfDataStorageException extends VmfException
{
    public VmfDataStorageException (String msg)
    {
        super(msg);
    }

    @Override
    public String toString() 
    {
        return "VmfDataStorageException [" + super.toString() + "]";
    }
}