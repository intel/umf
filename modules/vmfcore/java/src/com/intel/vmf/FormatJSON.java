package com.intel.vmf;

import java.util.HashMap;
import java.util.Map;

import com.intel.vmf.MetadataStream.VideoSegment;

public class FormatJSON implements Format {
    static
    {
        try
        {
            System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME);
        }
        catch (UnsatisfiedLinkError error1)
        {
            try
            {
                System.loadLibrary(Vmf.NATIVE_LIBRARY_NAME + "d");
            }
            catch (UnsatisfiedLinkError error2)
            {
                throw new java.lang.LinkageError("Native dynamic library is not found");
            }
        }
    }

    public final long nativeObj;

    protected FormatJSON (long addr) {
        if (addr == 0)
            throw new java.lang.IllegalArgumentException("Native object address is NULL");

        nativeObj = addr;
    }

    public FormatJSON() {
        this( n_FormatJSON() );
    }

    @Override
    public String store(
        MetadataSet set,
        MetadataSchema[] schemas,
        VideoSegment[] segments,
        //Stat[] stats,
        Map<String, String> attribs
    ) {
        long[] schemasAddrs = null, segmentsAddrs = null, statsAddrs = null;
        String[] attribNames = null, attribVals = null;

        if(schemas != null && schemas.length > 0) {
          schemasAddrs = new long[schemas.length];
          int i=0;
          for(MetadataSchema s : schemas)
            schemasAddrs[i++] = s.nativeObj;
        }
        if(segments != null && segments.length > 0) {
          segmentsAddrs = new long[segments.length];
          int i=0;
          for(MetadataStream.VideoSegment seg : segments)
            segmentsAddrs[i++] = seg.nativeObj;
        }
        /*
        if(stats != null && stats.length > 0) {
          statsAddrs = new long[stats.length];
          int i=0;
          for(Stat st : stats)
            statsAddrs[i++] = st.nativeObj;
        }
        */
        if(attribs != null && attribs.size()>0) {
          attribNames = new String[attribs.size()];
          attribVals  = new String[attribs.size()];
          int i=0;
          for (Map.Entry<String, String> entry : attribs.entrySet())
          {
            attribNames[i] = entry.getKey();
            attribVals [i] = entry.getValue();
            i++;
          }
        }

        return n_store(nativeObj, (set==null ? 0 : set.nativeObj), schemasAddrs, segmentsAddrs, statsAddrs, attribNames, attribVals);
    }

    @Override
    public Data parse(String text) {
        Data data = new Data();
        final int maxAttribCount = 10;
        String[] attribNames = new String[maxAttribCount], attribVals = new String[maxAttribCount]; 
        long[] objects = n_parse(nativeObj, text, attribNames, attribVals);

        // get item numbers finding zeros
        int cntMd=0, cntSch=0, cntSeg=0, cntSt=0, cntAtt=0;
        for(cntMd=0; cntMd<objects.length; cntMd++)
            if(objects[cntMd] == 0) break;
        for(cntSch=cntMd+1; cntSch<objects.length; cntSch++)
            if(objects[cntSch] == 0) break;
        for(cntSeg=cntSch+1; cntSeg<objects.length; cntSeg++)
            if(objects[cntSeg] == 0) break;
        cntSt = objects.length;
        for(cntAtt=0; cntAtt<attribNames.length; cntAtt++)
            if(attribNames[cntAtt] == null) break;

        // reconstruct objects
        int i;
        if(cntMd>0) {
            data.metadata = new MetadataInternal[cntMd];
            for(i=0; i<cntMd; i++)
                data.metadata[i] = new MetadataInternal(objects[i]);
        }
        if(cntSch-cntMd>1) {
            data.schemas = new MetadataSchema[cntSch-cntMd-1];
            for(i=cntMd+1; i<cntSch; i++)
                data.schemas[i-cntMd-1] = new MetadataSchema(objects[i]);
        }
        if(cntSeg-cntSch>1) {
            data.segments = new MetadataStream.VideoSegment[cntSeg-cntSch-1];
            for(i=cntSch+1; i<cntSeg; i++)
                data.segments[i-cntSch-1] = new MetadataStream.VideoSegment(objects[i]);
        }
        if(cntSt-cntSeg>1) {
            /*
            data.stats = new Stat[cntSt-cntSeg-1];
            for(i=cntSeg+1; i<cntSt; i++)
                data.stats[i-cntSeg-1] = new Stat(objects[i]);
            */
        }
        if(cntAtt>0) {
            data.attrib = new HashMap<String, String>();
            for(i=0; i<cntAtt; i++)
                data.attrib.put(attribNames[i], attribVals[i]);
        }

        return data;
    }

    @Override
    protected void finalize () throws Throwable 
    {
        if (nativeObj != 0)
            n_delete (nativeObj);

        super.finalize();
    }

    private native static long n_FormatJSON();
    private native static String n_store(long nativeObj, long set, long[] schemas, long[] segments, long[] stats, String[] attribNames, String[] attribVals);
    private native static long[] n_parse(long nativeObj, String text, String[] attribNames, String[] attribVals);
    private native static void n_delete (long nativeObj);
}
