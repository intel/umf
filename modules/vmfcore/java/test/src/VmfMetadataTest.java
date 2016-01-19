import junit.framework.*;
import com.intel.vmf.FieldValue;
import com.intel.vmf.Variant;
import static org.junit.Assert.*;
import org.junit.Test;

public class VmfMetadataTest 
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
    
    protected FieldDesc fields[] = new FieldDesc [3];
    fields[0] = new FieldDesc ("name", Variant.type_string, false);
    fields[1] = new FieldDesc ("last name", Variant.type_string, false);
    fields[2] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected MetadataDesc mdDesc = new MetadataDesc ("people", fields);
    
    @Before
    public static void setUp ()
    {
        protected Metadata md1 = new Metadata(mdDesc);
        protected Metadata md2 = new Metadata(mdDesc);
    }
    
    @Test
    public void testNotNull()
    {
        System.out.println("Inside VmfMetadataTest.testNotNull()");

        assertNotNull(fields);
        assertNotNull(mdDesc);
        assertNotNull(md1);
        assertNotNull(md2);
    }
    
    @Test
    public void testSettersGetters ()
    {
        System.out.println("Inside VmfMetadataTest.testSettersGetters()");
        
        assertEquals(-1, md1.getID ());
        assertEquals(-1, md2.getID ());
        assertEquals(-1, md1.getFrameIndex ());
        assertEquals(-1, md2.getFrameIndex ());
        assertEquals(0, md1.getNumOfFrames ());
        assertEquals(0, md2.getNumOfFrames ());
        assertEquals(-1, md1.getTime ());
        assertEquals(-1, md2.getTime ());
        assertEquals(0, md1.getDuration ());
        assertEquals(0, md2.getDuration ());
        
        assertEquals("people", md1.getName());
        assertEquals("", md1.getSchemaName());
        assertEquals(mdDesc, md1.getDesc());
        Variant tmp = md1.getFieldValue ("name");
        assertTrue(tmp.isEmpty());
        assertFalse(md1.hasField("age"));
        md1.setFrameIndex(0, 2);
        assertEquals(0, md1.getFrameIndex ());
        assertEquals(2, md1.getNumOfFrames ());
        md1.setTimestamp (0, 1);
        assertEquals(0, md1.getTime ());
        assertEquals(1, md1.getDuration ());
        
        Variant var1 = new Variant ("Dmitry");
        Variant var2 = new Variant ("Bogdanov");
        Variant var3 = new Variant (21);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        assertEquals (var1, md1.getFieldValue ("name"));
        assertEquals (var2, md1.getFieldValue ("last name"));
        assertEquals (var3, md1.getFieldValue ("age"));
    }
    
}