import junit.framework.*;
import com.intel.vmf.Variant;
import com.intel.vmf.Vmf;
import com.intel.vmf.vmf_vec2d;
import com.intel.vmf.vmf_vec3d;
import com.intel.vmf.vmf_vec4d;

import static org.junit.Assert.*;
import org.junit.Test;

public class VmfVariantTest 
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
    
    protected Variant var1;
    protected Variant var2;
    
    @Before
    public static void setUp ()
    {
        var1 = new Variant ();
        var2 = new Variant ();
    }
    
    @Test
    public void testSetTo()
    {
        System.out.println("Inside VmfVariantTest.testSetTo()");
        
        float value1 = 2.5;
        int value2 = 4;
        double value3 = 3.14;
        
        String str = "ok";
        
        vmf_vec2d vec2d = new vmf_vec2d ();
        vmf_vec3d vec3d = new vmf_vec3d ();
        vmf_vec4d vec4d = new vmf_vec4d ();
        
        assertEquals(Variant.type_unknown, var1.getType());
        
        var1.setTo(value1);
        assertEquals(Variant.type_real, var1.getType());
        
        var1.setTo(value2);
        assertEquals(Variant.type_integer, var1.getType());
        
        var1.setTo(value3);
        assertEquals(Variant.type_real, var1.getType());
        
        var1.setTo(str);
        assertEquals(Variant.type_string, var1.getType());
        
        var1.setTo(vec2d);
        assertEquals(Variant.type_vec2d, var1.getType());
        
        var1.setTo(vec3d);
        assertEquals(Variant.type_vec3d, var1.getType());
        
        var1.setTo(vec4d);
        assertEquals(Variant.type_vec4d, var1.getType());
        
        byte buf[] = new byte [2];
        
        buf[0] = 0;
        buf[1] = 0;
        
        var1.setTo(buf);
        assertEquals(Variant.type_rawbuffer, var1.getType());
        
        double array = new double [2];
        array[0] = 0.0;
        array[1] = 0.0;
        
        var1.setTo(array);
        assertEquals(Variant.type_real_array, var1.getType());
        
        String strArray[] = new String [2];
        strArray[0] = "";
        strArray[1] = "";
        
        var1.setTo(strArray);
        assertEquals(Variant.type_string_array, var1.getType());
        
        vmf_vec2d vec2dArray[] = new vmf_vec2d [2];
        vmf_vec3d vec3dArray[] = new vmf_vec3d [2];
        vmf_vec4d vec4dArray[] = new vmf_vec4d [2];
        
        var1.setTo(vec2dArray);
        assertEquals(Variant.type_vec2d_array , var1.getType());
        
        var1.setTo(vec3dArray);
        assertEquals(Variant.type_vec3d_array , var1.getType());
        
        var1.setTo(vec4dArray);
        assertEquals(Variant.type_vec4d_array , var1.getType());
        
        var2.setTo(var1);
        assertTrue(var1.equals(var2));
        
        var1.clear();
        assertTrue(var1.isEmpty());
        
        assertEquals("vec4d[]", var2.getTypeName());
     }
}
