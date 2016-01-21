import junit.framework.*;
import com.intel.vmf.MetadataStream;
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
import org.junit.Test;

public class VmfReferenceTest 
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
    protected final FieldDesc fds[] = new FieldDesc [3];
    fds[0] = new FieldDesc ("name", Variant.type_string, false);
    fds[1] = new FieldDesc ("last name", Variant.type_string, false);
    fds[2] = new FieldDesc ("age", Variant.type_integer, false);
    
    protected final MetadataDesc mdDesc = new MetadataDesc ("person", fds);
    
    protected final ReferenceDesc rd1 = new ReferenceDesc ("friend");
    protected final ReferenceDesc rd2 = new ReferenceDesc ("colleague", false, true);
    protected final ReferenceDesc rd3 = new ReferenceDesc ("spouse", true); 
    
    protected final Metadata md1 = new Metadata (mdDesc);
    protected final Metadata md2 = new Metadata (mdDesc);
    
    protected Reference ref1;
    protected Reference ref2;
     
    @Before
    public static void setUp ()
    {
        ref1 = new Reference ();
        ref2 = new Reference (rd1, md1);
    }
    
    @Test
    public void testReferenceDesc ()
    {
        System.out.println("Inside VmfReferenceDescTest.testReferenceDesc()");
        
        ref1.setReferenceMetadata (md2);
        
        assertEquals(md2, ref1.getReferenceMetadata ());
        assertEquals(md1, ref2.getReferenceMetadata ());
        
        assertEquals(rd1, ref2.getReferenceDescription ());
        
        ref2.setReferenceMetadata (md2);
        assertEquals(md2, ref2.getReferenceMetadata ());
    }
}
