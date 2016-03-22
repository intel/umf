import com.intel.vmf.*;

import static org.junit.Assert.*;
import org.junit.*;
import org.junit.rules.ExpectedException;

public class VmfXMLTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
	
    protected MetadataStream stream;
    protected final MetadataSchema schema1 = new MetadataSchema ("people");
    protected final MetadataSchema schema2 = new MetadataSchema ("transport");

    protected MetadataSchema schemas[];
    
    protected FieldDesc fields1[] = new FieldDesc [3];
    protected FieldDesc fields2[] = new FieldDesc [4];
    
    protected ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected MetadataDesc mdDesc1; 
    protected MetadataDesc mdDesc2;
    
    protected MetadataStream.VideoSegment videoSeg1;
    protected MetadataStream.VideoSegment videoSeg2;
    protected MetadataStream.VideoSegment videoSeg3;
    
    protected MetadataStream.VideoSegment videoSegs[];
    
    //protected XMLReader reader;
    //protected XMLWriter writer;
    
    protected MetadataSet mdSet;
    
    protected Variant var1;
    protected Variant var2;
    protected Variant var3;
    
    @Before
    public void setUp ()
    {
    	stream = new MetadataStream ();
    	
    	videoSegs = new MetadataStream.VideoSegment[3];
        schemas = new MetadataSchema[2];
        schemas[0] = schema1;
        schemas[1] = schema2;
    	
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
        
        schema1.add(mdDesc1);
        schema2.add(mdDesc2);
        
        stream.addSchema (schema1);
        stream.addSchema (schema2);
        
        Metadata md1 = new Metadata (mdDesc1);
        Metadata md2 = new Metadata (mdDesc1);
        Metadata md3 = new Metadata (mdDesc1);
        
        md1.setFrameIndex(0);
        md2.setFrameIndex(4);
        md3.setFrameIndex(0);
        
        md1.setTimestamp (0, 4);
        md2.setTimestamp (1, 3);
        md3.setTimestamp (3, 4);
        
        var1 = new Variant ("Den");
        var2 = new Variant ("Smith");
        var3 = new Variant (24);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        var1.setTo ("John");
        var2.setTo ("Sean");
        var3.setTo (25);
        
        md3.setFieldValue("name", var1);
        md3.setFieldValue("last name", var2);
        md3.setFieldValue("age", var3);
        
        stream.add (md1);
        stream.add (md2);
        stream.add (md3);
        
        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "friend");
        md2.addReference(md3, "friend");
        md2.addReference(md3, "colleague");
        
        mdSet = stream.getAll();
    	
        videoSeg1 = new MetadataStream.VideoSegment("holiday", 35, 0);
        videoSeg2 = new MetadataStream.VideoSegment("vacation", 30, 15);
        videoSeg3 = new MetadataStream.VideoSegment("rest", 40, 50);
        
        videoSegs[0] = videoSeg1;
        videoSegs[1] = videoSeg2;
        videoSegs[2] = videoSeg3;
        
    	//reader = new XMLReader();
        //writer = new XMLWriter();
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testXML_DUMMY()
    {
        String text = "";
    }
    /*    
    @Test
    public void testXMLReaderAndWriter()
    {
        String schemasStr = writer.store(schema1, schema2);
        assertFalse(schemasStr.isEmpty());
        
        MetadataSchema mdSchemas[] = reader.parseSchemas(schemasStr);
        assertEquals (2, mdSchemas.length);
        
        String str = writer.store(mdSet);
        assertFalse(str.isEmpty());
        
        MetadataInternal mdInt[] = reader.parseMetadata(str, schema1, schema2);
        assertEquals (mdSet.getSize(), mdInt.length);
        
        String segments = writer.store(videoSeg1, videoSeg2, videoSeg3);
        assertFalse(segments.isEmpty());
        
        MetadataStream.VideoSegment videoSegs1[] = reader.parseVideoSegments(segments);
        assertEquals (3, videoSegs1.length);
        
        String all = writer.store(1, "path.txt", "trampampam", videoSegs, schemas, mdSet);
        assertFalse(all.isEmpty());
    }
    
    @Test
    public void testXMLParseSchemaThrown()
    {
        String text = "";
        
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Schemas can not be parsed.");
        reader.parseSchemas(text);
    }

    @Test
    public void testXMLParseMetadataThrown()
    {
        String text = "";
        
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Metadata can not be parsed.");
        reader.parseMetadata(text);
    }
    
    @Test
    public void testXMLParseSegmentsThrown()
    {
        String text = "";
        
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segments can not be parsed.");
        reader.parseVideoSegments(text);
    }
    
    @Test
    public void testXMLStoreSchemaThrown()
    {
        MetadataSchema emptySchemas[] = new MetadataSchema[0];
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Input schemas vector is empty");
        writer.store(emptySchemas);
    }
    
    @Test
    public void testXMLStoreSetThrown()
    {
        MetadataSet set = new MetadataSet();
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Input MetadataSet is empty");
        writer.store(set);
    }
    
    @Test
    public void testXMLStoreSegmentThrown()
    {
        MetadataStream.VideoSegment segment = new MetadataStream.VideoSegment();
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Invalid segment. Segment must have not empty title, fps > 0 and start time >= 0");
        writer.store(segment);
    }
    
    @Test
    public void testXMLStoreSegmentsThrown()
    {
        MetadataStream.VideoSegment segments[] = new MetadataStream.VideoSegment[0];
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Input video segments vector is empty");
        writer.store(segments);
    }

    @Test
    public void testXMLStoreAllThrown()
    {
        MetadataSchema emptySchemas[] = new MetadataSchema[0];
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Input schemas vector is empty");
        writer.store(1, "path.txt", "trampampam", videoSegs, emptySchemas, mdSet);
    }
    
    @Test
    public void testXMLDeleteByGC()
    {
        reader = null;
        writer = null;
        
        videoSeg1 = null;
        videoSeg2 = null;
        videoSeg3 = null;
        
        var1 = null;
        var2 = null;
        var3 = null;
        
        mdDesc1 = null;
        mdDesc2 = null;
        
        mdSet = null;
        stream = null;
        
        System.gc();
    }
    */
}
