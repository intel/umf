import junit.framework.*;
import com.intel.vmf.FieldValue;
import com.intel.vmf.Vmf;
import com.intel.vmf.Variant;
import static org.junit.Assert.*;
import org.junit.Test;

public class VmfFieldValueTest 
{
    static
    {
        System.loadLibrary("vmf");
    }
    
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
    
    protected Variant var = new Variant();
    protected FieldValue tmp1 = new FieldValue();
    protected FieldValue tmp2 = new FieldValue("fieldName", var);
    protected FieldValue tmp3 = new FieldValue("fieldName", var);
    
    @Test
    public void testNotNull()
    {
        System.out.println("Inside VmfFieldValueTest.testNotNull()");

        assertNotNull(var);
        assertNotNull(tmp1);
        assertNotNull(tmp2);
        assertNotNull(tmp3);
    }
    
    @Test
    public void testEquals ()
    {
         System.out.println("Inside VmfFieldValueTest.testEquals()");

         assertTrue (tmp2.equals (tmp3));
         assertFalse (tmp1.equals (tmp2));
         assertFalse (tmp1.equals (tmp3));
    }
    
    @Test
    public void testGetName ()
    {
         System.out.println("Inside VmfFieldValueTest.testGetName()");
         
         assertEquals(tmp2.getName(), tmp4.getName()); 
         
         assertEquals("", tmp1.getName());
         assertEquals("fieldName", tmp2.getName());
         assertEquals("fieldName", tmp3.getName());
    }
    
    @Test
    public void testSetTo ()
    {
         System.out.println("Inside VmfFieldValueTest.testSetTo()");
         
         tmp1.setTo (tmp2);
         assertTrue (tmp1.equals (tmp2));
    }
    
    @Test
    public void testClear ()
    {
        System.out.println("Inside VmfFieldValueTest.testClear()");
        
        FieldValue tmp4 = new FieldValue();
        
        tmp2.clear();
        assertTrue (tmp2.equals(tmp4));
    }
}
