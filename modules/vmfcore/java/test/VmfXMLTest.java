import java.util.HashMap;

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
    
    protected FormatXML xml;
    
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
        
    	xml = new FormatXML();
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testXMLReaderAndWriter()
    {
    	String schemasStr = xml.store(null, schemas, null, null);
        assertFalse(schemasStr.isEmpty());
        
        Format.Data data = xml.parse(schemasStr);
        assertNotNull(data.schemas);
        assertEquals (schemas.length, data.schemas.length);
        
        String str = xml.store(mdSet, schemas, null, null);
        assertFalse(str.isEmpty());
        
        data = xml.parse(str);
        assertNotNull(data.metadata);
        assertEquals (mdSet.getSize(), data.metadata.length);
        assertEquals (schemas.length,  data.schemas.length);
        assertNull (data.segments);
        
        String segments = xml.store(null, null, videoSegs, null);
        assertFalse(segments.isEmpty());
        
        data = xml.parse(segments);
        assertNotNull(data.segments);
        assertEquals (videoSegs.length, data.segments.length);
        
        HashMap<String, String> attribs = new HashMap<String, String>();
        attribs.put("nextId", "100");
        attribs.put("filepath", "path.txt");
        String all = xml.store(mdSet, schemas, videoSegs, attribs);
        assertFalse(all.isEmpty());

        data = xml.parse(all);
        assertEquals (mdSet.getSize(), data.metadata.length);
        assertEquals (schemas.length,  data.schemas.length);
        assertEquals (videoSegs.length, data.segments.length);
        assertEquals (2, data.attrib.size());
        assertEquals ("100", data.attrib.get("nextId"));
}
    
    @Test
    public void testXMLParseSchemaThrown()
    {
        String text = "";
        
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Empty input XML string");
        xml.parse(text);
    }

   
    @Test
    public void testXMLDeleteByGC()
    {
        xml = null;
        
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
}
