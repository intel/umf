package com.intel.vmf;

public class Log
{
    static
    {
        System.loadLibrary("vmf");
    }
    
    public static final int LOG_INFO = 0;
    public static final int LOG_WARNING = 1;
    public static final int LOG_ERROR = 2;
    public static final int LOG_NO_MESSAGE = 3;
    
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
    
    private static native void n_logToFile (String file);
    private static native void n_logToConsole ();
    private static native void n_setVerbosityLevel (int level);
    private static native int n_getVerbosityLevel ();
}
