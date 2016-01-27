import com.intel.vmf.FieldDesc;
import com.intel.vmf.Variant;

import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import com.intel.vmf.Vmf;

public class VmfFieldDescTest 
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
    
    protected FieldDesc tmp1;
    protected FieldDesc tmp2;
    protected FieldDesc tmp3;
    protected FieldDesc tmp4;
    protected FieldDesc tmp5;
    protected FieldDesc tmp6;
    protected FieldDesc tmp7;
    protected FieldDesc tmp8;
    protected FieldDesc tmp9;
    
    
    @Before
    public void setUp()
    {
        tmp1 = new FieldDesc ();
        tmp2 = new FieldDesc ("string", Variant.type_string);
        tmp3 = new FieldDesc ("integer", Variant.type_integer, true);
        tmp4 = new FieldDesc ("string", Variant.type_string, false);
        tmp5 = new FieldDesc ("real", Variant.type_real);
        tmp6 = new FieldDesc ("vec2d", Variant.type_vec2d, true);
        tmp7 = new FieldDesc ("vec3d", Variant.type_vec3d, true);
        tmp8 = new FieldDesc ("vec4d", Variant.type_vec4d, true);
        
        tmp9 = new FieldDesc ("rawbuffer", Variant.type_rawbuffer);
    }
    
   @Test
   public void testEquals ()
   {
        System.out.println("Inside VmfFieldDescTest.testEquals()");

        assertTrue (tmp2.equals(tmp4));
        
        assertFalse (tmp1.equals (tmp2));
        assertFalse (tmp1.equals (tmp3));
        assertFalse (tmp1.equals (tmp4)); 
        assertFalse (tmp1.equals (tmp5));
        assertFalse (tmp1.equals (tmp6));
        assertFalse (tmp1.equals (tmp7)); 
        assertFalse (tmp1.equals (tmp8));
        assertFalse (tmp1.equals (tmp9));
   }
   
   @Test
   public void testGetName ()
   {
        System.out.println("Inside VmfFieldDescTest.testGetName()");
        
        assertEquals(tmp2.getName(), tmp4.getName()); 
        
        assertEquals("", tmp1.getName());
        assertEquals("string", tmp2.getName());
        assertEquals("integer", tmp3.getName());
        assertEquals("string", tmp4.getName());
        assertEquals("real", tmp5.getName());
        assertEquals("vec2d", tmp6.getName());
        assertEquals("vec3d", tmp7.getName());
        assertEquals("vec4d", tmp8.getName());
        
        assertEquals("rawbuffer", tmp9.getName());
   }
   
   
   @Test
   public void testGetType ()
   {
        System.out.println("Inside VmfFieldDescTest.testGetType()");
        
        assertEquals(tmp2.getType(), tmp4.getType());
        
        assertEquals(Variant.type_string, tmp1.getType());
        assertEquals(Variant.type_string, tmp2.getType());
        assertEquals(Variant.type_integer, tmp3.getType());
        assertEquals(Variant.type_string, tmp4.getType());
        assertEquals(Variant.type_real, tmp5.getType());
        assertEquals(Variant.type_vec2d, tmp6.getType());
        assertEquals(Variant.type_vec3d, tmp7.getType());
        assertEquals(Variant.type_vec4d, tmp8.getType());
        
        assertEquals(Variant.type_rawbuffer, tmp9.getType());
   }
   
   @Test
   public void testIsOptional()
   {
         System.out.println("Inside VmfFieldDescTest.testIsOptional()");
        
        assertEquals(tmp2.isOptional(), tmp4.isOptional());
        
        assertEquals(false, tmp1.isOptional());
        assertEquals(false, tmp2.isOptional());
        assertEquals(true, tmp3.isOptional());
        assertEquals(false, tmp4.isOptional());
        assertEquals(false, tmp5.isOptional());
        assertEquals(true, tmp6.isOptional());
        assertEquals(true, tmp7.isOptional());
        assertEquals(true, tmp8.isOptional());
        
        assertEquals(false, tmp9.isOptional());
   }
   
}
