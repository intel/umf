import junit.framework.*;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;
import org.junit.Test;

public class VmfMetadataStreamTest 
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
    
    protected final String videoFile = "D:/Projects/intel_vmf/vmf/data/BlueSquare.avi";
    protected MetadataStream stream;
    protected final MetadataSchema schema = new MetadataSchema ("test_schema");
    
    protected final FieldDesc fields[] = new FieldDesc [3];
    fields[0] = new FieldDesc ("name", Variant.type_string, false);
    fields[1] = new FieldDesc ("last name", Variant.type_string, false);
    fields[2] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected final ReferenceDesc refs[] = new ReferenceDesc [3];
    refs[0] = new ReferenceDesc ("friend");
    refs[1] = new ReferenceDesc ("colleague", false, true);
    refs[2] = new ReferenceDesc ("spouse", true);
    
    protected final MetadataDesc mdDesc = new MetadataDesc ("person", fields, refs);
    
    schema.add (mdDesc);
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public static void setUp ()
    {
        stream = new MetadataStream ();
    }
    
    @Test
    public void testSaveLoadAndQueries()
    {
        System.out.println("Inside VmfMetadataStreamTest.testSaveLoadAndQueries()");
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
        
        assertNotNull(md1);
        assertNotNull(md2);
        
        assertTrue (stream.open(videoFile, MetadataStream.ReadWrite));
        
        String str1 = stream.getChecksum ();
        assertFalse (str1.isEmpty());
        
        String str2 = stream.computeChecksum ();
        assertFalse (str2.isEmpty());
        
        assertEquals(str1, str2);
        
        stream.addSchema(schema);
        assertEquals(schema, stream.getSchema("test_schema"));
        
        Variant var1 = new Variant ("Den");
        Variant var2 = new Variant ("Smith");
        Variant var3 = new Variant (24);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.add(md1);
        stream.add(md2);
        
        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "friend");
        
        MetadataSet mdSet1 = stream.getAll();
        assertEquals(2, mdSet1.getSize());
        
        stream.save();
        stream.close();
        
        assertTrue (stream.reopen(MetadataStream.ReadOnly));
        
        stream.load ("test_schema");
        
        MetadataSet mdSet2 = stream.queryBySchema("test_schema");
        assertEquals(2, mdSet2.getSize());
        
        MetadataSet mdSet3 = mdSet2.queryByName("person");
        assertEquals(2, mdSet3.getSize());
        
        Metadata md = properties[0];
        String names[] = md->getFieldNames();
        
        assertEquals(3, names.length);
        
        MetadataSet mdSet4 = stream.queryByName ("person");
        assertEquals(2, mdSet4.getSize());
        
        FieldValue fv = new FieldValue("name", var1);
        
        MetadataSet mdSet5 = stream.queryByNameAndValue ("person", fv);
        assertEquals(2, mdSet5.getSize());
        
        var1.setTo ("John");
        var2.setTo ("Sean");
        var3.setTo (25);
        
        Metadata md3 = new Metadata (mdDesc); 
        
        md3.setFieldValue("name", var1);
        md3.setFieldValue("last name", var2);
        md3.setFieldValue("age", var3);
        
        stream.add(md3);
        
        md2.addReference(md3, "friend");
        md2.addReference(md3, "colleague");
        
        FieldValue fvs[] = new FieldValue [2];
        
        fvs[0] = new FieldValue("last name", var2);
        fvs[1] = new FieldValue("age", var3);
        
        MetadataSet mdSet6 = stream.queryByNameAndFields ("person", fvs);
        assertEquals(3, mdSet6.getSize());
        
        MetadataSet mdSet7 = stream.queryByReference ("person");
        assertEquals(5, mdSet7.getSize());
        
        MetadataSet mdSet8 = stream.queryByReference ("person", fv);
        assertEquals(5, mdSet8.getSize());
        
        MetadataSet mdSet9 = stream.queryByReference ("person", fvs);
        assertEquals(5, mdSet9.getSize());
        
        IWriter formater = new XMLWriter ();
        
        String serialized = stream.serialize (formater);
        assertFalse (serialized.isEmpty());
        
        stream.remove (mdSet6);
        mdSet6 = stream.queryByNameAndFields ("person", fvs);
        assertEquals(0, mdSet6.getSize());
        
        stream.add(md1);
        stream.add(md2);
        
        stream.remove(schema);
        mdSet = stream.queryBySchema("test_schema");
        assertEquals(0, mdSet.getSize());
        
        mdSet = stream.getAll();
        assertEquals(0, mdSet.getSize());
    }
    
    @Test
    public void testVideoSegments()
    {
        System.out.println("Inside VmfMetadataStreamTest.testVideoSegments()");
        
        Metadata.VideoSegment s1 = new Metadata.VideoSegment("holiday", 35, 30);
        Metadata.VideoSegment s2 = new Metadata.VideoSegment();
        Metadata.VideoSegment s3 = new Metadata.VideoSegment();
        
        stream.addVideoSegment (s1);
        stream.addVideoSegment (s2);
        stream.addVideoSegment (s3);
        
        VideoSegment vs[] = getAllVideoSegments ();
        assertEquals(3, vs.length);
    }
}
