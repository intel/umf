import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Reference;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfMetadataTest 
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
    
    final protected MetadataSchema schema = new MetadataSchema ("test_schema");
    
    final protected FieldDesc fields[] = new FieldDesc [3];
    
    final protected ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected MetadataDesc mdDesc;
    
    protected Metadata md1;
    protected Metadata md2;
    
    @Before
    public void setUp ()
    {
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        mdDesc = new MetadataDesc ("person", fields, refs);
        
        md1 = new Metadata(mdDesc);
        md2 = new Metadata(mdDesc);
    }
    
    @Test
    public void testSettersGetters ()
    {
        System.out.println("Inside VmfMetadataTest.testSettersGetters()");
        
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
        
        assertEquals("person", md1.getName());
        assertEquals("", md1.getSchemaName());
        assertEquals(mdDesc, md1.getDesc());
        Variant tmp = md1.getFieldValue ("name");
        assertTrue(tmp.isEmpty());
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
        
        assertEquals (var1, md1.getFieldValue ("name"));
        assertEquals (var2, md1.getFieldValue ("last name"));
        assertEquals (var3, md1.getFieldValue ("age"));
        
        schema.add (mdDesc);
        Metadata md3 = new Metadata(mdDesc);
        
        assertEquals("test_schema", md3.getSchemaName());
    }
    
    @Test
    public void testReferences()
    {
        System.out.println("Inside VmfMetadataTest.testReferences()");
        
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
        
        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "colleague");
        
        assertTrue(md2.isReference (md1, "spouse"));
        assertTrue(md2.isReference (md1, "colleague"));
        assertTrue(md2.isReference (md2));
        
        assertEquals (md2, md2.getFirstReference("person"));
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
        
        md2.removeReference (md2);
        
        set = md2.getReferencesByName ("");
        assertEquals (0, set.getSize());
        
        refs = md2.getAllReferences();
        assertEquals (2, refs.length);
        
        md2.removeReference (md1, "colleague");
        
        set = md2.getReferencesByName ("colleague");
        assertEquals (0, set.getSize());
        
        refs = md2.getAllReferences();
        assertEquals (1, refs.length);
    }
}
