import com.intel.vmf.FieldValue;
import com.intel.vmf.Log;
import com.intel.vmf.Variant;
import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class VmfFieldValueTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected Variant variant;
    protected FieldValue fieldValue1;
    protected FieldValue fieldValue2; 
    protected FieldValue fieldValue3; 
    
    @Before
    public void setUp()
    {
    	variant = new Variant();
    	variant.setTo(25);
    	
    	fieldValue1 = new FieldValue();
    	fieldValue2 = new FieldValue("fieldName", variant);
    	fieldValue3 = new FieldValue("fieldName", variant);
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testEquals ()
    {
         assertTrue (fieldValue2.equals (fieldValue3));
         assertFalse (fieldValue1.equals (fieldValue2));
         assertFalse (fieldValue1.equals (fieldValue3));
         
         variant.setTo("fieldValue");
         FieldValue fieldValue4 = new FieldValue ("fieldName", variant);
         fieldValue2.setTo(fieldValue4);
         
         thrown.expect(com.intel.vmf.Exception.class);
         thrown.expectMessage("vmf::Exception: Can't compare Variant objects with incompatible types");
         assertFalse (fieldValue2.equals(fieldValue3));
    }
    
    @Test
    public void testGetters ()
    {
         assertEquals(fieldValue2.getName(), fieldValue3.getName()); 
         
         assertTrue(fieldValue1.isEmpty());
         assertFalse(fieldValue2.isEmpty());
         assertFalse(fieldValue3.isEmpty());
         
         assertEquals("", fieldValue1.getName());
         assertEquals("fieldName", fieldValue2.getName());
         assertEquals("fieldName", fieldValue3.getName());
         
         assertEquals(fieldValue2.getType(), fieldValue3.getType());
         assertEquals(Variant.type_integer, fieldValue2.getType());
         assertEquals("integer", fieldValue3.getTypeName());
         
         assertEquals(Variant.type_empty, fieldValue1.getType());
         assertEquals("empty", fieldValue1.getTypeName());
    }
    
    @Test
    public void testSetToAndConvertTo ()
    {
         fieldValue1.setTo(fieldValue2);
         assertTrue (fieldValue1.equals (fieldValue2));
         
         thrown.expect(com.intel.vmf.Exception.class);
         thrown.expectMessage("vmf::Exception: Non-existent type.");
         fieldValue1.convertTo (14);
    }
    
    @Test
    public void testConvertIntToString ()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Cannot convert value to the target type!");
        fieldValue2.convertTo (Variant.type_string);
    }
    
    @Test
    public void testClear ()
    {
        FieldValue fv = new FieldValue();
        
        fieldValue2.clear();
        assertEquals("", fieldValue2.getName());
        assertEquals(fv.getType(), fieldValue2.getType());
        assertEquals(Variant.type_empty, fieldValue2.getType());
        assertEquals("empty", fv.getTypeName());
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
