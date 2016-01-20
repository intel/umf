import junit.framework.*;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;
import org.junit.Test;


public class VmfMetadataDescTest 
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
    
    protected MetadataSchema schema = new MetadataSchema ("test_schema", "Anna");
    
    protected FieldDesc fields1[] = new FieldDesc [3];
    fields1[0] = new FieldDesc ("name", Variant.type_string, false);
    fields1[1] = new FieldDesc ("last name", Variant.type_string, false);
    fields1[2] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected ReferenceDesc refs[] = new ReferenceDesc [3];
    refs[0] = new ReferenceDesc ("friend");
    refs[1] = new ReferenceDesc ("colleague", false, true);
    refs[2] = new ReferenceDesc ("spouse", true);
    
    protected FieldDesc fields2[] = new FieldDesc [4];
    fields2[0] = new FieldDesc ("manufacturer", Variant.type_string, false);
    fields2[1] = new FieldDesc ("model", Variant.type_string, false);
    fields2[2] = new FieldDesc ("number", Variant.type_string, false);
    fields2[3] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected MetadataDesc mdDesc1; 
    protected MetadataDesc mdDesc2;
    protected MetadataDesc mdDesc3;
    protected MetadataDesc mdDesc4;
    
    @Before
    public static void setUp ()
    {
        mdDesc1 = new MetadataDesc ("person", fields1, refs);
        mdDesc2 = new MetadataDesc ("car", fields2);
        mdDesc3 = new MetadataDesc ();
        mdDesc4 = new MetadataDesc ("", Variant.type_string);
    }
    
    @Test
    public void testMetadataDesc ()
    {
        System.out.println("Inside VmfMetadataDescTest.testMetadataDesc()");
        
        assertEquals("person", mdDesc1.getMetadataName ());
        assertEquals("car", mdDesc2.getMetadataName ());
        
        String str = mdDesc3.getMetadataName ();
        assertTrue (str.isEmpty());
        
        str = mdDesc4.getMetadataName ();
        assertTrue (str.isEmpty());
        
        schema.add(mdDesc1);
        schema.add(mdDesc2);
        
        assertEquals("test_schema", mdDesc1.getSchemaName ());
        assertEquals("test_schema", mdDesc2.getSchemaName ());
        
        str = mdDesc3.getSchemaName ();
        assertTrue (str.isEmpty());
        
        FieldDesc fds[] = mdDesc1.getFields ();
        assertEquals (3, fds.length);
        
        fd = mdDesc2.getFields ();
        assertEquals (4, fd.length);
        
        ReferenceDesc rds1[] = mdDesc1.getAllReferenceDescs ();
        assertEquals(3, rds1.length);
        
        mdDesc2.declareCustomReference ("father", true);
        ReferenceDesc rds2[] = mdDesc2.getAllReferenceDescs ();
        assertEquals(1, rds2.length);
        
        ReferenceDesc rd = mdDesc1.getReferenceDesc ("friend");
        assertEquals(refs[0], rd);
        
        FieldDesc fd = mdDesc1.getFieldDesc ("age");
        assertEquals(fields1[2], fd);
    }
}