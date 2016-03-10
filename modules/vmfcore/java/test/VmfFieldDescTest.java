import com.intel.vmf.FieldDesc;
import com.intel.vmf.Log;
import com.intel.vmf.Variant;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfFieldDescTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }
    
    protected FieldDesc fd1;
    protected FieldDesc fd2;
    protected FieldDesc fd3;
    protected FieldDesc fd4;
    protected FieldDesc fd5;
    protected FieldDesc fd6;
    protected FieldDesc fd7;
    protected FieldDesc fd8;
    protected FieldDesc fd9;
    
    
    @Before
    public void setUp()
    {
        fd1 = new FieldDesc ();
        fd2 = new FieldDesc ("string", Variant.type_string);
        fd3 = new FieldDesc ("integer", Variant.type_integer, true);
        fd4 = new FieldDesc ("string", Variant.type_string, false);
        fd5 = new FieldDesc ("real", Variant.type_real);
        fd6 = new FieldDesc ("vec2d", Variant.type_vec2d, true);
        fd7 = new FieldDesc ("vec3d", Variant.type_vec3d, true);
        fd8 = new FieldDesc ("vec4d", Variant.type_vec4d, true);
        fd9 = new FieldDesc ("rawbuffer", Variant.type_rawbuffer);
    }
    
   @Test
   public void testEquals ()
   {
        assertTrue (fd2.equals(fd4));
        
        assertFalse (fd1.equals (fd2));
        assertFalse (fd1.equals (fd3));
        assertFalse (fd1.equals (fd4)); 
        assertFalse (fd1.equals (fd5));
        assertFalse (fd1.equals (fd6));
        assertFalse (fd1.equals (fd7)); 
        assertFalse (fd1.equals (fd8));
        assertFalse (fd1.equals (fd9));
   }
   
   @Test
   public void testGetName ()
   {
        assertEquals(fd2.getName(), fd4.getName()); 
        
        assertEquals("", fd1.getName());
        assertEquals("string", fd2.getName());
        assertEquals("integer", fd3.getName());
        assertEquals("string", fd4.getName());
        assertEquals("real", fd5.getName());
        assertEquals("vec2d", fd6.getName());
        assertEquals("vec3d", fd7.getName());
        assertEquals("vec4d", fd8.getName());
        assertEquals("rawbuffer", fd9.getName());
   }
   
   
   @Test
   public void testGetType ()
   {
        assertEquals(fd2.getType(), fd4.getType());
        
        assertEquals(Variant.type_string, fd1.getType());
        assertEquals(Variant.type_string, fd2.getType());
        assertEquals(Variant.type_integer, fd3.getType());
        assertEquals(Variant.type_string, fd4.getType());
        assertEquals(Variant.type_real, fd5.getType());
        assertEquals(Variant.type_vec2d, fd6.getType());
        assertEquals(Variant.type_vec3d, fd7.getType());
        assertEquals(Variant.type_vec4d, fd8.getType());
        assertEquals(Variant.type_rawbuffer, fd9.getType());
   }
   
   @Test
   public void testIsOptional()
   {
        assertEquals(fd2.isOptional(), fd4.isOptional());
        
        assertEquals(false, fd1.isOptional());
        assertEquals(false, fd2.isOptional());
        assertEquals(true, fd3.isOptional());
        assertEquals(false, fd4.isOptional());
        assertEquals(false, fd5.isOptional());
        assertEquals(true, fd6.isOptional());
        assertEquals(true, fd7.isOptional());
        assertEquals(true, fd8.isOptional());
        assertEquals(false, fd9.isOptional());
   }
   
   @Test
   public void testDeleteByGC()
   {
       fd1 = null;
       fd2 = null;
       fd3 = null;
       fd4 = null;
       fd5 = null;
       fd6 = null;
       fd7 = null;
       fd8 = null;
       fd9 = null;
       
       System.gc();
   }
   
}
