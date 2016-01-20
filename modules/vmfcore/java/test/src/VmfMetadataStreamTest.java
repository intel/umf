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

public class VmfMetadataStreamTest 
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
    
    String videoFile = "D:/Projects/intel_vmf/vmf/data/BlueSquare.avi";
    protected MetadataStream stream;
    protected MetadataSchema schema;
    
    protected FieldDesc fields[] = new FieldDesc [3];
    fields[0] = new FieldDesc ("name", Variant.type_string, false);
    fields[1] = new FieldDesc ("last name", Variant.type_string, false);
    fields[2] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected ReferenceDesc refs[] = new ReferenceDesc [3];
    refs[0] = new ReferenceDesc ("friend");
    refs[1] = new ReferenceDesc ("colleague", false, true);
    refs[2] = new ReferenceDesc ("spouse", true);
    
    protected MetadataDesc mdDesc = new MetadataDesc ("person", fields, refs);
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public static void setUp ()
    {
        stream = new MetadataStream ();
        schema = new MetadataSchema ("test_schema");
        schema.add (mdDesc);
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
    }
    
    @Test
    public void testNotNull()
    {
        System.out.println("Inside VmfMetadataStreamTest.testNotNull()");
        
        assertNotNull(stream);
        assertNotNull(schema);  
        assertNotNull(md1);
        assertNotNull(md2);
    }
    
    @Test
    public void testSaveLoad()
    {
        System.out.println("Inside VmfMetadataStreamTest.testSaveLoad()");
        
        assertTrue (stream.open(videoFile, MetadataStream.ReadWrite));
        
        stream.addSchema(schema);
        assertEquals(schema, stream.getSchema("test_schema"));
        
        Variant var1 = new Variant ("Den");
        Variant var2 = new Variant ("Smith");
        Variant var3 = new Variant (21);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "colleague");
        
        stream.add(md1);
        stream.add(md2);
        
        stream.save();
        stream.close();
        
        assertTrue (stream.reopen(MetadataStream.ReadOnly));
        
        stream.load ("test_schema");
        
        MetadataSet mds = stream.queryBySchema("test_schema");
        assertEquals(2, mds.getSize());
        
        MetadataSet property = mds.queryByName("person");
        assertEquals(2, property.getSize());
        
    }
}