import com.intel.vmf.MetadataStream;
import com.intel.vmf.Metadata;
import com.intel.vmf.MetadataSet;
import com.intel.vmf.MetadataDesc;
import com.intel.vmf.FieldDesc;
import com.intel.vmf.FieldValue;
import com.intel.vmf.Log;
import com.intel.vmf.ReferenceDesc;
import com.intel.vmf.Variant;
import com.intel.vmf.MetadataSchema;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;

public class VmfMetadataSetTest
{
    @BeforeClass
    public static void disableLogging()
    {
        Log.setVerbosityLevel(Log.LOG_NO_MESSAGE);
    }

    protected MetadataStream stream;
    protected MetadataSchema schema;

    protected FieldDesc fields[];
    protected ReferenceDesc refs[];

    protected MetadataDesc mdDesc;

    protected MetadataSet mdSet;

    protected Variant var1;
    protected Variant var2;
    protected Variant var3;

    @Before
    public void setUp ()
    {
        fields = new FieldDesc [3];
        fields[0] = new FieldDesc ("name", Variant.type_string, false);
        fields[1] = new FieldDesc ("last name", Variant.type_string, false);
        fields[2] = new FieldDesc ("age", Variant.type_integer, false);

        refs = new ReferenceDesc [3];
        refs[0] = new ReferenceDesc ("friend");
        refs[1] = new ReferenceDesc ("colleague", false, true);
        refs[2] = new ReferenceDesc ("spouse", true, false);

        mdDesc = new MetadataDesc ("person", fields, refs);

        schema = new MetadataSchema ("test_schema");
        stream = new MetadataStream ();

        schema.add (mdDesc);

        Metadata md1 = new Metadata (mdDesc);
        Metadata md2 = new Metadata (mdDesc);
        Metadata md3 = new Metadata (mdDesc);

        stream.addSchema (schema);

        md1.setFrameIndex(0);
        md2.setFrameIndex(4);
        md3.setFrameIndex(0);

        md1.setTimestamp (0, 4);
        md2.setTimestamp (1, 3);
        md3.setTimestamp (3, 4);

        var1 = new Variant ("Den");
        var2 = new Variant ("Smith");
        var3 = new Variant (24);

        md1.setFieldValue("name", var1);
        md1.setFieldValue("last name", var2);
        md1.setFieldValue("age", var3);

        var1.setTo ("Anna");
        var2.setTo ("Smith");
        var3.setTo (22);

        md2.setFieldValue("name", var1);
        md2.setFieldValue("last name", var2);
        md2.setFieldValue("age", var3);

        var1.setTo ("John");
        var2.setTo ("Sean");
        var3.setTo (25);

        md3.setFieldValue("name", var1);
        md3.setFieldValue("last name", var2);
        md3.setFieldValue("age", var3);

        stream.add (md1);
        stream.add (md2);
        stream.add (md3);

        md2.addReference(md2);
        md2.addReference(md1, "spouse");
        md2.addReference(md1, "friend");
        md2.addReference(md3, "friend");
        md2.addReference(md3, "colleague");

        mdSet = stream.getAll();
    }

    @Test
    public void testQueries()
    {
        assertEquals(3, mdSet.getSize());

        MetadataSet mdSet2 = mdSet.queryBySchema("test_schema");
        assertEquals(3, mdSet2.getSize());

        mdSet2 = mdSet.queryByFrameIndex (0);
        assertEquals(2, mdSet2.getSize());

        mdSet2 = mdSet.queryByTime (0, 2);
        assertEquals(2, mdSet2.getSize());

        mdSet2 = mdSet.queryByName("person");
        assertEquals(3, mdSet2.getSize());

        FieldValue fv = new FieldValue("name", var1);

        mdSet2 = mdSet.queryByNameAndValue ("person", fv);
        assertEquals(1, mdSet2.getSize());

        FieldValue fvs[] = new FieldValue [2];

        fvs[0] = new FieldValue("last name", var2);
        fvs[1] = new FieldValue("age", var3);

        mdSet2 = mdSet.queryByNameAndFields ("person", fvs);
        assertEquals(1, mdSet2.getSize());

        mdSet2 = mdSet.queryByReference ("person");
        assertEquals(1, mdSet2.getSize());

        mdSet2 = mdSet.queryByReference ("person", fv);
        assertEquals(1, mdSet2.getSize());

        mdSet2 = mdSet.queryByReference ("person", fvs);
        assertEquals(1, mdSet2.getSize());

        mdSet.shift(25, 6, 2, mdSet2);
        mdSet.shift(25, 6);
    }

    @Test
    public void testDeleteByGC()
    {
        stream = null;
        schema = null;

        mdSet = null;
        mdDesc = null;

        var1 = null;
        var2 = null;
        var3 = null;

        fields = null;
        refs = null;

        System.gc();
    }

    @Rule
    public ExpectedException thrown = ExpectedException.none();

    @Test
    public void testAddRemove()
    {
    	MetadataSet set = new MetadataSet();
    	assertEquals(0, set.getSize());

    	set.push_back(new Metadata(mdDesc));
    	assertEquals(1, set.getSize());
    	set.push_back(new Metadata(mdDesc));
    	assertEquals(2, set.getSize());

    	set.clear();
    	assertEquals(0, set.getSize());

    	set.push_back(new Metadata(mdDesc));
    	assertEquals(1, set.getSize());
    	set.push_back(new Metadata(mdDesc));
    	assertEquals(2, set.getSize());

    	set.erase(1);
    	assertEquals(1, set.getSize());

    	thrown.expect(com.intel.vmf.Exception.class);
        thrown.expectMessage("vmf::Exception: Item index is out of range: 5");
        set.erase(5);
    }
 }
