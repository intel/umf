import static org.junit.Assert.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
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
    
    protected File newFile; 
    
    @Before
    public void setUp () throws IOException
    {
        Log.setVerbosityLevel(Log.LOG_INFO);
        newFile = new File("myLogFile.txt");
        
        if (newFile.exists())
            newFile.delete();
    }
    
    @After
    public void tearDown ()
    {
        if (newFile.exists())
            newFile.delete();
    }
    
    @Test
    public void testLog () throws IOException
    {
        assertTrue(newFile.createNewFile()); 
        
        Log.setLogToFile("myLogFile.txt");
        BufferedReader in = new BufferedReader(new FileReader("myLogFile.txt"));
        String str = in.readLine();
        assertTrue (str == null);
        
        Log.log(Log.LOG_INFO, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 61);
        Log.log(Log.LOG_WARNING, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 62);
        Log.log(Log.LOG_ERROR, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 63);
        Log.log(4, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 64);
        
        str = in.readLine();
        assertFalse (str == null);
        assertFalse (str.isEmpty());
        assertTrue(str.equals("INFO: Hello World! in VmfLogTest::testLog VmfLogTest.java:61"));
        in.close();
        
        Log.setLogToConsole ();
        
        Log.setVerbosityLevel(Log.LOG_INFO);
        assertEquals(Log.LOG_INFO, Log.getVerbosityLevel());
        
        Log.log(Log.LOG_INFO, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 77);
        
        Log.setVerbosityLevel(Log.LOG_WARNING);
        assertEquals(Log.LOG_WARNING, Log.getVerbosityLevel());
        
        Log.log(Log.LOG_WARNING, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 82);
        
        Log.setVerbosityLevel(Log.LOG_ERROR);
        assertEquals(Log.LOG_ERROR, Log.getVerbosityLevel());
        
        Log.log(Log.LOG_ERROR, "Hello World!", "VmfLogTest::testLog", "VmfLogTest.java", 87);
        
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
        assertEquals(Log.LOG_NO_MESSAGE, Log.getVerbosityLevel());
    }
}