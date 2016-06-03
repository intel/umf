import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

import com.intel.vmf.FieldDesc;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.MetadataSchema;
import com.intel.vmf.MetadataStream;
import com.intel.vmf.Stat;
import com.intel.vmf.StatField;
import com.intel.vmf.Variant;


public class VmfStatTest {

    StatField sf1, sf2, sfX;
    Stat s;

    @Before
    public void setUp() {
        sf1 = new StatField("name1", "schemaName", "metadataName", "fieldName", StatField.BuiltinOp_Min);
        sf2 = new StatField("name2", "schemaName", "metadataName", "fieldName", StatField.BuiltinOp_Min);
        sfX = new StatField("nameX", "schemaNameX", "metadataNameX", "fieldNameX", StatField.BuiltinOp_Last);

        s = new Stat("Stat name", sf1, sf2, sfX);
    }

    @Test
    public void testStatFieldSimple () {
        assertTrue(sf1.equals(sf2));
        assertTrue(sf1.equals(sf2));
        assertFalse(sf1.equals(sfX));

        assertEquals( sf1.getName(), "name1" );
        assertEquals( sf1.getSchemaName(), "schemaName" );
        assertEquals( sf1.getMetadataName(), "metadataName" );
        assertEquals( sf1.getFieldName(), "fieldName" );
        assertEquals( sf1.getOpName(), StatField.BuiltinOp_Min );
        assertNotEquals( sf1.getOpName(), sfX.getOpName() );
    }

    @Test
    public void testStatSimple () {
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

        assertTrue( s.getFieldCopy(sf1.getName()).equals(sf1) );
        assertTrue( s.getFieldCopy(sfX.getName()).equals(sfX) );
    }

    @Test
    public void testStat () {
        MetadataDesc mdDesc = new MetadataDesc("point",
                new FieldDesc("x", Variant.type_integer),
                new FieldDesc("y", Variant.type_real)
        );

        MetadataSchema schema = new MetadataSchema("test schema");
        schema.add(mdDesc);

        MetadataStream stream = new MetadataStream();
        stream.addSchema(schema);

        stream.addStat(
            new Stat("stat",
                new StatField("min-x", schema.getName(), mdDesc.getMetadataName(), "x", StatField.BuiltinOp_Min),
                new StatField("cnt-y", schema.getName(), mdDesc.getMetadataName(), "y", StatField.BuiltinOp_Count)
            )
        );

        String[] sNames = stream.getAllStatNames();
        assertEquals(sNames.length, 1);
        assertEquals(sNames[0], "stat");

        Stat st = stream.getStat("stat");
        assertEquals(st.getUpdateMode(), Stat.UpdateMode_Disabled);
        st.setUpdateMode(Stat.UpdateMode_Manual);
        assertEquals(stream.getStat("stat").getUpdateMode(), Stat.UpdateMode_Manual);

        String[] sfNames = st.getAllFieldNames();
        assertEquals(sfNames.length, 2);
        assertEquals(sfNames[0], "min-x");
        assertEquals(sfNames[1], "cnt-y");

        assertTrue(st.getValue("min-x").equals(new Variant()));
        assertTrue(st.getValue("cnt-y").equals(new Variant(0)));

        Metadata md1 = new Metadata(mdDesc);
        md1.setFieldValue("x", new Variant(1));
        md1.setFieldValue("y", new Variant(1));
        stream.add(md1);
        st.update(true);

        assertTrue(st.getValue("min-x").equals(new Variant(1)));
        assertTrue(st.getValue("cnt-y").equals(new Variant(1)));

        Metadata md2 = new Metadata(mdDesc);
        md2.setFieldValue("x", new Variant(2));
        md2.setFieldValue("y", new Variant(2));
        stream.add(md2);
        st.update(true);
        assertTrue(st.getValue("min-x").equals(new Variant(1)));
        assertTrue(st.getValue("cnt-y").equals(new Variant(2)));

        stream.remove(md1.getID());
        assertTrue(st.getState() == Stat.State_NeedRescan);
        stream.recalcStat();
        st.update(true);
        assertTrue(st.getValue("min-x").equals(new Variant(2)));
        assertTrue(st.getValue("cnt-y").equals(new Variant(1)));
    }
}
