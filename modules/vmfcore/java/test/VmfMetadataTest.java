import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

import com.intel.vmf.FieldDesc;
import com.intel.vmf.Log;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Reference;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;

public class VmfMetadataTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected MetadataStream stream; 
    protected final MetadataSchema schema = new MetadataSchema ("test_schema");
    
    protected FieldDesc fields[];
    
    protected ReferenceDesc refs[];
    
    protected MetadataDesc mdDesc;
    
    protected Metadata md1;
    protected Metadata md2;
    protected Metadata md3;
    
    @Before
    public void setUp ()
    {
        fields = new FieldDesc [3];
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs = new ReferenceDesc [3];
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        mdDesc = new MetadataDesc ("person", fields, refs);
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
       
        schema.add (mdDesc);
        
        stream = new MetadataStream();
        stream.addSchema(schema);
    }
    
    @Test
    public void testSettersAndGetters ()
    {
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
        
        /*TO DO: Fix for native code Metadata.operator ==.
        assertTrue(md2.equals(md1));*/
        
        assertEquals("person", md1.getName());
        assertEquals("", md1.getSchemaName());
        
        MetadataDesc mdDesc1 = md1.getDesc();
        assertEquals(mdDesc.getMetadataName(), mdDesc1.getMetadataName());
        assertEquals(mdDesc.getFields().length, mdDesc1.getFields().length);
        assertEquals(mdDesc.getAllReferenceDescs().length, mdDesc1.getAllReferenceDescs().length);
        
        assertFalse(md1.hasField("age"));
        md1.setFrameIndex(0, 2);
        assertEquals(0, md1.getFrameIndex ());
        assertEquals(2, md1.getNumOfFrames ());
        md1.setTimestamp (0, 1);
        assertEquals(0, md1.getTime ());
        assertEquals(1, md1.getDuration ());
        
        Variant var1 = new Variant ("Den");
        Variant var2 = new Variant ("Smith");
        Variant var3 = new Variant (21);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        assertTrue (var1.equals(md1.getFieldValue ("name")));
        assertTrue (var2.equals(md1.getFieldValue ("last name")));
        assertTrue(var3.equals(md1.getFieldValue ("age")));
        
        Metadata md3 = new Metadata(mdDesc);
        
        assertEquals("test_schema", md3.getSchemaName());
    }
    
    @Test
    public void testReferences()
    {
        Variant var1 = new Variant ("Den");
        Variant var2 = new Variant ("Smith");
        Variant var3 = new Variant (21);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (26);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.add(md1);
        stream.add(md2);
        
        assertTrue(md1.isValid());
        assertTrue(md2.isValid());
        
        assertTrue(md1.lessThan(md2));
        
        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "colleague");
        
        assertTrue(md2.isReference (md1, "spouse"));
        assertTrue(md2.isReference (md1, "colleague"));
        assertTrue(md2.isReference (md2));
        
        assertEquals(0, md1.getID ());
        assertEquals(1, md2.getID ());
        
        assertTrue(md2.isReference (0, "spouse"));
        assertTrue(md2.isReference (0, "colleague"));
        assertTrue(md2.isReference (1));
        
        Metadata tmp = md2.getFirstReference("person");
        assertTrue(tmp.equals(md2));
        
        tmp = md2.getFirstReference("new");
        assertNull(tmp);

        MetadataSet set = md2.getReferencesByMetadata ("person");
        assertEquals (3, set.getSize());
        
        set = md2.getReferencesByName ("spouse");
        assertEquals (1, set.getSize());
        set = md2.getReferencesByName ("colleague");
        assertEquals (1, set.getSize());
        set = md2.getReferencesByName ("");
        assertEquals (1, set.getSize());
        
        Reference refs[] = md2.getAllReferences();
        assertEquals (3, refs.length);
        
        assertEquals(0, md1.getID ());
        assertEquals(1, md2.getID ());
        
        md2.removeReference (0, "spouse");
        refs = md2.getAllReferences();
        assertEquals (2, refs.length);
        set = md2.getReferencesByName ("spouse");
        assertEquals (0, set.getSize());
        
        md2.removeReference (md2);
        set = md2.getReferencesByName ("");
        assertEquals (0, set.getSize());

        refs = md2.getAllReferences();
        assertEquals (1, refs.length);
        
        md2.removeReference (md1, "colleague");
        
        set = md2.getReferencesByName ("colleague");
        assertEquals (0, set.getSize());
        
        refs = md2.getAllReferences();
        assertEquals (0, refs.length);
        
        FieldDesc fds[];
        fds = new FieldDesc [1];
        fds[0] = new FieldDesc ("", Variant.type_string, true);
        MetadataDesc desc = new MetadataDesc ("empty", fds);
        md3 = new Metadata(desc);
        md3.addValue(var1);
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testEqualsTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Cannot compare metadata that has not been added to any stream!");
        md1.equals(md2);
    }
    
    @Test
    public void testLessThanTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Cannot compare metadata that has not been added to any stream!");
        md1.lessThan(md2);
    }
    
    @Test
    public void testValidateTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: The metadata contains neither value nor encrypted data");
        md1.validate();
    }
    
    @Test
    public void testValidateNotTrown()
    {
        Variant var1 = new Variant ("Den");
        Variant var2 = new Variant ("Smith");
        Variant var3 = new Variant (21);
        
        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);
        
        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (26);
        
        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);
        
        stream.add(md1);
        stream.add(md2);
        
        md1.validate();
        md2.validate();
    }
    
    @Test
    public void testSetFrameIndexTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Can't set metadata frame index. Invalid frame index value");
        md1.setFrameIndex(-2);
    }
    
    @Test
    public void testSetNumOfFramesTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Can't set metadata number of frames. Invalid number of frames value");
        md1.setFrameIndex(1, -1);
    }
    
    @Test
    public void testSetTimestampTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Can't set metadata timestamp. Invalid timestamp value");
        md1.setTimestamp(-2);
    }
    
    @Test
    public void testSetDurationTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Can't set metadata duration. Invalid duration value");
        md1.setTimestamp(1, -1);
    }
    
    @Test
    public void testGetFieldValueTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Metadata field not found in metadata description");
        md1.getFieldValue("my");
    }
    
    @Test
    public void testGetReferencesByMetadataTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: MetadataName is empty!");
        md1.getReferencesByMetadata("");
    }
    
    @Test
    public void testAddReferencesTrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Metadata has to be added into stream before being referenced!");
        md1.addReference(md1);
    }
    
    @Test
    public void testSetFieldValueTrown()
    {
        Variant var1 = new Variant ("Den");
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Metadata field not found in metadata description");
        md1.setFieldValue("empty", var1);
    }
    
}
