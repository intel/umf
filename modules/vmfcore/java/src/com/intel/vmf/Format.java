package com.intel.vmf;

import java.util.Map;

public interface Format {

	public String store(
	        MetadataSet set,
	        MetadataSchema[] schemas,
	        MetadataStream.VideoSegment[] segments,
	        //Stat[] stats,
	        Map<String, String> attribs
	        );

	public static final class Data {
		public MetadataInternal[] metadata;
		public MetadataSchema[] schemas;
		public MetadataStream.VideoSegment[] segments;
        //public Stat[] stats;
		public Map<String, String> attrib;
	}
	
	public Data parse(String text);
}
