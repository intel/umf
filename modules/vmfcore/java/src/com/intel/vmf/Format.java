package com.intel.vmf;

import java.util.HashMap;
import java.util.Map;

public abstract class Format {

	public abstract String store(
	        MetadataSet set,
	        MetadataSchema[] schemas,
	        MetadataStream.VideoSegment[] segments,
	        Stat[] stats,
	        Map<String, String> attribs
	        );

	public abstract Data parse(String text);

	public static final class Data {
		public MetadataInternal[] metadata;
		public MetadataSchema[] schemas;
		public MetadataStream.VideoSegment[] segments;
        public Stat[] stats;
		public Map<String, String> attrib;
		
		public Data(long[] objects, String[] attribNames, String[] attribVals) {
			// get item numbers finding zeros
			int posEndMd=0, posEndSch=0, posEndSeg=0, posEndSt=0, cntAtt=0;
			
			if(objects == null || objects.length == 0) throw new ExceptionIncorrectParam("Empty objects addresses array");

			for(posEndMd=0; posEndMd<objects.length; posEndMd++)
				if(objects[posEndMd] == 0) break;
			if(posEndMd == objects.length) throw new ExceptionIncorrectParam("No zero marker for MetadataInternal[] end");
			
			for(posEndSch=posEndMd+1; posEndSch<objects.length; posEndSch++)
				if(objects[posEndSch] == 0) break;
			if(posEndSch == objects.length) throw new ExceptionIncorrectParam("No zero marker for MetadataSchema[] end");

			for(posEndSeg=posEndSch+1; posEndSeg<objects.length; posEndSeg++)
				if(objects[posEndSeg] == 0) break;
			if(posEndSeg == objects.length) throw new ExceptionIncorrectParam("No zero marker for VideoSegment[] end");

			posEndSt = objects.length;
			
			if( attribNames == null || attribNames.length == 0 ||
				attribVals  == null || attribVals.length  == 0
			   ) throw new ExceptionIncorrectParam("Empty attributes array(s)");
			for(cntAtt=0; cntAtt<attribNames.length; cntAtt++)
				if(attribNames[cntAtt] == null) break;
			
			// reconstruct objects
			int i;
			if(posEndMd>0) {
				metadata = new MetadataInternal[posEndMd];
				for(i=0; i<posEndMd; i++) metadata[i] = new MetadataInternal(objects[i]);
			}
			if(posEndSch-posEndMd>1) {
				schemas = new MetadataSchema[posEndSch-posEndMd-1];
				for(i=posEndMd+1; i<posEndSch; i++) schemas[i-posEndMd-1] = new MetadataSchema(objects[i]);
			}
			if(posEndSeg-posEndSch>1) {
				segments = new MetadataStream.VideoSegment[posEndSeg-posEndSch-1];
				for(i=posEndSch+1; i<posEndSeg; i++) segments[i-posEndSch-1] = new MetadataStream.VideoSegment(objects[i]);
			}
			if(posEndSt-posEndSeg>1) {
				stats = new Stat[posEndSt-posEndSeg-1];
				for(i=posEndSeg+1; i<posEndSt; i++) stats[i-posEndSeg-1] = new Stat(objects[i]);
			}
			if(cntAtt>0) {
				attrib = new HashMap<String, String>();
				for(i=0; i<cntAtt; i++) attrib.put(attribNames[i], attribVals[i]);
			}
		}
		
	}

	// common stuff

    static
    {
        try
        { System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME); }
        catch (UnsatisfiedLinkError error1)
        {
            try
            { System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d"); }
            catch (UnsatisfiedLinkError error2)
            { throw new java.lang.LinkageError("Native dynamic library is not found"); }
        }
    }

    public final long nativeObj;

    protected Format (long addr)
    {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");
        nativeObj = addr;
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0) n_delete (nativeObj);
        super.finalize();
    }

    private native static void n_delete (long nativeObj);
}
