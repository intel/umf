import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import com.intel.vmf.Stat;
import com.intel.vmf.StatField;


public class VmfStatTest {
	
	StatField sf1, sf2, sfX;
	Stat s;

    @Before
    public void setUp() {
    	sf1 = new StatField("name", "schemaName", "metadataName", "fieldName", "opName");
    	sf2 = new StatField("name", "schemaName", "metadataName", "fieldName", "opName");
    	sfX = new StatField("nameX", "schemaNameX", "metadataNameX", "fieldNameX", "opNameX");
    	
    	s = new Stat("Stat name", sf1, sf2, sfX);
    }
    
    @Test
    public void testStatField () {
    	assertTrue(sf1.equals(sf2));
    	assertFalse(sf1.equals(sfX));
    	
    	assertEquals( sf1.getName(), "name" );
    	assertEquals( sf1.getSchemaName(), "schemaName" );
    	assertEquals( sf1.getMetadataName(), "metadataName" );
    	assertEquals( sf1.getFieldName(), "fieldName" );
    	assertEquals( sf1.getOpName(), "opName" );
    	assertNotEquals( sf1.getOpName(), sfX.getOpName() );
    	
    	assertNull(sf1.getMetadataDesc());
    	assertNull(sfX.getFieldDesc());
    }

    @Test
    public void testStat () {
    	assertEquals( s.getName(), "Stat name" );
    	assertEquals( s.getState(), Stat.State_UpToDate );
    	assertEquals( s.getUpdateMode(), Stat.UpdateMode_Disabled);
    	
    	s.setUpdateMode(Stat.UpdateMode_OnTimer);
    	assertEquals( s.getUpdateMode(), Stat.UpdateMode_OnTimer);
    	
    	s.setUpdateTimeout(100);
    	assertEquals( s.getUpdateTimeout(), 100);
    	
    	String[] afn = s.getAllFieldNames();
    	assertEquals(afn.length, 3);
    	assertEquals(afn[0], sf1.getName());
    	assertEquals(afn[1], sf2.getName());
    	assertEquals(afn[2], sfX.getName());
    	
    	assertEquals( s.getField(sf1.getName()), sf1 );
    	assertEquals( s.getField(sfX.getName()), sfX );
    }
}
