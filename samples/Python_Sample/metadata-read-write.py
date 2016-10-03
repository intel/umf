import umflib 


umflib.initialize()
print("UMF sample: read/write GPS location and time \n \n")
FILE_NAME = "global_test.avi"
META_SOURCE_NAME = "test-id"
GPS_DESC = "gps"
GPS_COORD_FIELD = "GPS"
GPS_TIME_FIELD = "Time"
GPS_SCHEMA_NAME = "gps_schema"

GPS_METADATA_ITEM1 = "lat=53.78,lng=132.46";
GPS_METADATA_ITEM2 = "lat=53.28,lng=131.87";
GPS_METADATA_ITEM3 = "lat=52.95,lng=131.41";
GPS_METADATA_ITEM4 = "lat=52.49,lng=130.98";


print("Adding metadata... \n")
print("Opening file name " +FILE_NAME+"\n")


mdStream = umflib.MetadataStream()
print("Metadatastream created")

if not (mdStream.open( FILE_NAME, 2 )):
	print("Unable to initialize \n")

fieldDesc=[]
fieldDesc.append(umflib.FieldDesc(GPS_COORD_FIELD))
fieldDesc.append(umflib.FieldDesc(GPS_TIME_FIELD))

gpsDesc=umflib.MetadataDesc(GPS_DESC,fieldDesc)

gpsSchema=umflib.MetadataSchema(GPS_SCHEMA_NAME)

gpsSchema.add(gpsDesc)

t = "21.02.2013 18:35";

print("Adding MetadataSchema "+GPS_SCHEMA_NAME)

mdStream.addSchema(gpsSchema)

print(mdStream.getAllSchemaNames())
t = "21.02.2013 18:45";
print("Adding metadata's item "+ GPS_METADATA_ITEM1+"with associated time"+t+"\n");

gpsMetadata= umflib.Metadata(gpsDesc)

gpsMetadata.setFieldValue(GPS_COORD_FIELD,umflib.Variant(GPS_METADATA_ITEM1))
gpsMetadata.setFieldValue(GPS_TIME_FIELD, umflib.Variant(t))
mdStream.add(gpsMetadata)

mdStream.save();
mdStream.close();
