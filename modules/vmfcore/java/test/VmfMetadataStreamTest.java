import com.intel.vmf.MetadataStream;
import com.intel.vmf.MetadataStream.VideoSegment;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.FieldValue;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.XMLWriter;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.io.File;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
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
    
    protected final String srcFile = "BlueSquare.avi";
    protected final String dstFile = "test.avi";
    
    void copy (String srcFile, String dstFile) throws IOException 
    {
    	FileInputStream fileInputStream = new FileInputStream(srcFile);
		FileChannel srcChannel = fileInputStream.getChannel();
    	    
    	FileOutputStream fileOutputStream = new FileOutputStream(dstFile);
		FileChannel dstChannel = fileOutputStream.getChannel();

    	dstChannel.transferFrom(srcChannel, 0, srcChannel.size());

    	srcChannel.close();
    	dstChannel.close();
    	fileInputStream.close();
    	fileOutputStream.close();
    }
    
    protected MetadataStream stream;
    protected MetadataSchema schema;
    
    protected final FieldDesc fields[] = new FieldDesc [3];
    
    protected final ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected MetadataDesc mdDesc;
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public void setUp ()
    {
    	schema = new MetadataSchema ("test_schema");
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        mdDesc = new MetadataDesc ("person", fields, refs);
        
        schema.add (mdDesc);
        stream = new MetadataStream ();
        stream.addSchema(schema);
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
    }
    
    @After
    public void tearDown()
    {
    	stream.clear();
    }
    
    @Test
    public void testSaveLoadAndQueries() throws IOException
    {
    	copy (srcFile, dstFile);
    	
    	assertEquals(schema.getName(), stream.getSchema("test_schema").getName());
        assertEquals(schema.getAuthor(), stream.getSchema("test_schema").getAuthor());
        assertEquals(schema.getAll().length, stream.getSchema("test_schema").getAll().length);
    	
        assertTrue (stream.open(dstFile, MetadataStream.ReadWrite));
        stream.getChecksum ();
        
        String str = stream.computeChecksum ();
        assertFalse (str.isEmpty());
        
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
        
        stream.addSchema(schema);
        
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
        
        Metadata md = mdSet3.getElement(0);
        String names[] = md.getFieldNames();
        assertEquals(3, names.length);
        
        MetadataSet mdSet4 = stream.queryByName ("person");
        assertEquals(2, mdSet4.getSize());
        
        FieldValue fv = new FieldValue("name", var1);

        MetadataSet mdSet5 = stream.getAll();
        assertEquals(2, mdSet5.getSize());
        
        mdSet5 = stream.queryByNameAndValue ("person", fv);
        assertEquals(1, mdSet5.getSize());
        
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
        
        mdSet4 = stream.queryByName ("person");
        assertEquals(3, mdSet4.getSize());
        
        FieldValue fvs[] = new FieldValue [2];
        fvs[0] = new FieldValue("last name", var2);
        fvs[1] = new FieldValue("age", var3);
        
        MetadataSet mdSet6 = stream.queryByNameAndFields ("person", fvs);
        assertEquals(1, mdSet6.getSize());
        
        MetadataSet mdSet7 = stream.queryByReference ("person");
        assertEquals(1, mdSet7.getSize());
        
        MetadataSet mdSet8 = stream.queryByReference ("person", fv);
        assertEquals(1, mdSet8.getSize());
        
        MetadataSet mdSet9 = stream.queryByReference ("person", fvs);
        assertEquals(1, mdSet9.getSize());
        
        XMLWriter writer = new XMLWriter ();
        
        String serialized = stream.serialize (writer);
        assertFalse (serialized.isEmpty());
        
        stream.remove (mdSet4);
        mdSet4 = stream.queryByName("person");
        assertEquals(0, mdSet4.getSize());
        
        stream.add(md3);
        stream.add(md1);
        
        stream.remove(schema);
        mdSet1 = stream.queryBySchema("test_schema");
        assertEquals(0, mdSet1.getSize());
        
        mdSet1 = stream.getAll();
        assertEquals(0, mdSet1.getSize());

        new File(dstFile).delete();
    }
    
    @Test
    public void testVideoSegments()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment("holiday", 35, 30);
        MetadataStream.VideoSegment s2 = new MetadataStream.VideoSegment();
        MetadataStream.VideoSegment s3 = new MetadataStream.VideoSegment();
        
        s2.setTitle("vacation");
        s3.setTitle("rest");
        
        s2.setTime(0);
        s3.setTime(65);
        
        s2.setFPS(25);
        s3.setFPS(40);
        
        stream.addVideoSegment (s1);
        stream.addVideoSegment (s2);
        stream.addVideoSegment (s3);
        
        VideoSegment vs[] = stream.getAllVideoSegments ();
        assertEquals(3, vs.length);
    }
}
