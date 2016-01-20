import junit.framework.*;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;
import org.junit.Test;

public class VmfMetadataSchemaTest 
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
    
    protected MetadataSchema schema;
    
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
    
    protected MetadataDesc mdDesc1 = new MetadataDesc ("person", fields1, refs);
    protected MetadataDesc mdDesc2 = new MetadataDesc ("car", fields2);
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public static void setUp ()
    {
        schema = new MetadataSchema ("test_schema", "Anna");
        
        assertNotNull(schema);
        assertNotNull(mdDesc1);
        assertNotNull(mdDesc2);
    }
    
    @Test
    public void testMetadataSchema ()
    {
        String str1 = schema.getName();
        assertEquals(st1, "test_schema");
        
        String str2 = schema.getAuthor ();
        assertEquals(st2, "Anna");
        
        schema.add(mdDesc1);
        schema.add(mdDesc2);
        
        assertEquals(2, schema.size());
        
        MetadataDesc mdDesc3 = schema.findMetadataDesc("person");
        
        assertNotNull(mdDesc3);
        assertEquals(mdDesc1, mdDesc3);
        
        MetadataDesc mdDescs[] = schema.getAll();
        assertEquals(2, mdDescs.length);
        
        String stdName = "vmf://ns.intel.com/vmf/std-dst-schema-1.0";
        
        MetadataSchema std = getStdSchema();
        assertNotNull(std);
        assertEquals(stdName, schema.getName());
        
        assertEquals(stdName, getStdSchemaName ());
    }
}