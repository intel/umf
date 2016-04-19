import org.junit.runner.RunWith;
import org.junit.runners.Suite;

@RunWith(Suite.class)
@Suite.SuiteClasses({
  VmfFieldDescTest.class,
  VmfFieldValueTest.class,
  VmfJSONTest.class,
  VmfLogTest.class,
  VmfMetadataDescTest.class,
  VmfMetadataSchemaTest.class,
  VmfMetadataSetTest.class,
  VmfMetadataStreamTest.class,
  VmfMetadataTest.class,
  VmfReferenceDescTest.class,
  VmfReferenceTest.class,
  VmfStatTest.class,
  VmfVariantTest.class,
  VmfVec2d3d4dTest.class,
  VmfVideoSegmentTest.class,
  VmfXMLTest.class,
  VmfStatTest.class,
  VmfCompressorTest.class,
  VmfEncryptorTest.class,
  VmfFormatCompressedTest.class,
  VmfFormatEncryptedTest.class
})

public class VmfTestSuite {}
