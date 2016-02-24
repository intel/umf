import com.intel.vmf.Log;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Vmf;

import static org.junit.Assert.*;

import org.junit.*;
import org.junit.rules.ExpectedException;

public class VmfVideoSegmentTest 
{
    @BeforeClass
    public static void init()
    {
        Vmf.initialize();
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    @AfterClass
    public static void terminate()
    {
        Vmf.terminate();
    }
    
    protected MetadataStream.VideoSegment vs1;
    protected MetadataStream.VideoSegment vs2;
    protected MetadataStream.VideoSegment newSegment;
    
    @Before
    public void setUp ()
    {
        vs1 = new MetadataStream.VideoSegment();
        vs2 = new MetadataStream.VideoSegment("holiday", 35, 30);
    }
    
    @Test
    public void testGettersAndSetters()
    {
        assertEquals ("holiday", vs2.getTitle());
        assertEquals ("", vs1.getTitle ());
        
        vs1.setTitle ("travel");
        assertEquals ("travel", vs1.getTitle ());
        
        assertEquals (0, vs1.getFPS (), 0.00001);
        assertEquals (35, vs2.getFPS (), 0.00001);
        
        vs1.setFPS (30);
        assertEquals (30, vs1.getFPS (), 0.00001);
        
        assertEquals (0, vs1.getDuration ());
        assertEquals (0, vs2.getDuration ());
        
        vs1.setDuration (10);
        vs2.setDuration (20);
        
        assertEquals (10, vs1.getDuration ());
        assertEquals (20, vs2.getDuration ());
        
        assertEquals (-1, vs1.getTime ());
        assertEquals (30, vs2.getTime ());
        
        vs1.setTime (0);
        assertEquals (0, vs1.getTime ());
        
        assertEquals (0, vs1.getWidth ());
        assertEquals (0, vs1.getHeight ());
        
        assertEquals (0, vs2.getWidth ());
        assertEquals (0, vs2.getHeight ());
        
        vs1.setResolution (1280, 720);
        vs2.setResolution (1360, 768);
        
        assertEquals (1280, vs1.getWidth ());
        assertEquals (720, vs1.getHeight ());
        
        assertEquals (1360, vs2.getWidth ());
        assertEquals (768, vs2.getHeight ());
    }
    
    @Rule
    public ExpectedException thrown = ExpectedException.none();
    
    @Test
    public void testCreateSegmentTitleTrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment title can't be empty");
        newSegment = new MetadataStream.VideoSegment("", 25, 0);
    }
    
    @Test
    public void testCreateSegmentFPSThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment fps must be positive");
        newSegment = new MetadataStream.VideoSegment("vacation", -1, 0);
    }
    
    @Test
    public void testCreateSegmentStartThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment start time must be positive");
        newSegment = new MetadataStream.VideoSegment("vacation", 20, -1);
    }
    
    @Test
    public void testCreateSegmentDurationThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment duration must be positive");
        newSegment = new MetadataStream.VideoSegment("vacation", 20, 0, -1);
    }
    
    @Test
    public void testSetSegmentTitleThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment title can't be empty");
        vs1.setTitle("");
    }
    
    @Test
    public void testSetSegmentFPSThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment fps must be positive");
        vs1.setFPS(0);
    }
    
    @Test
    public void testSetSegmentDurationThrown()
    {
        newSegment = new MetadataStream.VideoSegment();
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment duration must positive");
        newSegment.setDuration(-1);
    }
    
    @Test
    public void testSetSegmentTimeThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment start time must be positive");
        vs1.setTime(-1);
    }
    
    @Test
    public void testSetSegmentResolutionThrown()
    {
        thrown.expect(com.intel.vmf.VmfException.class);
        thrown.expectMessage("vmf::Exception: Segment resoulution width and height must be positive");
        vs1.setResolution(0, 0);
    }
 }
