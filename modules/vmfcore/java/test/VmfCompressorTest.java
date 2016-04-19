import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import com.intel.vmf.Compressor;
import com.intel.vmf.Variant;


public class VmfCompressorTest {

	@Before
    public void setUp() {
    }

    @Test
    public void testCompressor () {
    	String[] ids = Compressor.getRegisteredIds();
        assertNotNull(ids);
        assertTrue(ids.length == 1);
        assertTrue(ids[0].equals(Compressor.BUILTIN_ZLIB));

        Compressor comp = Compressor.create(Compressor.BUILTIN_ZLIB);
        assertNotNull(comp);
        assertTrue(comp.getId().equals(Compressor.BUILTIN_ZLIB));

        Compressor comp2 = comp.createNewInstance();
        assertNotNull(comp2);
        assertTrue(comp2.getId().equals(Compressor.builtinId()));

        String text = "This is a text for Compressor test";
        Variant v = comp.compress(text);
        assertNotNull(v);
        String text2 = comp2.decompress(v);
        assertTrue(text.equals(text2));
    }
}
