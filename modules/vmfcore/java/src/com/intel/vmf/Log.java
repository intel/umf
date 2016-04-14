package com.intel.vmf;

import com.intel.vmf.Vmf;

public class Log
{
    static
    {
        try
        {
            System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError error1)
        {
            try
            {
                System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
            }
            catch (UnsatisfiedLinkError error2)
            {
                throw new java.lang.LinkageError("Native dynamic library is not found");
            }
        }
    }
    
    public static final int LOG_INFO = 0;
    public static final int LOG_WARNING = 1;
    public static final int LOG_ERROR = 2;
    public static final int LOG_NO_MESSAGE = 3;
    
    public static void log (int level, String msg, String func, String file, int line)
    {
        n_log (level, msg, func, file, line);
    }
    
    public static void setLogToFile (String file)
    {
        n_logToFile (file);
    }
    
    public static void setLogToConsole ()
    {
        n_logToConsole ();
    }
    
    public static void setVerbosityLevel(int level)
    {
        n_setVerbosityLevel (level);
    }
    
    public static int getVerbosityLevel()
    {
        return n_getVerbosityLevel ();
    }
    
    private static native void n_log (int level, String msg, String func, String file, int line);
    private static native void n_logToFile (String file);
    private static native void n_logToConsole ();
    private static native void n_setVerbosityLevel (int level);
    private static native int n_getVerbosityLevel ();
}
