import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

import com.intel.vmf.FieldDesc;
import com.intel.vmf.FieldValue;
import com.intel.vmf.Log;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.MetadataStream.VideoSegment;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;


public class VmfMetadataStreamTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
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
    
    protected FieldDesc fields[];
    
    protected ReferenceDesc refs[];
    
    protected MetadataDesc mdDesc;
    
    protected Metadata md1;
    protected Metadata md2;
    
    protected Variant var1;
    protected Variant var2;
    protected Variant var3;
    
    @Before
    public void setUp () throws IOException
    {
        copy (srcFile, dstFile);
        fields = new FieldDesc [3];
    	schema = new MetadataSchema ("test_schema");
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs = new ReferenceDesc [3];
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        mdDesc = new MetadataDesc ("person", fields, refs);
        
        schema.add (mdDesc);
        stream = new MetadataStream ();
        stream.addSchema(schema);
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
        
        var1 = new Variant ("Den");
        var2 = new Variant ("Smith");
        var3 = new Variant (24);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
    }
    
    @After
    public void tearDown()
    {
        if (stream != null)
        {
            stream.close();
            stream.clear();
        }
        
        File newFile = new File("new.avi");
        if (newFile.exists())
            newFile.delete();
    }
    
    @Test
    public void testSaveLoadAndQueries() throws IOException
    {
    	assertEquals(schema.getName(), stream.getSchema("test_schema").getName());
        assertEquals(schema.getAuthor(), stream.getSchema("test_schema").getAuthor());
        assertEquals(schema.getAll().length, stream.getSchema("test_schema").getAll().length);
    	
        assertTrue (stream.open(dstFile, MetadataStream.ReadWrite));
        assertTrue (stream.getChecksum ().isEmpty());
        
        String str = stream.computeChecksum ();
        assertFalse (str.isEmpty());
        
        long packetSize = 0, packetOffset = 0;
        long options[] = new long [2];
        options [0] = packetSize;
        options [1] = packetOffset;
        
        assertFalse (stream.computeChecksum (options).isEmpty());
        assertNotEquals(0, options [0]);
        assertNotEquals(0, options [1]);
        
        stream.setChecksum("");
        assertEquals ("", stream.getChecksum());
        
        stream.setChecksum(str);
        assertEquals (str, stream.getChecksum());
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.addSchema(schema);
        
        md1.setFrameIndex(30);
        md2.setFrameIndex(40);
        md1.setTimestamp(1500);
        md2.setTimestamp(1800);
        
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
        
        assertTrue(stream.load ("test_schema"));
        assertTrue(stream.load("test_schema","person"));
        
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
        md3.setFrameIndex(60);
        md3.setTimestamp(2100);
        
        stream.add(md3);
        
        Metadata newMd = stream.getById(0);
        assertTrue(newMd.equals(md1));
        
        md2.addReference(md3, "friend");
        md2.addReference(md3, "colleague");
        
        mdSet4 = stream.queryByFrameIndex(40);
        assertEquals(1, mdSet4.getSize());
        assertTrue(md2.equals(mdSet4.getElement(0)));
        
        mdSet4 = stream.queryByTime(1500, 1900);
        assertEquals(2, mdSet4.getSize());
        
        mdSet4 = stream.queryByName ("person");
        assertEquals(3, mdSet4.getSize());
        
        FieldValue fvs[] = new FieldValue [2];
        fvs[0] = new FieldValue("last name", var2);
        fvs[1] = new FieldValue("age", var3);
        
        MetadataSet mdSet6 = stream.queryByNameAndFields ("person", fvs);
        assertEquals(1, mdSet6.getSize());
        
        mdSet6 = stream.queryByReference ("person");
        assertEquals(1, mdSet6.getSize());
        
        mdSet6 = stream.queryByReference ("person", fv);
        assertEquals(1, mdSet6.getSize());
        
        mdSet6 = stream.queryByReference ("person", fvs);
        assertEquals(1, mdSet6.getSize());
        
        FieldValue emptyNameFv = new FieldValue();
        mdSet6 = stream.queryByReference ("person", emptyNameFv);
        assertEquals(0, mdSet6.getSize());
        
        /*XMLWriter writer = new XMLWriter ();
        String serialized = stream.serialize (writer);
        assertFalse (serialized.isEmpty());
        
        XMLReader reader = new XMLReader ();
        MetadataStream newStream = new MetadataStream();
        assertEquals(0, newStream.getAllSchemaNames().length);
        newStream.deserialize(serialized, reader);
        assertEquals(1, newStream.getAllSchemaNames().length);*/
        
        stream.remove (mdSet4);
        mdSet4 = stream.queryByName("person");
        assertEquals(0, mdSet4.getSize());
        
        stream.add(md3);
        stream.add(md1);
        
        assertEquals(2, stream.getAll().getSize());
        
        long num = md3.getID();
        stream.remove(num);
        
        assertEquals(1, stream.getAll().getSize());
        
        stream.remove(schema);
        mdSet1 = stream.queryBySchema("test_schema");
        assertEquals(0, mdSet1.getSize());
        
        mdSet1 = stream.getAll();
        assertEquals(0, mdSet1.getSize());
        
        stream.close();
        
        copy (srcFile, "new.avi");
        stream.saveTo("new.avi");
        stream.close();
        
        assertTrue (stream.open(dstFile, MetadataStream.ReadWrite));
        assertFalse (stream.open(dstFile, MetadataStream.ReadWrite));
        stream.close();
    }
    
    @Test
    public void testRemoveAll()
    {
        assertTrue (stream.open(dstFile, MetadataStream.ReadWrite));
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.addSchema(schema);
        
        stream.add(md1);
        stream.add(md2);
        
        assertEquals(2, stream.getAll().getSize());
        stream.remove();
        assertEquals(0, stream.getAll().getSize());
        
        assertEquals(0, stream.queryBySchema("test_schema").getSize());
        assertEquals(null, stream.getSchema("test_schema"));
        assertEquals(0, stream.getAllSchemaNames().length);
    }
    
    @Test
    public void testImport() throws IOException
    {
        assertTrue (stream.open(dstFile, MetadataStream.ReadWrite));
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.addSchema(schema);
        
        md1.setFrameIndex(4, 8);
        md2.setFrameIndex(5, 10);
        
        stream.add(md1);
        stream.add(md2);
        
        copy (srcFile, "new.avi");
        MetadataStream newStream = new MetadataStream();
        assertTrue (newStream.open("new.avi", MetadataStream.ReadWrite));
        
        newStream.addSchema(schema);
        
        MetadataSet set = stream.getAll();
        assertEquals(2, set.getSize());
        
        newStream.importSet(stream, set, 6, 2);
    }
    
    @Test
    public void testVideoSegments()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment("holiday", 20, 0);
        MetadataStream.VideoSegment s2 = new MetadataStream.VideoSegment();
        MetadataStream.VideoSegment s3 = new MetadataStream.VideoSegment();
        
        s2.setTitle("vacation");
        s3.setTitle("rest");
        
        s2.setTime(5000);
        s3.setTime(15000);
        
        s2.setFPS(20);
        s3.setFPS(20);
        
        s1.setDuration(5000);
        s2.setDuration(10000);
        s3.setDuration(1000);
        
        stream.addVideoSegment (s1);
        stream.addVideoSegment (s2);
        stream.addVideoSegment (s3);
        
        VideoSegment vs[] = stream.getAllVideoSegments ();
        assertEquals(3, vs.length);
        
        assertEquals(30, stream.convertTimestampToFrameIndex(1500, 100));
        assertEquals(2, stream.convertDurationToNumOfFrames(1500, 100));
        assertEquals(1500, stream.convertFrameIndexToTimestamp(30, 2));
        assertEquals(100, stream.convertNumOfFramesToDuration(30, 2));
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testAddSegmentThrow()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment("holiday", 20, 0);
        MetadataStream.VideoSegment s2 = new MetadataStream.VideoSegment("vacation", 20, 3000);
        s1.setDuration(4000);
        s2.setDuration(10000);
        stream.addVideoSegment (s1);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Input segment intersects a one of the already created segments");
        stream.addVideoSegment (s2);
    }
    
    @Test
    public void testAddSegmentTitleThrow()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment();
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Segment contains empty 'title' value");
        stream.addVideoSegment (s1);
    }
    
    @Test
    public void testAddSegmentStartTimeThrow()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment();
        s1.setTitle("title");
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Segment contains invalid 'timeStart' value");
        stream.addVideoSegment (s1);
    }
    
    @Test
    public void testAddSegmentFPSThrow()
    {
        MetadataStream.VideoSegment s1 = new MetadataStream.VideoSegment();
        s1.setTitle("title");
        s1.setTime(0);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Segment contains invalid 'FPS' value");
        stream.addVideoSegment (s1);
    }
    
    @Test
    public void testSaveToThrow()
    {
        stream.open(dstFile, MetadataStream.ReadWrite);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: The previous file has not been closed!");
        stream.saveTo("stream.log");
    }
    
    @Test
    public void testClearReopenThrow()
    {
        stream.clear();
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: No files has been assosiated with this stream");
        stream.reopen(MetadataStream.ReadOnly);
    }

    @Test
    public void testReopen2Throw()
    {
        stream.open(dstFile, MetadataStream.ReadWrite);
        
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: The previous file has not been closed!");
        stream.reopen(MetadataStream.ReadWrite);
    }
    
    @Test
    public void testReopen3Throw()
    {
        stream.open(dstFile, MetadataStream.ReadWrite);
        stream.close();
        stream.clear();
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: The file path is emtpy!");
        stream.reopen(MetadataStream.ReadWrite);
    }
    
    
    @Test
    public void testAddSchemaThrow()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Metadata Schema already exists!");
        stream.addSchema(schema);
    }
    
    @Test
    public void testDeleteByGC()
    {
        stream = null;
        schema = null;
        
        mdDesc = null;
        
        md1 = null;
        md2 = null;
        
        fields = null;
        refs = null;
        
        System.gc();
    }
}
