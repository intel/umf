
public class VideoSegment
{
        protected final long nativeObj;
        
        protected VideoSegment (long addr)
        {
            if (addr == 0)
                throw new java.lang.UnsupportedOperationException("Native object address is NULL");
            
            nativeObj = addr;
        }
        
        public VideoSegment ()
        {
            nativeObj = n_VideoSegment ();
        }
        
        public VideoSegment (String title, double fps, long startTime)
        {
            long duration = 0;
            long width = 0;
            long height = 0;
            
            nativeObj = n_VideoSegment (title, fps, startTime, duration, width, height);
        }
        
        public VideoSegment (String title, double fps, long startTime, long duration, long width, long height)
        {
            nativeObj = n_VideoSegment (title, fps, startTime, duration, width, height);
        }
        
        public String getTitle ()
        {
            return n_getTitle (nativeObj);
        }
        
        public void setTitle (String title)
        {
            n_setTitle (nativeObj, title);
        }
        
        public double getFPS ()
        {
            return n_getFPS (nativeObj);
        }
        
        public void setFPS (double fps)
        {
            n_setFPS (nativeObj, fps);
        }
        
        public long getDuration ()
        {
            return n_getDuration (nativeObj);
        }
        
        public void setDuration (long duration)
        {
            n_setDuration (nativeObj, duration);
        }
        
        public long getTime ()
        {
            return n_getTime (nativeObj);
        }
        
        public void setTime (long startTime)
        {
            n_setTime (nativeObj, startTime);
        }
        
        public void getResolution (long width, long height)
        {
            n_getResolution (nativeObj, width, height);
        }
        
        public void setResolution(long width, long height)
        {
            n_setResolution (nativeObj, width, height);
        }
        
        @Override
        protected void finalize () throws Throwable 
        {
            if (nativeObj != 0)
                n_delete (nativeObj);
            
            super.finalize();
        }
        
        private native long n_VideoSegment ();
        private native long n_VideoSegment (String title, double fps, long startTime, long duration, long width, long height);
        private native static String n_getTitle (long nativeObj);
        private native static void n_setTitle (long nativeObj, String title);
        private native static double n_getFPS (long nativeObj);
        private native static void n_setFPS (long nativeObj, double fps);
        private native static long n_getDuration (long nativeObj);
        private native static void n_setDuration (long nativeObj, long duration);
        private native static long n_getTime (long nativeObj);
        private native static void n_setTime (long nativeObj, long startTime);
        private native static void n_getResolution (long nativeObj, long width, long height);
        private native static void n_setResolution (long nativeObj, long width, long height);
        private native static void n_delete (long nativeObj);
}
