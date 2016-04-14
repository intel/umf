import com.intel.vmf.Log;
import com.intel.vmf.Variant;
import com.intel.vmf.vmf_vec2d;
import com.intel.vmf.vmf_vec3d;
import com.intel.vmf.vmf_vec4d;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class VmfVariantTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected Variant var1;
    protected Variant var2;
    
    @Before
    public void setUp ()
    {
        var1 = new Variant ();
        var2 = new Variant ();
    }
    
    @Test
    public void testSetTo()
    {
        float floatValue = 2.5f;
        int intValue = 4;
        double doubleValue = 3.14;
        long longValue = 333333333;
        
        String str = "ok";
        
        vmf_vec2d vec2d = new vmf_vec2d ();
        vmf_vec3d vec3d = new vmf_vec3d ();
        vmf_vec4d vec4d = new vmf_vec4d ();
        
        assertEquals(Variant.type_empty, var1.getType());
        
        var1.setTo(floatValue);
        assertEquals(Variant.type_real, var1.getType());
        
        var1.setTo(intValue);
        assertEquals(Variant.type_integer, var1.getType());
        
        var1.setTo(doubleValue);
        assertEquals(Variant.type_real, var1.getType());
        
        var1.setTo(longValue);
        assertEquals(Variant.type_integer, var1.getType());
        
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
        
        long lArray[] = new long [2];
        lArray[0] = 0;
        lArray[1] = 0;
        var1.setTo(lArray);
        assertEquals(Variant.type_integer_vector, var1.getType());
        
        double dArray[] = new double [2];
        dArray[0] = 0.0;
        dArray[1] = 0.0;
        var1.setTo(dArray);
        assertEquals(Variant.type_real_vector, var1.getType());
        
        String strArray[] = new String [2];
        strArray[0] = "";
        strArray[1] = "";
        var1.setTo(strArray);
        assertEquals(Variant.type_string_vector, var1.getType());
        
        float fArray[] = new float [2];
        fArray[0] = 0.f;
        fArray[1] = 0.f;
        var1.setTo(fArray);
        assertEquals(Variant.type_real_vector, var1.getType());
        
        int iArray[] = new int [2];
        iArray[0] = 0;
        iArray[1] = 0;
        var1.setTo(iArray);
        assertEquals(Variant.type_integer_vector, var1.getType());
        
        vmf_vec2d vec2dArray[] = new vmf_vec2d [2];
        vmf_vec3d vec3dArray[] = new vmf_vec3d [2];
        vmf_vec4d vec4dArray[] = new vmf_vec4d [2];
        
        vec2dArray[0] = new vmf_vec2d();
        vec2dArray[1] = new vmf_vec2d();
        
        vec3dArray[0] = new vmf_vec3d();
        vec3dArray[1] = new vmf_vec3d();
        
        vec4dArray[0] = new vmf_vec4d();
        vec4dArray[1] = new vmf_vec4d();
        
        var1.setTo(vec2dArray);
        assertEquals(Variant.type_vec2d_vector , var1.getType());
        
        var1.setTo(vec3dArray);
        assertEquals(Variant.type_vec3d_vector , var1.getType());
        
        var1.setTo(vec4dArray);
        assertEquals(Variant.type_vec4d_vector , var1.getType());
        
        var2.setTo(var1);
        assertTrue(var1.equals(var2));
        
        var1.clear();
        assertTrue(var1.isEmpty());
        
        assertEquals("vec4d[]", var2.getTypeName());
        
        var2.setTo(longValue);
        String value = var2.toString();
        assertFalse(value.isEmpty());
        
        var2.fromString(Variant.type_integer, value);
        
        var2.fromString(Variant.type_real, "5.5");
        assertEquals(var2.getType(), Variant.type_real);
        value = var2.toString();
        assertEquals("5.5", value);
        
        String typeStr = Variant.typeToString(Variant.type_vec4d_vector);
        assertEquals ("vec4d[]", typeStr);
        
        int type = Variant.typeFromString(typeStr);
        assertEquals(Variant.type_vec4d_vector, type);
    }

    @Test
    public void testToAndFromString()
    {
        long longValue = 333333333;

        var2.setTo(longValue);
        String value = var2.toString();
        assertFalse(value.isEmpty());

        var2.fromString(Variant.type_integer, value);

        var2.fromString(Variant.type_real, "5.5");
        assertEquals(var2.getType(), Variant.type_real);

        value = var2.toString();
        assertEquals("5.5", value);

        value = var2.toString(true);
        assertEquals("(real) 5.5", value);

        String typeStr = Variant.typeToString(Variant.type_vec4d_vector);
        assertEquals ("vec4d[]", typeStr);

        int type = Variant.typeFromString(typeStr);
        assertEquals(Variant.type_vec4d_vector, type);

        assertTrue(Variant.isConvertible(Variant.type_integer, Variant.type_real));
        assertFalse(Variant.isConvertible(Variant.type_integer, Variant.type_string));

        String base64Str = "trampampampampam";
        byte[] buf = Variant.base64Decode(base64Str);
        assertNotEquals(0, buf.length);

        assertEquals(base64Str, Variant.base64Encode(buf));
    }

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testDecodeThrow ()
    {
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Invalid base64 string: more than 2 trailing '=' symbols");
        Variant.base64Decode("trampampampam===");
    }
    
    @Test
    public void testFromStringThrow ()
    {
        vmf_vec4d vec4dArray[] = new vmf_vec4d [2];
        vec4dArray[0] = new vmf_vec4d();
        vec4dArray[1] = new vmf_vec4d();
        var1.setTo(vec4dArray);
        
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Invalid array item separator");
        var1.fromString(Variant.type_vec4d_vector, "0 0 0 0 : 0 0 0 0");
    }
    
    @Test
    public void testConvertToThrow ()
    {
        long longValue = 555555;
        var1.setTo(longValue);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Non-existent type.");
        var1.convertTo (14);
    }

    @Test
    public void testConvertToStringThrow ()
    {
        long longValue = 555555;
        var1.setTo(longValue);
        thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Cannot convert value to the target type!");
        var1.convertTo (Variant.type_string);
    }
}
