import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import com.intel.umf.Log;
import com.intel.umf.umf_vec2d;
import com.intel.umf.umf_vec3d;
import com.intel.umf.umf_vec4d;

public class VmfVec2d3d4dTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected umf_vec2d v2d;
    protected umf_vec3d v3d;
    protected umf_vec4d v4d;
    
    protected umf_vec2d v2d1;
    protected umf_vec3d v3d1;
    protected umf_vec4d v4d1;
    
    @Before
    public void setUp ()
    {
        v2d = new umf_vec2d (1, 2);
        v3d = new umf_vec3d (1, 2, 3);
        v4d = new umf_vec4d (1, 2, 3, 4);
        
        v2d1 = new umf_vec2d ();
        v3d1 = new umf_vec3d ();
        v4d1 = new umf_vec4d ();
    }
    
	@Test
    public void testVmfVec2d3d4d ()
    {
        assertEquals(1, v2d.getX(), 0.00001);
        assertEquals(2, v2d.getY(), 0.00001);
        
        assertEquals(1, v3d.getX(), 0.00001);
        assertEquals(2, v3d.getY(), 0.00001);
        assertEquals(3, v3d.getZ(), 0.00001);
        
        assertEquals(1, v4d.getX(), 0.00001);
        assertEquals(2, v4d.getY(), 0.00001);
        assertEquals(3, v4d.getZ(), 0.00001);
        assertEquals(4, v4d.getW(), 0.00001);
        
        assertEquals(0, v2d1.getX(), 0.00001);
        assertEquals(0, v2d1.getY(), 0.00001);
        
        assertEquals(0, v3d1.getX(), 0.00001);
        assertEquals(0, v3d1.getY(), 0.00001);
        assertEquals(0, v3d1.getZ(), 0.00001);
        
        assertEquals(0, v4d1.getX(), 0.00001);
        assertEquals(0, v4d1.getY(), 0.00001);
        assertEquals(0, v4d1.getZ(), 0.00001);
        assertEquals(0, v4d1.getW(), 0.00001);
    }
	
	@Test
    public void testDeleteByGC()
    {
	    v2d = null;
	    v3d = null;
	    v4d = null;
	    
	    v2d1 = null;
	    v3d1 = null;
	    v4d1 = null;
	    
	    System.gc();
    }
}
