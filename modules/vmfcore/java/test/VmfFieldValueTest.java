import com.intel.vmf.FieldValue;
import com.intel.vmf.Vmf;
import com.intel.vmf.Variant;
import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfFieldValueTest 
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
    
    protected Variant variant;
    protected FieldValue fieldValue1;
    protected FieldValue fieldValue2; 
    protected FieldValue fieldValue3; 
    
    @Before
    public void setUp()
    {
    	variant = new Variant();
    	fieldValue1 = new FieldValue();
    	variant.setTo(25);
    	fieldValue2 = new FieldValue("fieldName", variant);
    	fieldValue3 = new FieldValue("fieldName", variant);
    }
    
    @Test
    public void testEquals ()
    {
         assertTrue (fieldValue2.equals (fieldValue3));
         assertFalse (fieldValue1.equals (fieldValue2));
         assertFalse (fieldValue1.equals (fieldValue3));
    }
    
    @Test
    public void testGetters ()
    {
         assertEquals(fieldValue2.getName(), fieldValue3.getName()); 
         
         assertEquals("", fieldValue1.getName());
         assertEquals("fieldName", fieldValue2.getName());
         assertEquals("fieldName", fieldValue3.getName());
         
         assertEquals(fieldValue2.getType(), fieldValue3.getType());
         assertEquals(Variant.type_integer, fieldValue2.getType());
         assertEquals("integer", fieldValue3.getTypeName());
         
         assertEquals(Variant.type_unknown, fieldValue1.getType());
         assertEquals("unknown", fieldValue1.getTypeName());
    }
    
    @Test
    public void testSetTo ()
    {
         fieldValue1.setTo(fieldValue2);
         assertTrue (fieldValue1.equals (fieldValue2));
    }
    
    @Test
    public void testClear ()
    {
        FieldValue fv = new FieldValue();
        
        fieldValue2.clear();
        assertEquals("", fieldValue2.getName());
        assertEquals(fv.getType(), fieldValue2.getType());
        assertEquals(Variant.type_unknown, fieldValue2.getType());
        assertEquals("unknown", fv.getTypeName());
    }
    
    @Test
    public void testDeleteByGC()
    {
        fieldValue1 = null;
        fieldValue2 = null;
        fieldValue3 = null;
        variant = null;
        System.gc();
    }
}
