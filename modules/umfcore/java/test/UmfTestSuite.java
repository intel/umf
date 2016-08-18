import org.junit.runner.RunWith;
import org.junit.runners.Suite;

@RunWith(Suite.class)
@Suite.SuiteClasses({
  UmfFieldDescTest.class,
  UmfFieldValueTest.class,
  UmfJSONTest.class,
  UmfLogTest.class,
  UmfMetadataDescTest.class,
  UmfMetadataSchemaTest.class,
  UmfMetadataSetTest.class,
  UmfMetadataStreamTest.class,
  UmfMetadataTest.class,
  UmfReferenceDescTest.class,
  UmfReferenceTest.class,
  UmfStatTest.class,
  UmfVariantTest.class,
  UmfVec2d3d4dTest.class,
  UmfVideoSegmentTest.class,
  UmfXMLTest.class,
  UmfStatTest.class,
  UmfCompressorTest.class,
  UmfEncryptorTest.class,
  UmfFormatCompressedTest.class,
  UmfFormatEncryptedTest.class
})

public class UmfTestSuite {}
