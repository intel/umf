import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import com.intel.vmf.Log;
import com.intel.vmf.Vmf;


public class VmfLogTest 
{
    @BeforeClass
    public static void init()
    {
        Vmf.initialize();
    }
    
    @AfterClass
    public static void terminate()
    {
        Vmf.terminate();
    }
    
    @Test
    public void testLog ()
    {
        Log.setLogToConsole ();
        assertEquals(Log.LOG_INFO, Log.getVerbosityLevel());
        
        Log.setVerbosityLevel(Log.LOG_WARNING);
        assertEquals(Log.LOG_WARNING, Log.getVerbosityLevel());
        
        Log.setVerbosityLevel(Log.LOG_ERROR);
        assertEquals(Log.LOG_ERROR, Log.getVerbosityLevel());
        
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
        assertEquals(Log.LOG_NO_MESSAGE, Log.getVerbosityLevel());
    }
}