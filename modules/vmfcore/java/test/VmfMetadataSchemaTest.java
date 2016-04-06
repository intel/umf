import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.Log;
import com.intel.vmf.Variant;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.ReferenceDesc;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class VmfMetadataSchemaTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected MetadataSchema schema;
    
    protected FieldDesc fields1[] = new FieldDesc [3];
    
    protected ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected FieldDesc fields2[] = new FieldDesc [4];
    
    protected MetadataDesc mdDesc1; 
    protected MetadataDesc mdDesc2;
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public void setUp ()
    {
        fields1[0] = new FieldDesc ("name", Variant.type_string, false);
        fields1[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields1[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        fields2[0] = new FieldDesc ("manufacturer", Variant.type_string, false);
        fields2[1] = new FieldDesc ("model", Variant.type_string, false);
        fields2[2] = new FieldDesc ("number", Variant.type_string, false);
        fields2[3] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        mdDesc1 = new MetadataDesc ("person", fields1, refs);
        mdDesc2 = new MetadataDesc ("car", fields2);
        
        schema = new MetadataSchema ("test_schema", "Anna");
    }
    
    @Test
    public void testMetadataSchema ()
    {
        String str1 = schema.getName();
        assertEquals(str1, "test_schema");
        
        String str2 = schema.getAuthor ();
        assertEquals(str2, "Anna");
        
        schema.add(mdDesc1);
        schema.add(mdDesc2);
        
        assertEquals(2, schema.size());
        
        MetadataDesc mdDesc3 = schema.findMetadataDesc("person");
        assertEquals("person", mdDesc3.getMetadataName());
        
        FieldDesc fds[] = mdDesc3.getFields();
        assertEquals(3, fds.length);
        
        ReferenceDesc rds[] = mdDesc3.getAllReferenceDescs();
        assertEquals(4, rds.length);
        
        MetadataDesc mdDescs[] = schema.getAll();
        assertEquals(2, mdDescs.length);
        
        String stdName = "vmf://ns.intel.com/vmf/std-dst-schema-1.0";
        
        MetadataSchema std = MetadataSchema.getStdSchema();
        
        assertEquals(stdName, std.getName());
        
        assertEquals(stdName, MetadataSchema.getStdSchemaName ());
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testAddThrow()
    {
        schema.add(mdDesc1);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Metadata with same name already exists!");
        schema.add(mdDesc1);
    }
    
    @Test
    public void testCreateSchemaThrow()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Schema name can't be empty.");
        @SuppressWarnings("unused")
        MetadataSchema newSchema = new MetadataSchema("");
    }
    
    @Test
    public void testDeleteByGC()
    {
        schema = null;
        
        fields1 = null;
        fields2 = null;
        
        refs = null;
        
        mdDesc1 = null;
        mdDesc2 = null;
        
        md1 = null;
        md2 = null;
        
        System.gc();
    }
}
