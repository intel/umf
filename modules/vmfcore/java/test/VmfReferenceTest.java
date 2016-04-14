import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.Log;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Reference;
import com.intel.vmf.Variant;
import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfReferenceTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected final FieldDesc fds1[] = new FieldDesc [3];
    protected final FieldDesc fds2[] = new FieldDesc [4];
    
    protected MetadataDesc mdDesc1;
    protected MetadataDesc mdDesc2;
    
    protected final ReferenceDesc rd1 = new ReferenceDesc ("friend");
    protected final ReferenceDesc rd2 = new ReferenceDesc ("colleague", false, true);
    protected final ReferenceDesc rd3 = new ReferenceDesc ("spouse", true, false); 
    
    protected Metadata md1;
    protected Metadata md2;
    
    protected Reference ref1;
    protected Reference ref2;
     
    @Before
    public void setUp ()
    {
        fds1[0] = new FieldDesc ("name", Variant.type_string, false);
        fds1[1] = new FieldDesc ("last name", Variant.type_string, false);
        fds1[2] = new FieldDesc ("age", Variant.type_integer, false);
        
        fds2[0] = new FieldDesc ("manufacturer", Variant.type_string, false);
        fds2[1] = new FieldDesc ("model", Variant.type_string, false);
        fds2[2] = new FieldDesc ("number", Variant.type_string, false);
        fds2[3] = new FieldDesc ("age", Variant.type_integer, false);
        
        mdDesc1 = new MetadataDesc ("person", fds1);
        mdDesc2 = new MetadataDesc ("car", fds2);
        
        md1 = new Metadata (mdDesc1);
        md2 = new Metadata (mdDesc2);
        
        ref1 = new Reference ();
        ref2 = new Reference (rd1, md1);
    }
    
    @Test
    public void testReferenceDesc ()
    {
    	/*TO DO: Fix for native code Metadata.operator ==.
        assertFalse(md2.equals(md1));*/
    	
        ref1.setReferenceMetadata (md2);
        
        /*TO DO: Fix for native code Metadata.operator ==.
        assertTrue(md2.equals(ref1.getReferenceMetadata ()));*/
        
        assertEquals(md2.getName(), ref1.getReferenceMetadata().getName());
        assertEquals(md2.getDesc().getMetadataName(), ref1.getReferenceMetadata().getDesc().getMetadataName());
        assertEquals(md2.getDesc().getAllReferenceDescs().length, ref1.getReferenceMetadata ().getDesc().getAllReferenceDescs().length);
        assertEquals(md2.getDesc().getFields().length, ref1.getReferenceMetadata ().getDesc().getFields().length);
        assertEquals(md2.getID(), ref1.getReferenceMetadata().getID());
        assertEquals(md2.getDuration(), ref1.getReferenceMetadata().getDuration());
        assertEquals(md2.getAllReferences().length, ref1.getReferenceMetadata().getAllReferences().length);
        assertEquals(md2.getFieldNames().length, ref1.getReferenceMetadata().getFieldNames().length);
        assertEquals(md2.getFrameIndex(), ref1.getReferenceMetadata().getFrameIndex());
        assertEquals(md2.getNumOfFrames(), ref1.getReferenceMetadata().getNumOfFrames());
        
        /*TO DO: Fix for native code Metadata.operator ==.
        assertTrue(md1.equals(ref2.getReferenceMetadata ()));*/
        
        assertEquals(md1.getName(), ref2.getReferenceMetadata().getName());
        assertEquals(md1.getDesc().getMetadataName(), ref2.getReferenceMetadata().getDesc().getMetadataName());
        assertEquals(md1.getDesc().getAllReferenceDescs().length, ref2.getReferenceMetadata ().getDesc().getAllReferenceDescs().length);
        assertEquals(md1.getDesc().getFields().length, ref2.getReferenceMetadata ().getDesc().getFields().length);
        assertEquals(md1.getID(), ref2.getReferenceMetadata().getID());
        assertEquals(md1.getDuration(), ref2.getReferenceMetadata().getDuration());
        assertEquals(md1.getAllReferences().length, ref2.getReferenceMetadata().getAllReferences().length);
        assertEquals(md1.getFieldNames().length, ref2.getReferenceMetadata().getFieldNames().length);
        assertEquals(md1.getFrameIndex(), ref2.getReferenceMetadata().getFrameIndex());
        assertEquals(md1.getNumOfFrames(), ref2.getReferenceMetadata().getNumOfFrames());
        
        assertEquals(rd1.getName(), ref2.getReferenceDescription().getName());
        assertEquals(rd1.isUnique(), ref2.getReferenceDescription().isUnique());
        assertEquals(rd1.isCustom(), ref2.getReferenceDescription().isCustom());
        
        ref2.setReferenceMetadata (md2);
        assertEquals(md2.getName(), ref2.getReferenceMetadata().getName());
        assertEquals(md2.getDesc().getMetadataName(), ref2.getReferenceMetadata().getDesc().getMetadataName());
        assertEquals(md2.getDesc().getAllReferenceDescs().length, ref2.getReferenceMetadata ().getDesc().getAllReferenceDescs().length);
        assertEquals(md2.getDesc().getFields().length, ref2.getReferenceMetadata ().getDesc().getFields().length);
        assertEquals(md2.getID(), ref2.getReferenceMetadata().getID());
        assertEquals(md2.getDuration(), ref2.getReferenceMetadata().getDuration());
        assertEquals(md2.getAllReferences().length, ref2.getReferenceMetadata().getAllReferences().length);
        assertEquals(md2.getFieldNames().length, ref2.getReferenceMetadata().getFieldNames().length);
        assertEquals(md2.getFrameIndex(), ref2.getReferenceMetadata().getFrameIndex());
        assertEquals(md2.getNumOfFrames(), ref2.getReferenceMetadata().getNumOfFrames());
    }
    
    @Test
    public void testDeleteByGC()
    {
        ref1 = null;
        ref2 = null;
        
        md1 = null;
        md2 = null;
        
        mdDesc1 = null;
        mdDesc2 = null;
        
        System.gc();
    }
}
