import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.Log;
import com.intel.vmf.Variant;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;


public class VmfMetadataDescTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected MetadataSchema schema = new MetadataSchema ("test_schema", "Anna");
    
    protected FieldDesc fields1[] = new FieldDesc [3];
    
    protected ReferenceDesc refs[] = new ReferenceDesc [3];
    
    protected ReferenceDesc refs1[] = new ReferenceDesc [3];
    
    protected FieldDesc fields2[] = new FieldDesc [4];
    
    protected MetadataDesc mdDesc1; 
    protected MetadataDesc mdDesc2;
    protected MetadataDesc mdDesc3;
    protected MetadataDesc mdDesc4;
    
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
        
        refs1[0] = new ReferenceDesc ();
        refs1[1] = new ReferenceDesc ("owner", true, false);
        refs1[2] = new ReferenceDesc ("mechanic", false, false);
        
        mdDesc1 = new MetadataDesc ("person", fields1, refs);
        mdDesc2 = new MetadataDesc ("car", fields2, refs1);
        mdDesc3 = new MetadataDesc ();
        mdDesc4 = new MetadataDesc ("", Variant.type_string);
    }
    
    @Test
    public void testMetadataDesc ()
    {
        assertEquals("person", mdDesc1.getMetadataName ());
        assertEquals("car", mdDesc2.getMetadataName ());
        
        String str = mdDesc3.getMetadataName ();
        assertTrue (str.isEmpty());
        
        str = mdDesc4.getMetadataName ();
        assertTrue (str.isEmpty());
        
        schema.add(mdDesc1);
        schema.add(mdDesc2);
        
        assertEquals("test_schema", mdDesc1.getSchemaName ());
        assertEquals("test_schema", mdDesc2.getSchemaName ());
        
        str = mdDesc3.getSchemaName ();
        assertTrue (str.isEmpty());
        
        FieldDesc fds[] = mdDesc1.getFields ();
        assertEquals (3, fds.length);
        
        fds = mdDesc2.getFields ();
        assertEquals (4, fds.length);
        
        ReferenceDesc rds1[] = mdDesc1.getAllReferenceDescs ();
        assertEquals(4, rds1.length);
        
        ReferenceDesc rds2[] = mdDesc2.getAllReferenceDescs ();
        assertEquals(3, rds2.length);
        
        mdDesc2.declareCustomReference ("father", true);
        
        rds2 = mdDesc2.getAllReferenceDescs ();
        assertEquals(4, rds2.length);
        
        ReferenceDesc rd1 = mdDesc1.getReferenceDesc ("friend");
        assertEquals(refs[0].getName(), rd1.getName());
        assertEquals(refs[0].isUnique(), rd1.isUnique());
        assertEquals(refs[0].isCustom(), rd1.isCustom());
        
        FieldDesc fd1 = mdDesc1.getFieldDesc ("age");
        assertEquals(fields1[2].getName(), fd1.getName());
        assertEquals(fields1[2].getType(), fd1.getType());
        assertEquals(fields1[2].isOptional(), fd1.isOptional());
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testDeclareCustomReferenceThrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: This reference name already exist!");
        mdDesc1.declareCustomReference ("friend", true);
    }
    
    @Test
    public void testCreateThrown()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Invalid metadata descriprion type.");
        @SuppressWarnings("unused")
        MetadataDesc mdDesc = new MetadataDesc ("name", Variant.type_empty);
    }
    
    @Test
    public void testDeleteByGC()
    {
        mdDesc1 = null;
        mdDesc2 = null;
        mdDesc3 = null;
        mdDesc4 = null;
        
        fields1 = null;
        fields2 = null;
        
        refs = null;
        
        System.gc();
    }
}
