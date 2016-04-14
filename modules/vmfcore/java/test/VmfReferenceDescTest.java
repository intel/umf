import com.intel.vmf.Log;
import com.intel.vmf.ReferenceDesc;
import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class VmfReferenceDescTest 
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }

    protected ReferenceDesc ref1;
    protected ReferenceDesc ref2;
    protected ReferenceDesc ref3;
    protected ReferenceDesc ref4;

    @Before
    public void setUp ()
    {
       ref1 = new ReferenceDesc ("friend");
       ref2 = new ReferenceDesc ("colleague", false, true);
       ref3 = new ReferenceDesc ("spouse", true, false);
       ref4 = new ReferenceDesc ();
    }

    @Test
    public void testReferenceDesc ()
    {
        assertEquals("friend", ref1.getName ());
        assertEquals("colleague", ref2.getName());
        assertEquals("spouse", ref3.getName());
        assertEquals("", ref4.getName());

        assertEquals(false, ref1.isUnique());
        assertEquals(false, ref2.isUnique());
        assertEquals(true, ref3.isUnique());
        assertEquals(false, ref4.isUnique());

        assertEquals(false, ref1.isCustom());
        assertEquals(true, ref2.isCustom());
        assertEquals(false, ref3.isCustom());
        assertEquals(false, ref4.isCustom());
    }
}
