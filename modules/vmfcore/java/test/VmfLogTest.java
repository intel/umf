import static org.junit.Assert.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

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
    public void testLog () throws IOException
    {
        File newFile = new File("myLogFile.txt");
        if (newFile.exists())
            newFile.delete();
        
        assertTrue(newFile.createNewFile()); 
        
        Log.setLogToFile("myLogFile.txt");
        BufferedReader in = new BufferedReader(new FileReader("myLogFile.txt"));
        String str = in.readLine();
        assertTrue (str == null);
        
        Log.log(Log.LOG_INFO, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 30);
        
        str = in.readLine();
        assertFalse (str.isEmpty());
        str.equals("INFO: Hello World! in VmfLogTest::testLog VmfLogTest.java:30");
        in.close();
        newFile.delete();
        
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