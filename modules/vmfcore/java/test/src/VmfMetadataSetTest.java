import com.intel.vmf.MetadataStream;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.FieldValue;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfMetadataSetTest 
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
    
    protected MetadataStream stream;
    protected MetadataSchema schema;
    
    protected final FieldDesc fields[] = new FieldDesc [3];
    
    protected final ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected final MetadataDesc mdDesc = new MetadataDesc ("person", fields, refs);
    
    protected MetadataSet mdSet1;
    
    protected Variant var1;
    protected Variant var2;
    protected Variant var3;
    
    @Before
    public void setUp ()
    {
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);
        
        Metadata md1 = new Metadata (mdDesc);
        Metadata md2 = new Metadata (mdDesc);
        Metadata md3 = new Metadata (mdDesc);
        
        schema = new MetadataSchema ("test_schema");
        stream = new MetadataStream ();
        
        schema.add (mdDesc);
        
        stream.addSchema (schema);
        
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
        
        mdSet1 = stream.getAll();
    }
    
    @Test
    public void testQueries()
    {
        System.out.println("Inside VmfMetadataSetTest.testQueries()");
        
        assertEquals(3, mdSet1.getSize());
        
        MetadataSet mdSet2 = mdSet1.queryBySchema("test_schema");
        assertEquals(3, mdSet2.getSize());
        
        mdSet2 = mdSet1.queryByFrameIndex (0);
        assertEquals(2, mdSet2.getSize());
        
        mdSet2 = mdSet1.queryByTime (0, 2);
        assertEquals(2, mdSet2.getSize());
        
        mdSet2 = mdSet1.queryByName("person");
        assertEquals(2, mdSet2.getSize());
        
        FieldValue fv = new FieldValue("name", var1);
        
        mdSet2 = stream.queryByNameAndValue ("person", fv);
        assertEquals(3, mdSet2.getSize());
        
        FieldValue fvs[] = new FieldValue [2];
        
        fvs[0] = new FieldValue("last name", var2);
        fvs[1] = new FieldValue("age", var3);
        
        mdSet2 = stream.queryByNameAndFields ("person", fvs);
        assertEquals(3, mdSet2.getSize());
        
        mdSet2 = stream.queryByReference ("person");
        assertEquals(5, mdSet2.getSize());
        
        mdSet2 = stream.queryByReference ("person", fv);
        assertEquals(5, mdSet2.getSize());
        
        mdSet2 = stream.queryByReference ("person", fvs);
        assertEquals(5, mdSet2.getSize());
    }
 }
